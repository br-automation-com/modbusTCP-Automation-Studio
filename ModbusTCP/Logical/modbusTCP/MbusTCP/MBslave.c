// ------------------------------------------------------------------------------------------------- 
// Library: MbusTCP
// Author: Stephan Stricker
// Created: October 29, 2022
// ------------------------------------------------------------------------------------------------- 
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
#include "MbusTCP.h"
#ifdef __cplusplus
	};
#endif

#ifdef __i386__
#define IsI386 					1
#else
#define IsI386					0
#endif

#ifdef __arm__
#define IsARM 					1
#else
#define IsARM					0
#endif

// ------------------------------------------------------------------------------------------------- 
// Local functions 
// ------------------------------------------------------------------------------------------------- 
void	add_master					(struct MBslave* inst, modbus_log_typ LOGBOOK);
void	remove_master				(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK);
UINT 	analyze_req_from_master		(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK);
UINT	read_discrete_inputs		(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK);
UINT	read_coils					(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK);
UINT	read_holding_registers		(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK);
UINT	read_input_registers		(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK);
UINT	write_single_coil			(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK);
UINT	write_single_register		(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK);
UINT	write_multiple_coils		(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK);
UINT	write_multiple_registers	(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK);
UINT	create_error_message		(struct MBslave* inst, USINT master, USINT error_code, UINT error_last);

UINT MySwapUINT(UINT data);
INT MySwapINT(INT data);
void NewLogEntry(STRING message[LOG_LINE_LEN], modbus_log_typ LOGBOOK);
void MessageLog(USINT type, USINT function_code, UINT start_addr, UINT length, modbus_log_typ LOGBOOK);

