/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Library: MbusTCP
 * File: MBdeclarations.h
 * Author: strickers
 * Created: October 29, 2009
 *******************************************************************/

/* ------------------------------------------------------------------------------------------------- */
/* Includes	 */
/* ------------------------------------------------------------------------------------------------- */
#include <mbustcp.h>
#include <astcp.h>
#include <asarcfg.h>
#include <asieccon.h>
#include <AsBrStr.h>
#include <sys_lib.h>
#include <bur/plctypes.h>

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

#define	RECEIVE_TIMEOUT				5000	/* The slave has to responde within this time (ms) */
#define	DEBUG_LEVEL					2		/* 1=Report only errors, 2=Report errors, requests, response */
#define LOG_LINE_LEN				50		/* Length of loogbook line */

/* ------------------------------------------------------------------------------------------------- */
/* STEP SEQUENCE */
/* ------------------------------------------------------------------------------------------------- */
#define INIT						0
#define GET_IP						10
#define WAIT_SERVER					20
#define OPEN_PORT					30
#define SET_LINGER					40
#define SET_NODELAY					41
#define WAIT_CONNECTION				50
#define WAIT_REQUEST				60
#define SEND_ANSWER					70
#define CREATE_REQUEST				80
#define SEND_REQUEST				81
#define	WAIT_ANSWER					90
#define	CLOSE_PORT					100
#define	WAIT						110
#define	ERROR						120

/* ------------------------------------------------------------------------------------------------- */
/* STEP SUB SEQUENCE */
/* ------------------------------------------------------------------------------------------------- */
#define SUB_WAIT					0
#define SUB_SEND					10
#define SUB_CLOSE_MASTER			20

/* ------------------------------------------------------------------------------------------------- */
/* MODBUS FUNCTION */
/* ------------------------------------------------------------------------------------------------- */
#define READ_COILS					0x01
#define READ_DISCRETE_INPUTS		0x02
#define READ_HOLDING_REGISTERS		0x03
#define READ_INPUT_REGISTERS		0x04
#define WRITE_SINGLE_COIL			0x05
#define WRITE_SINGLE_REGISTER		0x06
#define WRITE_MULTIPLE_COILS		0x0F
#define WRITE_MULTIPLE_REGISTERS	0x10
#define	ON							0xFF00
#define OFF							0x0000

/* ------------------------------------------------------------------------------------------------- */
/* ERROR */
/* ------------------------------------------------------------------------------------------------- */
#define	ERROR_CRITICAL				50000	/* COM: Critical error, disable and re-enable FUB */
#define	ERROR_MESSAGE_SIZE			50001	/* COM: Frame message is to short */
#define	ERROR_MESSAGE_FORMAT		50002	/* COM: Message format is incorrect */
#define	ERROR_REC_BUF_EXCEEDED		50003	/* COM: Receive buffer exceeded maximum configured size */
#define	ERROR_ADR_EMPTY				50004	/* COM: Variable address is empty */
#define	ERROR_CYCLIC_ZERO			50005	/* COM: Value cyclic time is 0 */
#define	ERROR_QUANTITY				50006	/* COM: Quantity is higher than the Modbus limit (125 for analog/2000 for digital) */
#define	ERROR_TOO_MANY_MASTERS		50100	/* SLV: More than MAX_MASTER are trying to connect */
#define	ERROR_ADR_EXCEEDED			50101	/* SLV: Address exceeded maximum configured size */
#define	ERROR_LENGTH_EXCEEDED		50102	/* SLV: Address+Size exceeded maximum configured size or modbus limit (125/2000) */
#define	ERROR_FUNCTION_CODE			50103	/* SLV: Function code is not supported */
#define	ERROR_MASTER_TIMED_OUT		50104	/* SLV: The master did not send a request for a certain time */
#define	ERROR_CFG_MASTERS			50110	/* SLV: Configuration error in num masters */
#define ERROR_BAD_SLAVE_DATA		50200	/* MAS: Bad data received from slave */
#define ERROR_SLAVE_DATA_TOO_SHORT	50201	/* MAS: Data received from slave too short */
#define ERROR_SLAVE_DATA_TOO_LONG	50202	/* MAS: Data received from slave too long */
#define ERROR_TIMEOUT_REQ			50210	/* MAS: A slave request timed out */
#define ERROR_TIMEOUT_CONNECT		50211	/* MAS: Connection request timed out */
#define ERROR_DEVICE_ERR			50300	/* MAS: Invalid Ethernet device */

#define ERROR_MOD_FUNCTION			0x01	/* Modbus error: wrong function code */
#define ERROR_MOD_ADR				0x02	/* Modbus error: wrong address */
#define ERROR_MOD_VALUE				0x03	/* Modbus error: wrong value */

/* ------------------------------------------------------------------------------------------------- */
/* LOG BOOK MESSAGES */
/* ------------------------------------------------------------------------------------------------- */
#define TYP_REQUEST 				0
#define TYP_RESPONSE 				1		

/* ------------------------------------------------------------------------------------------------- */
/* OTHER CONSTANTS */
/* ------------------------------------------------------------------------------------------------- */
#define	MODBUS_TCP_PORT				502		/* Port to use for ModbusTCP communication */
#define	MODBUS_MAX_REG				125		/* Maximum number of modbus registers for one request */
#define DISABLE						65534
#define	BUSY						65535

/* ------------------------------------------------------------------------------------------------- */
/* Structures */
/* ------------------------------------------------------------------------------------------------- */
typedef struct
{
	UINT	transaction_id;
	UINT	protocol_id;
	UINT	length;
	USINT	unit;
	USINT	function_code;
	UINT	quantity;
}t_master_request;

typedef struct
{
	UINT	starting_addr;
	UINT	quantity;
} t_read_request;

typedef struct
{
	UINT	starting_addr;
	UINT	value;
} t_single_write_request;

typedef struct
{
	UINT	starting_addr;
	UINT	quantity;
	UINT	byte_nr;
} t_multiple_write_request;

/* ------------------------------------------------------------------------------------------------- */
/* Global functions */
/* ------------------------------------------------------------------------------------------------- */
void NewLogEntry(STRING message[50], modbus_log_typ LOGBOOK);
void MessageLog (USINT type, USINT function_code, UINT start_addr, UINT length, modbus_log_typ LOGBOOK);

UINT 	MySwapUINT(UINT data);
INT 	MySwapINT(INT data);