// ------------------------------------------------------------------------------------------------- 
// Slave function block 
// ------------------------------------------------------------------------------------------------- 
void MBslave(struct MBslave* inst)
{
	USINT 	i, j;
	UDINT 	NO_DELAY = 1;		// Send frame without delay 

	switch(inst->internal.step)
	{
		// ----------------------------------------------------------------------------------------- 
		// Assign buffers and port 
		case INIT:
			// ----------------------------------------------------------------------------------------- 
			// Check configuration 
			if((sizeof(inst->internal.master_info)/sizeof(inst->internal.master_info[0]) != sizeof(inst->internal.tcp_receive)/sizeof(inst->internal.tcp_receive[0])) ||
				(sizeof(inst->internal.receive_buff)/sizeof(inst->internal.receive_buff[0]) != sizeof(inst->internal.tcp_receive)/sizeof(inst->internal.tcp_receive[0])))
			{
				NewLogEntry("Configuration missmatch num masters", inst->log);
				inst->status 		= 50110;
				inst->internal.step = ERROR;
				break;
			}
			
			if(inst->port == 0) inst->port = MODBUS_TCP_PORT;

			inst->internal.tcp_open.pIfAddr	= 0;
			inst->internal.tcp_open.port	= inst->port;
			inst->internal.tcp_open.options	= tcpOPT_REUSEADDR;
			inst->internal.tcp_send.enable	= 1;
			inst->internal.tcp_send.pData	= (UDINT) inst->internal.send_buff;
		// ----------------------------------------------------------------------------------------- 
		// Wait for function block enable 
		case DISABLE:
			inst->status = ERR_FUB_ENABLE_FALSE;
			inst->internal.substep = 0;
			inst->last_error = 0;
			if(inst->enable)
			{
				inst->internal.step = GET_IP;
			}
			break;
		// ----------------------------------------------------------------------------------------- 
		// Read own IP address 
		case GET_IP:
			if(inst->enable)
			{
				inst->internal.tcp_ip.enable 	= 1;
				inst->internal.tcp_ip.pDevice 	= (UDINT)inst->device;
				inst->internal.tcp_ip.pIPAddr 	= (UDINT)inst->my_ip;
				inst->internal.tcp_ip.Len 		= 18; 
				CfgGetIPAddr(&inst->internal.tcp_ip);
				// --------------------------------------------------------------------------------- 
				// Success  
				if (inst->internal.tcp_ip.status == 0)
				{
					if(brsstrcmp((UDINT)inst->my_ip, (UDINT)"0.0.0.0") == 0) break;
					inst->internal.step = OPEN_PORT;
				}
				else if(inst->internal.tcp_ip.status != BUSY) inst->internal.step = ERROR;
			}
			else inst->internal.step = CLOSE_PORT;
			inst->status = inst->internal.tcp_ip.status;
			break;
		// ----------------------------------------------------------------------------------------- 
		// Open ethernet port 
		case OPEN_PORT:
			if(inst->enable)
			{
				inst->internal.tcp_open.enable	= 1;
				TcpOpen(&inst->internal.tcp_open);
				// --------------------------------------------------------------------------------- 
				// Success  	
				if (inst->internal.tcp_open.status == 0)
				{
					NewLogEntry("Open port successful", inst->log);
					inst->internal.tcp_open.enable = 0;
					TcpOpen(&inst->internal.tcp_open);
						
					inst->internal.tcp_server.enable	= 1;
					inst->internal.tcp_server.ident 	= inst->internal.tcp_open.ident;
					inst->internal.tcp_server.backlog	= 0;
					inst->internal.tcp_server.pIpAddr	= (UDINT)inst->internal.master_addr;
				
					inst->internal.step = WAIT_REQUEST;
				}
				else if(inst->internal.tcp_open.status != BUSY) inst->internal.step = ERROR;
			}
			else inst->internal.step = CLOSE_PORT;
			inst->status = inst->internal.tcp_open.status;
			break;
		// ------------------------------------------------------------------------------------------ 
		// Set linger options 			
		case SET_LINGER:
			if(inst->enable)
			{
				// Set linger options 
				inst->internal.linger_opt.lLinger = 0;
				inst->internal.linger_opt.lOnOff = 1;
			
				inst->internal.tcp_ioctl.enable		= 1;
				inst->internal.tcp_ioctl.ident 		= inst->internal.master_info[inst->internal.master_last].ident;
				inst->internal.tcp_ioctl.ioctl		= tcpSO_LINGER_SET;
				inst->internal.tcp_ioctl.pData		= (UDINT) &(inst->internal.linger_opt);
				inst->internal.tcp_ioctl.datalen 	= sizeof(inst->internal.linger_opt);
				TcpIoctl(&inst->internal.tcp_ioctl);
				// ---------------------------------------------------------------------------------- 
				// Success  	
				if (inst->internal.tcp_ioctl.status == 0)
				{			
					inst->internal.step = SET_NODELAY;
				}
				else if(inst->internal.tcp_ioctl.status != BUSY) inst->internal.step = CLOSE_PORT;
			}
			else inst->internal.step = CLOSE_PORT;
			inst->status = inst->internal.tcp_ioctl.status;
			break;
		// ------------------------------------------------------------------------------------------ 
		// Set no delay options 			
		case SET_NODELAY:
			if(inst->enable)
			{
				inst->internal.tcp_ioctl.enable		= 1;
				inst->internal.tcp_ioctl.ident 		= inst->internal.master_info[inst->internal.master_last].ident;
				inst->internal.tcp_ioctl.ioctl		= tcpTCP_NODELAY_SET;
				inst->internal.tcp_ioctl.pData		= (UDINT) &NO_DELAY;
				inst->internal.tcp_ioctl.datalen 	= sizeof(NO_DELAY);
				TcpIoctl(&inst->internal.tcp_ioctl);
				// ---------------------------------------------------------------------------------- 
				// Success  	
				if (inst->internal.tcp_ioctl.status == 0)
				{			
					inst->internal.step = WAIT_REQUEST;
				}
				else if(inst->internal.tcp_ioctl.status != BUSY) inst->internal.step = CLOSE_PORT;
			}
			else inst->internal.step = CLOSE_PORT;
			inst->status = inst->internal.tcp_ioctl.status;
			break;
		// ------------------------------------------------------------------------------------------ 
		// Wait for new master request 
		case WAIT_REQUEST:
			if(inst->enable)
			{
				// ---------------------------------------------------------------------------------- 
				// Look for new masters that want to connect 
				TcpServer(&inst->internal.tcp_server);
				// ---------------------------------------------------------------------------------- 
				// If a new master is connected, add it to the structure  	
				if(inst->internal.tcp_server.status == 0) 
				{
					add_master(inst, inst->log);
					if(IsI386 || IsARM) inst->internal.step = SET_LINGER;
					break;
				}
				else if(inst->internal.tcp_server.status == tcpERR_INVALID_IDENT || inst->internal.tcp_server.status == tcpERR_NOMORE_IDENTS || inst->internal.tcp_server.status == tcpERR_SOCKET_LISTEN || inst->internal.tcp_server.status == tcpERR_SOCKET_ACCEPT)
				{
					NewLogEntry("ERR: TcpServer", inst->log);
					inst->last_error = inst->internal.tcp_server.status;
					inst->internal.step = CLOSE_PORT;
				}
				inst->status = inst->internal.tcp_server.status;

				// ---------------------------------------------------------------------------------- 
				// Handle master connections 
				switch(inst->internal.substep)
				{
					// ------------------------------------------------------------------------------ 
					case SUB_WAIT:
						// -------------------------------------------------------------------------- 
						// Find next next master 
						inst->internal.i = inst->internal.master_last+1;
						if (inst->internal.i >= sizeof(inst->internal.master_info)/sizeof(inst->internal.master_info[0])) inst->internal.i = 0;
						while(inst->internal.tcp_receive[inst->internal.i].enable == 0)
						{
							inst->internal.i++;
							if (inst->internal.i >= sizeof(inst->internal.master_info)/sizeof(inst->internal.master_info[0])) inst->internal.i = 0;
							if(inst->internal.master_last == inst->internal.i) break;
						}
						inst->internal.master_last = inst->internal.i;
						if(inst->internal.tcp_receive[inst->internal.i].enable == 0) break;

						// -------------------------------------------------------------------------- 
						// Check for new data 
						TcpRecv(&inst->internal.tcp_receive[inst->internal.i]);
						// -------------------------------------------------------------------------- 
						// New frame received 	
						if((inst->internal.tcp_receive[inst->internal.i].status == 0) && 
							(inst->internal.tcp_receive[inst->internal.i].recvlen !=0))
						{
							// ---------------------------------------------------------------------- 
							// Make sure frame fits into buffer 	
							if(inst->internal.tcp_receive[inst->internal.i].recvlen < inst->internal.tcp_receive[inst->internal.i].datamax)
							{
								inst->internal.master_info[inst->internal.i].timeout_timer.IN = 0;
								inst->internal.master_info[inst->internal.i].disconnect = 0;
								inst->internal.send_byte = analyze_req_from_master(inst, inst->internal.i, inst->log);
								inst->internal.substep = SUB_SEND;
							}
							else
							{
								inst->internal.send_byte = create_error_message(inst, inst->internal.i, ERROR_MOD_ADR, ERROR_REC_BUF_EXCEEDED);
								NewLogEntry("Receive buffer exceeded", inst->log);
								inst->internal.substep = SUB_SEND;
							}
						}
							// ------------------------------------------------------------------------------------------ 
							// Increase timeout timer if master is idleing 
						else if((inst->internal.tcp_receive[inst->internal.i].status == BUSY) 	||
							(inst->internal.tcp_receive[inst->internal.i].status == DISABLE) 	||
							(inst->internal.tcp_receive[inst->internal.i].status == tcpERR_NO_DATA))
						{
							if ((inst->master_timeout != 0) && (inst->internal.master_info[inst->internal.i].ident != 0))
							{
								inst->internal.master_info[inst->internal.i].timeout_timer.IN = 1;
								inst->internal.master_info[inst->internal.i].timeout_timer.PT = inst->master_timeout;
							}
						}
							// ------------------------------------------------------------------------------------------ 
							// Master closed the connection 
						else if((inst->internal.tcp_receive[inst->internal.i].status == tcpERR_NOT_CONNECTED) ||
							((inst->internal.tcp_receive[inst->internal.i].status == 0) &&
							(inst->internal.tcp_receive[inst->internal.i].recvlen == 0)))
						{
							NewLogEntry("Master closed connection", inst->log);
							inst->internal.master_info[inst->internal.i].timeout_timer.IN = 0;
							inst->internal.master_info[inst->internal.i].disconnect = 0;
							inst->internal.substep = SUB_CLOSE_MASTER;
						}
							// ------------------------------------------------------------------------------------------ 
							// Close master connection when error occurs 
						else if(inst->internal.tcp_receive[inst->internal.i].status != 0)
						{
							NewLogEntry("ERR: TcpRecv", inst->log);
							inst->internal.master_info[inst->internal.i].timeout_timer.IN = 0;
							inst->internal.master_info[inst->internal.i].disconnect = 0;
							inst->internal.substep = SUB_CLOSE_MASTER;
						}
						if(inst->internal.tcp_receive[inst->internal.i].status != tcpERR_NO_DATA)	inst->status = inst->internal.tcp_receive[inst->internal.i].status; 
						else 																		inst->status = 0;
						
						// ------------------------------------------------------------------------------------------ 
						// Close master connection when master times out 
						if ((inst->internal.master_info[inst->internal.i].disconnect == 1) && (inst->internal.master_info[inst->internal.i].ident != 0))
						{
							NewLogEntry("Master timed out", inst->log);
							inst->internal.master_info[inst->internal.i].timeout_timer.IN = 0;
							inst->internal.master_info[inst->internal.i].disconnect = 0;
							inst->last_error = ERROR_MASTER_TIMED_OUT;
							inst->internal.substep = SUB_CLOSE_MASTER;
						}
						// Return response in same cycle if running on SG4 target 
						if((inst->internal.substep != SUB_SEND) || !IsI386 || !IsARM) break;
					// ------------------------------------------------------------------------------ 
					// Send answer to master 							
					case SUB_SEND:		
						inst->internal.tcp_send.ident	= inst->internal.tcp_receive[inst->internal.i].ident;
						inst->internal.tcp_send.datalen	= inst->internal.send_byte;
						inst->internal.tcp_send.flags	= 0;
						TcpSend(&inst->internal.tcp_send);
						// ---------------------------------------------------------------------------------- 
						// Success  	
						if(inst->internal.tcp_send.status == 0)			inst->internal.substep = SUB_WAIT;
						else if(inst->internal.tcp_send.status != BUSY)	inst->internal.substep = SUB_CLOSE_MASTER;
						inst->status = inst->internal.tcp_send.status;
					
						// ---------------------------------------------------------------------------------- 
						// Check for timeout of the master timeout is activated	 
						if ((inst->internal.master_info[inst->internal.i].disconnect == 1) && (inst->internal.master_info[inst->internal.i].ident != 0))
						{
							NewLogEntry("Master timeout", inst->log);
							inst->internal.master_info[inst->internal.i].timeout_timer.IN = 0;
							inst->internal.master_info[inst->internal.i].disconnect = 0;
							inst->internal.substep = SUB_CLOSE_MASTER;
						}
						break;
					// ------------------------------------------------------------------------------ 
					// Remove master 					
					case SUB_CLOSE_MASTER:	
						inst->internal.tcp_close.enable 	= 1;
						inst->internal.tcp_close.ident		= inst->internal.master_info[inst->internal.i].ident;
						inst->internal.tcp_close.how 		= 0;
						TcpClose(&inst->internal.tcp_close);
						// ---------------------------------------------------------------------------------- 
						// Success  	
						if(inst->internal.tcp_close.status == 0)
						{
							remove_master(inst, inst->internal.i, inst->log);
							inst->internal.master_info[inst->internal.i].disconnect = 0;
							inst->internal.substep = SUB_WAIT;
						}
						else if(inst->internal.tcp_close.status != BUSY)
						{
							NewLogEntry("ERR: TcpClose", inst->log);
							inst->internal.step = CLOSE_PORT;
						}
						inst->status = inst->internal.tcp_close.status; 
						break;
				}
			}
			else inst->internal.step = CLOSE_PORT;
			break;		
		// ------------------------------------------------------------------------------------------ 
		// Close ethernet port 				
		case CLOSE_PORT:
			inst->internal.tcp_close.enable 	= 1;
			inst->internal.tcp_close.ident		= inst->internal.tcp_open.ident;
			inst->internal.tcp_close.how 		= 0;
			TcpClose(&inst->internal.tcp_close);
			if(inst->internal.tcp_close.status != 65535)
			{
				for (i=0;i<sizeof(inst->internal.master_info)/sizeof(inst->internal.master_info[0]);i++) remove_master(inst, i, inst->log);
				inst->status = inst->internal.tcp_close.status;
				NewLogEntry("Port closed", inst->log);
				inst->internal.step = ERROR;
			}
			break;
		// ------------------------------------------------------------------------------------------ 
		// Error step 			
		case ERROR:
			if (inst->enable == 0) 
			{
				inst->internal.step = DISABLE;
				break;
			}

			inst->last_error = ERROR_CRITICAL;		
			break;
	}
	
	// ------------------------------------------------------------------------------------------ 
	// Run timer for each master to disconnect when it times out 
	for (j=0;j<sizeof(inst->internal.master_info)/sizeof(inst->internal.master_info[0]);j++)
	{
		TON(&inst->internal.master_info[j].timeout_timer);
		
		if (inst->internal.master_info[j].timeout_timer.Q == 1)
		{
			inst->internal.master_info[j].disconnect = 1;
			inst->internal.master_info[j].timeout_timer.IN = 0;
		}
	}
}

void add_master(struct MBslave* inst, struct modbus_log_typ LOGBOOK)
{
	inst->internal.master_last = 0;
	
	if(inst->internal.master_nr < sizeof(inst->internal.master_info)/sizeof(inst->internal.master_info[0]))											// If there is a free master 
	{
		for(inst->internal.master_last = 0; inst->internal.master_last < sizeof(inst->internal.master_info)/sizeof(inst->internal.master_info[0]); inst->internal.master_last++)						// Search for a free master 
		{
			if(inst->internal.master_info[inst->internal.master_last].ident == 0)
			{
				inst->internal.master_info[inst->internal.master_last].ident 	= inst->internal.tcp_server.identclnt;
				inst->internal.master_info[inst->internal.master_last].port		= inst->internal.tcp_server.portclnt;
				brsmemcpy((UDINT)inst->internal.master_info[inst->internal.master_last].ip_addr, (UDINT)inst->internal.master_addr, sizeof(inst->internal.master_addr));
				inst->internal.tcp_receive[inst->internal.master_last].enable	= 1;
				inst->internal.tcp_receive[inst->internal.master_last].ident		= inst->internal.tcp_server.identclnt;
				inst->internal.tcp_receive[inst->internal.master_last].pData		= (UDINT)inst->internal.receive_buff[inst->internal.master_last].byte;
				inst->internal.tcp_receive[inst->internal.master_last].datamax	= sizeof(inst->internal.receive_buff[inst->internal.master_last].byte);
				inst->internal.tcp_receive[inst->internal.master_last].flags		= 0;
				inst->internal.master_nr++;
				
				NewLogEntry("Added master", LOGBOOK);
				NewLogEntry((STRING *)inst->internal.master_info[inst->internal.master_last].ip_addr, LOGBOOK);
				return;
			}
		}
	}
	else
	{
		NewLogEntry("ERR: Too many masters", LOGBOOK);
		inst->last_error = ERROR_TOO_MANY_MASTERS;
	}
}

void remove_master(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK)
{ 
	// Remove master only if the number of masters is higher than 0 
	if (inst->internal.master_nr > 0)
	{
		NewLogEntry("Removed master", LOGBOOK);
		NewLogEntry((STRING *)inst->internal.master_info[master].ip_addr, LOGBOOK);
	
		inst->internal.tcp_receive[master].enable 	= 0;
		inst->internal.master_info[master].ident	= 0;
		inst->internal.master_info[master].port		= 0;
		brsmemset((UDINT)inst->internal.master_info[master].ip_addr, 0, sizeof(inst->internal.master_addr));
		inst->internal.master_nr--;
	}
}

UINT analyze_req_from_master(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK)
{
	t_master_request			request;
	UINT						uint_var;

	if(inst->internal.tcp_receive[master].recvlen < 8)											// If buffer length is less than the minimum frame length 
	{
		NewLogEntry("ERR: Message too small", LOGBOOK);
		return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_MESSAGE_SIZE);
	}
	else
	{
		brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[0]), sizeof(uint_var));
		request.transaction_id = MySwapUINT(uint_var);
		brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[2]), sizeof(uint_var));
		request.protocol_id = MySwapUINT(uint_var);
		brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[4]), sizeof(uint_var));
		request.length = MySwapUINT(uint_var);	
		request.unit = inst->internal.receive_buff[master].byte[6];
		request.function_code = inst->internal.receive_buff[master].byte[7];
		brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[10]), sizeof(uint_var));
		request.quantity = MySwapUINT(uint_var);
		
		if(	(request.protocol_id	!= 0)	||
			(request.length			== 0)	||
			(request.function_code	== 0))
		{
			NewLogEntry("ERR: Bad message format", LOGBOOK);
			return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_MESSAGE_FORMAT);
		}
		else if ((request.function_code	== READ_COILS || request.function_code	== READ_DISCRETE_INPUTS || request.function_code == WRITE_MULTIPLE_COILS) && request.quantity > MODBUS_MAX_REG*2*8)
		{
			NewLogEntry("ERR: Max length exceeded", LOGBOOK);
			return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_QUANTITY);
		}
		else if ((request.function_code	== READ_HOLDING_REGISTERS || request.function_code	== READ_INPUT_REGISTERS || request.function_code == WRITE_MULTIPLE_REGISTERS) && request.quantity > MODBUS_MAX_REG)
		{
			NewLogEntry("ERR: Max length exceeded", LOGBOOK);
			return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_QUANTITY);
		}
		else
		{
			switch(request.function_code)
			{
				case READ_COILS:							// 0x01 
					return read_coils(inst, master, LOGBOOK);
				
				case READ_DISCRETE_INPUTS:					// 0x02 
					return read_discrete_inputs(inst, master, LOGBOOK);
				
				case READ_HOLDING_REGISTERS:				// 0x03 
					return read_holding_registers(inst, master, LOGBOOK);	
				
				case READ_INPUT_REGISTERS:					// 0x04 
					return read_input_registers(inst, master, LOGBOOK);	
				
				case WRITE_SINGLE_COIL:						// 0x05 
					return write_single_coil(inst, master, LOGBOOK);
				
				case WRITE_SINGLE_REGISTER:					// 0x06 
					return write_single_register(inst, master, LOGBOOK);

				case WRITE_MULTIPLE_COILS:					// 0x0F 
					return write_multiple_coils(inst, master, LOGBOOK);

				case WRITE_MULTIPLE_REGISTERS:				// 0x10 
					return write_multiple_registers(inst, master, LOGBOOK);
				
				default:
					NewLogEntry("ERR: Unsupported function code", LOGBOOK);
					return create_error_message(inst, master, ERROR_MOD_FUNCTION, ERROR_FUNCTION_CODE);
			}
			return 0;
		}
	}
}

UINT create_error_message(struct MBslave* inst, USINT master, USINT error_code, UINT error_last)
{
	UINT	length;
	UINT	uint_var;
		
	// Messaggio di errore per function code non definito 
	brsmemset((UDINT)inst->internal.send_buff, 0, sizeof(inst->internal.send_buff));
	// Transaction id: 2 byte 
	brsmemcpy((UDINT)&inst->internal.send_buff[0], (UDINT)&inst->internal.receive_buff[master].byte[0], sizeof(uint_var));
	// Protocol id: 2 byte 
	brsmemcpy((UDINT)&inst->internal.send_buff[2], (UDINT)&inst->internal.receive_buff[master].byte[2], sizeof(uint_var));
	// Length: 2 byte 
	length = 3;
	uint_var = MySwapUINT(length);
	brsmemcpy((UDINT)&inst->internal.send_buff[4], (UDINT)&uint_var, sizeof(uint_var));
	// Unit: 1 byte 
	inst->internal.send_buff[6] = inst->internal.receive_buff[master].byte[6];
	// Function code: 1 byte 
	inst->internal.send_buff[7] = inst->internal.receive_buff[master].byte[7] + 0x80;
	// Exception code: 1 byte 
	inst->internal.send_buff[8] = error_code;
	
	inst->last_error = error_last;
	return 9;
}

UINT read_coils(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK)
{
	t_read_request		read_coils;
	UINT				i;
	UINT				byte_nr;
	UINT				uint_var;
	UINT				length;
	
	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[8]), sizeof(uint_var));
	read_coils.starting_addr = MySwapUINT(uint_var);
	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[10]), sizeof(uint_var));
	read_coils.quantity = MySwapUINT(uint_var);
	
	MessageLog (TYP_REQUEST, READ_COILS, read_coils.starting_addr, read_coils.quantity, LOGBOOK);

	if( read_coils.starting_addr <= sizeof((*(inst->p_cfg)).p_coils)/4)
	{
		if((read_coils.starting_addr + read_coils.quantity) <= sizeof((*(inst->p_cfg)).p_coils)/4)
		{
			if ((read_coils.quantity % 8) > 0)	
			{
				byte_nr = (read_coils.quantity / 8) + 1;
			}
			else								
			{
				byte_nr = (read_coils.quantity / 8);
			}
			brsmemset((UDINT)inst->internal.send_buff, 0, sizeof(inst->internal.send_buff));
			// Transaction id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[0], (UDINT)&inst->internal.receive_buff[master].byte[0], sizeof(uint_var));
			// Protocol id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[2], (UDINT)&inst->internal.receive_buff[master].byte[2], sizeof(uint_var));
			// Length: 2 byte 
			length = 3 + byte_nr;
			uint_var = MySwapUINT(length);
			brsmemcpy((UDINT)&inst->internal.send_buff[4], (UDINT)&uint_var, sizeof(uint_var));
			// Unit: 1 byte 
			inst->internal.send_buff[6] = inst->internal.receive_buff[master].byte[6];
			// Function code: 1 byte 
			inst->internal.send_buff[7] = inst->internal.receive_buff[master].byte[7];
			// Byte nr: 1 byte 
			inst->internal.send_buff[8] = byte_nr;
			for(i = 0; i < read_coils.quantity; i++)
			{
				if(((*(inst->p_cfg)).p_coils[read_coils.starting_addr+i]) != 0)
				{
					if((*((BOOL*)((*(inst->p_cfg)).p_coils[read_coils.starting_addr+i]))))
					{	
						inst->internal.send_buff[9 + (i/8)] = inst->internal.send_buff[9 + (i/8)] | (USINT)(0x01 << (i%8));
					}
					else
					{
						inst->internal.send_buff[9 + (i/8)] = inst->internal.send_buff[9 + (i/8)] & (USINT)~(0x01 << (i%8));
					}
				}
				else // Variable address is empty 
				{
					NewLogEntry("ERR: Empty variable ADR", LOGBOOK);
					return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_ADR_EMPTY);
				}			
			}
			MessageLog (TYP_RESPONSE, READ_COILS, read_coils.starting_addr, read_coils.quantity, LOGBOOK);
			return length + 6;
		}
		else // Length exceeds maximum range 
		{
			NewLogEntry("ERR: Length exceeds max", LOGBOOK);
			return create_error_message(inst, master, ERROR_MOD_ADR, ERROR_LENGTH_EXCEEDED);
		}			
	}
	else // Address exceed maximum range 
	{
		NewLogEntry("ERR: Address exceeds max", LOGBOOK);
		return create_error_message(inst, master, ERROR_MOD_ADR, ERROR_ADR_EXCEEDED);
	}
	return 0;
}

UINT read_discrete_inputs(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK)
{
	t_read_request	read_discrete_inputs;
	UINT			i;
	UINT			byte_nr;
	UINT			uint_var;
	UINT			length;

	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[8]), sizeof(uint_var));
	read_discrete_inputs.starting_addr = MySwapUINT(uint_var);
	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[10]), sizeof(uint_var));
	read_discrete_inputs.quantity = MySwapUINT(uint_var);
	
	MessageLog (TYP_REQUEST, READ_DISCRETE_INPUTS, read_discrete_inputs.starting_addr, read_discrete_inputs.quantity, LOGBOOK);
	
	if( read_discrete_inputs.starting_addr <= sizeof((*(inst->p_cfg)).p_discrete_inputs)/4)
	{
		if((read_discrete_inputs.starting_addr + read_discrete_inputs.quantity) <= sizeof((*(inst->p_cfg)).p_discrete_inputs)/4)
		{
			if ((read_discrete_inputs.quantity % 8) > 0)	
			{
				byte_nr = (read_discrete_inputs.quantity / 8) + 1;
			}
			else											
			{
				byte_nr = (read_discrete_inputs.quantity / 8);
			}
			brsmemset((UDINT)inst->internal.send_buff, 0, sizeof(inst->internal.send_buff));
			// Transaction id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[0], (UDINT)&inst->internal.receive_buff[master].byte[0], sizeof(uint_var));
			// Protocol id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[2], (UDINT)&inst->internal.receive_buff[master].byte[2], sizeof(uint_var));
			// Length: 2 byte 
			length = 3 + byte_nr;
			uint_var = MySwapUINT(length);
			brsmemcpy((UDINT)&inst->internal.send_buff[4], (UDINT)&uint_var, sizeof(uint_var));
			// Unit: 1 byte 
			inst->internal.send_buff[6] = inst->internal.receive_buff[master].byte[6];
			// Function code: 1 byte 
			inst->internal.send_buff[7] = inst->internal.receive_buff[master].byte[7];
			// Byte nr: 1 byte 
			inst->internal.send_buff[8] = byte_nr;
			for(i = 0; i < read_discrete_inputs.quantity; i++)
			{
				if(((*(inst->p_cfg)).p_discrete_inputs[read_discrete_inputs.starting_addr+i]) != 0)
				{
					if((*((BOOL*)((*(inst->p_cfg)).p_discrete_inputs[read_discrete_inputs.starting_addr+i]))))
					{
						inst->internal.send_buff[9 + (i/8)] = inst->internal.send_buff[9 + (i/8)] | (USINT)(0x01 << (i%8));
					}
					else
					{
						inst->internal.send_buff[9 + (i/8)] = inst->internal.send_buff[9 + (i/8)] & (USINT)~(0x01 << (i%8));
					}
				}
				else // Variable address is empty 
				{
					NewLogEntry("ERR: Empty variable ADR", LOGBOOK);
					return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_ADR_EMPTY);
				}			
			}
			MessageLog (TYP_RESPONSE, READ_DISCRETE_INPUTS, read_discrete_inputs.starting_addr, read_discrete_inputs.quantity, LOGBOOK);
			return length + 6;
		}
		else // Length exceeds maximum range 
		{
			NewLogEntry("ERR: Length exceeds max", LOGBOOK);
			return create_error_message(inst, master, ERROR_MOD_ADR, ERROR_LENGTH_EXCEEDED);
		}			
	}
	else // Address exceed maximum range 
	{
		NewLogEntry("ERR: Address exceeds max", LOGBOOK);
		return create_error_message(inst, master, ERROR_MOD_ADR, ERROR_ADR_EXCEEDED);
	}

	return 0;
}

UINT read_holding_registers(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK)
{
	t_read_request	read_holding_registers;
	UINT			i;
	UINT			byte_nr;
	UINT			uint_var;
	INT				int_var;
	UINT			length;

	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[8]), sizeof(uint_var));
	read_holding_registers.starting_addr = MySwapUINT(uint_var);
	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[10]), sizeof(uint_var));
	read_holding_registers.quantity = MySwapUINT(uint_var);
	
	MessageLog (TYP_REQUEST, READ_HOLDING_REGISTERS, read_holding_registers.starting_addr, read_holding_registers.quantity, LOGBOOK);
	
	if( read_holding_registers.starting_addr <= sizeof((*(inst->p_cfg)).p_holding_registers)/4)
	{
		if((read_holding_registers.starting_addr + read_holding_registers.quantity) <= sizeof((*(inst->p_cfg)).p_holding_registers)/4)
		{
			byte_nr = read_holding_registers.quantity * 2;
			brsmemset((UDINT)inst->internal.send_buff, 0, sizeof(inst->internal.send_buff));
			// Transaction id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[0], (UDINT)&inst->internal.receive_buff[master].byte[0], sizeof(uint_var));
			// Protocol id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[2], (UDINT)&inst->internal.receive_buff[master].byte[2], sizeof(uint_var));
			// Length: 2 byte 
			length = 3 + byte_nr;
			uint_var = MySwapUINT(length);
			brsmemcpy((UDINT)&inst->internal.send_buff[4], (UDINT)&uint_var, sizeof(uint_var));
			// Unit: 1 byte 
			inst->internal.send_buff[6] = inst->internal.receive_buff[master].byte[6];
			// Function code: 1 byte 
			inst->internal.send_buff[7] = inst->internal.receive_buff[master].byte[7];
			// Byte nr: 1 byte 
			inst->internal.send_buff[8] = byte_nr;
			// Dati: x byte 
			for(i = 0; i < read_holding_registers.quantity; i++)
			{
				if((*(inst->p_cfg)).p_holding_registers[read_holding_registers.starting_addr + i])
				{
					int_var = MySwapINT(*((INT*)((*(inst->p_cfg)).p_holding_registers[read_holding_registers.starting_addr + i])));
					brsmemcpy((UDINT)&inst->internal.send_buff[9 + (i*2)], (UDINT)&int_var, sizeof(int_var));
				}
				else // Variable address is empty 
				{
					NewLogEntry("ERR: Empty variable ADR", LOGBOOK);
					return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_ADR_EMPTY);	
				}
			}
			MessageLog (TYP_RESPONSE, READ_HOLDING_REGISTERS, read_holding_registers.starting_addr, read_holding_registers.quantity, LOGBOOK);
			return length + 6;
		}
		else // Length exceeds maximum range 
		{
			NewLogEntry("ERR: Length exceeds max", LOGBOOK);
			return create_error_message(inst, master, ERROR_MOD_ADR, ERROR_LENGTH_EXCEEDED);
		}			
	}
	else // Address exceed maximum range 
	{
		NewLogEntry("ERR: Address exceeds max", LOGBOOK);
		return create_error_message(inst, master, ERROR_MOD_ADR, ERROR_ADR_EXCEEDED);
	}

	return 0;
}

UINT read_input_registers(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK)
{
	t_read_request	read_input_registers;
	UINT			i;
	UINT			byte_nr;
	UINT			uint_var;
	INT				int_var;
	UINT			length;

	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[8]), sizeof(uint_var));
	read_input_registers.starting_addr = MySwapUINT(uint_var);
	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[10]), sizeof(uint_var));
	read_input_registers.quantity = MySwapUINT(uint_var);
	
	MessageLog (TYP_REQUEST, READ_INPUT_REGISTERS, read_input_registers.starting_addr, read_input_registers.quantity, LOGBOOK);
	
	if( read_input_registers.starting_addr <= sizeof((*(inst->p_cfg)).p_input_registers)/4)
	{
		if((read_input_registers.starting_addr + read_input_registers.quantity) <= sizeof((*(inst->p_cfg)).p_input_registers)/4)
		{
			byte_nr = read_input_registers.quantity * 2;
			brsmemset((UDINT)inst->internal.send_buff, 0, sizeof(inst->internal.send_buff));
			// Transaction id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[0], (UDINT)&inst->internal.receive_buff[master].byte[0], sizeof(uint_var));
			// Protocol id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[2], (UDINT)&inst->internal.receive_buff[master].byte[2], sizeof(uint_var));
			// Length: 2 byte 
			length = 3 + byte_nr;
			uint_var = MySwapUINT(length);
			brsmemcpy((UDINT)&inst->internal.send_buff[4], (UDINT)&uint_var, sizeof(uint_var));
			// Unit: 1 byte 
			inst->internal.send_buff[6] = inst->internal.receive_buff[master].byte[6];
			// Function code: 1 byte 
			inst->internal.send_buff[7] = inst->internal.receive_buff[master].byte[7];
			// Byte nr: 1 byte 
			inst->internal.send_buff[8] = byte_nr;
			// Dati: x byte 
			for(i = 0; i < read_input_registers.quantity; i++)
			{
				if((*(inst->p_cfg)).p_input_registers[read_input_registers.starting_addr + i])
				{
					int_var = MySwapINT(*((INT*)((*(inst->p_cfg)).p_input_registers[read_input_registers.starting_addr + i])));
					brsmemcpy((UDINT)&inst->internal.send_buff[9 + (i*2)], (UDINT)&int_var, sizeof(int_var));
				}
				else // Variable address is empty 
				{
					NewLogEntry("ERR: Empty variable ADR", LOGBOOK);
					return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_ADR_EMPTY);
				}
			}
			MessageLog (TYP_RESPONSE, READ_INPUT_REGISTERS, read_input_registers.starting_addr, read_input_registers.quantity, LOGBOOK);
			return length + 6;
		}
		else // Lenghth exceeds maximum range 
		{
			NewLogEntry("ERR: Length exceeds max", LOGBOOK);
			return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_LENGTH_EXCEEDED);
		}			
	}
	else // Address exceed maximum range 
	{
		NewLogEntry("ERR: Address exceeds max", LOGBOOK);
		return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_ADR_EXCEEDED);
	}

	return 0;
}

UINT write_single_coil(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK)
{
	t_single_write_request	write_single_coil;
	UINT					uint_var;
	UINT					length;

	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[8]), sizeof(uint_var));
	write_single_coil.starting_addr = MySwapUINT(uint_var);
	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[10]), sizeof(uint_var));
	write_single_coil.value = MySwapUINT(uint_var);
	
	MessageLog (TYP_REQUEST, WRITE_SINGLE_COIL, write_single_coil.starting_addr, 1, LOGBOOK);
	
	if((write_single_coil.value == ON) || (write_single_coil.value == OFF))
	{
		if((write_single_coil.starting_addr <= sizeof((*(inst->p_cfg)).p_coils)/4))
		{
			if(((*(inst->p_cfg)).p_coils[write_single_coil.starting_addr]) != 0)
			{
				if(write_single_coil.value == ON)
				{
					*((BOOL*)((*(inst->p_cfg)).p_coils[write_single_coil.starting_addr])) = 1;
				}
				else
				{
					*((BOOL*)((*(inst->p_cfg)).p_coils[write_single_coil.starting_addr])) = 0;
				}

				brsmemset((UDINT)inst->internal.send_buff, 0, sizeof(inst->internal.send_buff));
				// Transaction id: 2 byte 
				brsmemcpy((UDINT)&inst->internal.send_buff[0], (UDINT)&inst->internal.receive_buff[master].byte[0], sizeof(uint_var));
				// Protocol id: 2 byte 
				brsmemcpy((UDINT)&inst->internal.send_buff[2], (UDINT)&inst->internal.receive_buff[master].byte[2], sizeof(uint_var));
				// Length: 2 byte 
				length = 6;
				uint_var = MySwapUINT(length);
				brsmemcpy((UDINT)&inst->internal.send_buff[4], (UDINT)&uint_var, sizeof(uint_var));
				// Unit: 1 byte 
				inst->internal.send_buff[6] = inst->internal.receive_buff[master].byte[6];
				// Function code: 1 byte 
				inst->internal.send_buff[7] = inst->internal.receive_buff[master].byte[7];
				// Starting address: 2 byte 
				brsmemcpy((UDINT)&inst->internal.send_buff[8], (UDINT)&inst->internal.receive_buff[master].byte[8], 2);
				// Value: 2 byte 
				brsmemcpy((UDINT)&inst->internal.send_buff[10], (UDINT)&inst->internal.receive_buff[master].byte[10], 2);
			
				MessageLog (TYP_RESPONSE, WRITE_SINGLE_COIL, write_single_coil.starting_addr, write_single_coil.value, LOGBOOK);
				return 12;
			}
			else // Variable address is empty 
			{
				NewLogEntry("ERR: Empty variable ADR", LOGBOOK);
				return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_ADR_EMPTY);
			}			
		}
		else // Address exceeds maximum range 
		{
			NewLogEntry("ERR: Address exceeds max", LOGBOOK);
			return create_error_message(inst, master, ERROR_MOD_ADR, ERROR_ADR_EXCEEDED);
		}
	}
	else // Address+Lenghth exceed maximum range 
	{
		NewLogEntry("ERR: Not boolean value", LOGBOOK);
		return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_LENGTH_EXCEEDED);
	}

	return 0;
}

UINT write_single_register(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK)
{
	t_single_write_request	write_single_register;
	INT						int_var;
	UINT					uint_var;
	UINT					length;

	brsmemcpy((UDINT)&uint_var,(UDINT)&(inst->internal.receive_buff[master].byte[8]), sizeof(uint_var));
	write_single_register.starting_addr = MySwapUINT(uint_var);
	brsmemcpy((UDINT)&int_var, (UDINT)&(inst->internal.receive_buff[master].byte[10]), sizeof(int_var));
	write_single_register.value = MySwapUINT(int_var);
	
	MessageLog (TYP_REQUEST, WRITE_SINGLE_REGISTER, write_single_register.starting_addr, 1, LOGBOOK);
	
	if((write_single_register.starting_addr <= sizeof((*(inst->p_cfg)).p_holding_registers)/4))
	{
		if(((*(inst->p_cfg)).p_holding_registers[write_single_register.starting_addr]) != 0)
		{
			*((INT*)((*(inst->p_cfg)).p_holding_registers[write_single_register.starting_addr])) = write_single_register.value;

			brsmemset((UDINT)inst->internal.send_buff, 0, sizeof(inst->internal.send_buff));
			// Transaction id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[0], (UDINT)&inst->internal.receive_buff[master].byte[0], sizeof(uint_var));
			// Protocol id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[2], (UDINT)&inst->internal.receive_buff[master].byte[2], sizeof(uint_var));
			// Length: 2 byte 
			length = 6;
			uint_var = MySwapUINT(length);
			brsmemcpy((UDINT)&inst->internal.send_buff[4], (UDINT)&uint_var, sizeof(uint_var));
			// Unit: 1 byte 
			inst->internal.send_buff[6] = inst->internal.receive_buff[master].byte[6];
			// Function code: 1 byte 
			inst->internal.send_buff[7] = inst->internal.receive_buff[master].byte[7];
			// Starting address: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[8], (UDINT)&inst->internal.receive_buff[master].byte[8], 2);
			// Value: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[10], (UDINT)&inst->internal.receive_buff[master].byte[10], 2);
		
			MessageLog (TYP_RESPONSE, WRITE_SINGLE_REGISTER, write_single_register.starting_addr, write_single_register.value, LOGBOOK);
			return 12;
		}
		else // Variable address is empty 
		{
			NewLogEntry("ERR: Empty variable ADR", LOGBOOK);
			return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_ADR_EMPTY);
		}			
	}
	else // Address exceeds maximum range 
	{
		NewLogEntry("ERR: Address exceeds max", LOGBOOK);
		return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_ADR_EXCEEDED);
	}
	return 0;
}

UINT write_multiple_coils(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK)
{
	t_multiple_write_request	write_multiple_coil;
	UINT						uint_var;
	UINT						length;
	UINT						i;

	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[8]), sizeof(uint_var));
	write_multiple_coil.starting_addr = MySwapUINT(uint_var);
	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[10]), sizeof(uint_var));
	write_multiple_coil.quantity = MySwapUINT(uint_var);
	write_multiple_coil.byte_nr = inst->internal.receive_buff[master].byte[12];
	
	MessageLog (TYP_REQUEST, WRITE_MULTIPLE_COILS, write_multiple_coil.starting_addr, write_multiple_coil.quantity, LOGBOOK);
	
	if(write_multiple_coil.starting_addr <= sizeof((*(inst->p_cfg)).p_coils)/4)
	{
		if((write_multiple_coil.starting_addr + write_multiple_coil.quantity) <= sizeof((*(inst->p_cfg)).p_coils)/4)
		{
			// Writing to registers
			for(i=0; i < write_multiple_coil.quantity; i++)
			{
				if(((*(inst->p_cfg)).p_coils[write_multiple_coil.starting_addr+i]) != 0)
				{
					if ((inst->internal.receive_buff[master].byte[13 + (i/8)] & (USINT)(0x01 << (i%8))))
					{
						*((BOOL*)((*(inst->p_cfg)).p_coils[write_multiple_coil.starting_addr+i])) = 1;
					}
					else
					{
						*((BOOL*)((*(inst->p_cfg)).p_coils[write_multiple_coil.starting_addr+i])) = 0;
					}
				}
				else // Variable address is empty 
				{
					NewLogEntry("ERR: Empty variable ADR", LOGBOOK);
					return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_ADR_EMPTY);
				}	
			}

			// Generation of the answer message 
			brsmemset((UDINT)inst->internal.send_buff, 0, sizeof(inst->internal.send_buff));
			// Transaction id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[0], (UDINT)&inst->internal.receive_buff[master].byte[0], sizeof(uint_var));
			// Protocol id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[2], (UDINT)&inst->internal.receive_buff[master].byte[2], sizeof(uint_var));
			// Length: 2 byte 
			length = 6;
			uint_var = MySwapUINT(length);
			brsmemcpy((UDINT)&inst->internal.send_buff[4], (UDINT)&uint_var, sizeof(uint_var));
			// Unit: 1 byte 
			inst->internal.send_buff[6] = inst->internal.receive_buff[master].byte[6];
			// Function code: 1 byte 
			inst->internal.send_buff[7] = inst->internal.receive_buff[master].byte[7];
			// Starting address: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[8], (UDINT)&inst->internal.receive_buff[master].byte[8], 2);
			// Value: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[10], (UDINT)&inst->internal.receive_buff[master].byte[10], 2);
			
			MessageLog (TYP_RESPONSE, WRITE_MULTIPLE_COILS, write_multiple_coil.starting_addr, write_multiple_coil.quantity, LOGBOOK);
			return 12;
		}
		else // Length exceeds maximum range 
		{
			NewLogEntry("ERR: Length exceeds max", LOGBOOK);
			return create_error_message(inst, master, ERROR_MOD_ADR, ERROR_LENGTH_EXCEEDED);
		}			
	}
	else // Address exceed maximum range 
	{
		NewLogEntry("ERR: Address exceeds max", LOGBOOK);
		return create_error_message(inst, master, ERROR_MOD_ADR, ERROR_ADR_EXCEEDED);
	}
	return 0;
}

UINT write_multiple_registers(struct MBslave* inst, USINT master, modbus_log_typ LOGBOOK)
{
	t_multiple_write_request	write_multiple_register;
	INT							int_var;
	UINT						uint_var;
	UINT						length;
	UINT						i;

	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[8]), sizeof(uint_var));
	write_multiple_register.starting_addr = MySwapUINT(uint_var);
	brsmemcpy((UDINT)&uint_var, (UDINT)&(inst->internal.receive_buff[master].byte[10]), sizeof(uint_var));
	write_multiple_register.quantity = MySwapUINT(uint_var);
	write_multiple_register.byte_nr = inst->internal.receive_buff[master].byte[12];
	
	MessageLog (TYP_REQUEST, WRITE_MULTIPLE_REGISTERS, write_multiple_register.starting_addr, write_multiple_register.quantity, LOGBOOK);
	
	if(write_multiple_register.starting_addr <= sizeof((*(inst->p_cfg)).p_holding_registers)/4)
	{
		if((write_multiple_register.starting_addr + write_multiple_register.quantity) <= sizeof((*(inst->p_cfg)).p_holding_registers)/4)
		{
			// Writing to registers 
			for(i=0; i < write_multiple_register.quantity; i++)
			{
				if ((*(inst->p_cfg)).p_holding_registers[write_multiple_register.starting_addr+i])
				{
					brsmemcpy((UDINT)&int_var, (UDINT)&inst->internal.receive_buff[master].byte[13+(i*2)], sizeof(int_var));
					*((INT*)((*(inst->p_cfg)).p_holding_registers[write_multiple_register.starting_addr+i])) = MySwapINT(int_var);
				}
				else // Variable address is empty 
				{
					NewLogEntry("ERR: Empty variable ADR", LOGBOOK);
					return create_error_message(inst, master, ERROR_MOD_VALUE, ERROR_ADR_EMPTY);
				}
			}

			// Generation of the answer message 
			brsmemset((UDINT)inst->internal.send_buff, 0, sizeof(inst->internal.send_buff));
			// Transaction id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[0], (UDINT)&inst->internal.receive_buff[master].byte[0], sizeof(uint_var));
			// Protocol id: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[2], (UDINT)&inst->internal.receive_buff[master].byte[2], sizeof(uint_var));
			// Length: 2 byte 
			length = 6;
			uint_var = MySwapUINT(length);
			brsmemcpy((UDINT)&inst->internal.send_buff[4], (UDINT)&uint_var, sizeof(uint_var));
			// Unit: 1 byte 
			inst->internal.send_buff[6] = inst->internal.receive_buff[master].byte[6];
			// Function code: 1 byte 
			inst->internal.send_buff[7] = inst->internal.receive_buff[master].byte[7];
			// Starting address: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[8], (UDINT)&inst->internal.receive_buff[master].byte[8], 2);
			// Value: 2 byte 
			brsmemcpy((UDINT)&inst->internal.send_buff[10], (UDINT)&inst->internal.receive_buff[master].byte[10], 2);
			
			MessageLog (TYP_RESPONSE, WRITE_MULTIPLE_REGISTERS, write_multiple_register.starting_addr, write_multiple_register.quantity, LOGBOOK);
			return 12;
		}
		else // Lenghth exceeds maximum range 
		{
			NewLogEntry("ERR: Length exceeds max", LOGBOOK);
			return create_error_message(inst, master, ERROR_MOD_ADR, ERROR_ADR_EXCEEDED);
		}			
	}
	else // Address exceed maximum range 
	{
		NewLogEntry("ERR: Address exceeds max", LOGBOOK);
		return create_error_message(inst, master, ERROR_MOD_ADR, ERROR_LENGTH_EXCEEDED);
	}
	return 0;
}
