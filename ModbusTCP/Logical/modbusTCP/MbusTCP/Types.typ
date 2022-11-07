
TYPE
	step_enum : 
		( (*local state machine*)
		INIT,
		GET_IP,
		WAIT_SERVER,
		OPEN_PORT,
		SET_LINGER,
		SET_NODELAY,
		WAIT_CONNECTION,
		WAIT_REQUEST,
		SEND_ANSWER,
		CREATE_REQUEST,
		SEND_REQUEST,
		WAIT_ANSWER,
		CLOSE_PORT,
		WAIT,
		ERROR
		);
	step_sub_enum : 
		(
		SUB_WAIT,
		SUB_SEND,
		SUB_CLOSE_MASTER
		);
	receive_buff_typ : 	STRUCT  (*Receive data buffer*)
		byte : ARRAY[0..500]OF USINT; (*Received data*)
	END_STRUCT;
	t_master_request : 	STRUCT 
		transaction_id : UINT;
		protocol_id : UINT;
		length : UINT;
		unit : USINT;
		function_code : USINT;
		quantity : UINT;
	END_STRUCT;
	t_read_request : 	STRUCT 
		starting_addr : UINT;
		quantity : UINT;
	END_STRUCT;
	t_single_write_request : 	STRUCT 
		starting_addr : UINT;
		value : UINT;
	END_STRUCT;
	t_multiple_write_request : 	STRUCT 
		starting_addr : UINT;
		quantity : UINT;
		byte_nr : UINT;
	END_STRUCT;
	master_info_typ : 	STRUCT 
		ident : UDINT;
		port : UINT;
		ip_addr : ARRAY[0..17]OF USINT; (*Client IP address*)
		timeout_timer : TON; (*Timer to determine when connection times out*)
		disconnect : BOOL; (*Flag to disconnect this client if it times out.*)
	END_STRUCT;
	modbus_master_action_enable_typ : 	STRUCT 
		cyclic : BOOL;
		single : BOOL;
	END_STRUCT;
	modbus_master_action_param_typ : 	STRUCT 
		unit : USINT;
		type : USINT;
		start_addr : UINT;
		quantity : UINT;
		p_pv : UDINT;
		timer : UDINT;
	END_STRUCT;
	modbus_master_cfg_typ : 	STRUCT 
		action_enable : ARRAY[0..19]OF modbus_master_action_enable_typ;
		action_param : ARRAY[0..19]OF modbus_master_action_param_typ;
	END_STRUCT;
	modbus_master_internal_typ : 	STRUCT  (*Internal master variables*)
		step : UINT; (*Internal state in a state machine*)
		cycle_time : UINT;
		time_old : TIME;
		tcp_open : TcpOpen; (*TcpOpen FUB from AsTCP*)
		tcp_ioctl : TcpIoctl; (*TcpIoctl FUB from AsTCP*)
		tcp_client : TcpClient; (*TcpClient FUB from AsTCP*)
		tcp_send : TcpSend; (*TcpSend FUB from AsTCP*)
		tcp_receive : TcpRecv; (*TcpRecv FUB from AsTCP*)
		tcp_close : TcpClose; (*TcpClose FUB from AsTCP*)
		tcp_ip : CfgGetIPAddr;
		linger_opt : tcpLINGER_typ; (*TcpIoctl linger options*)
		last_req : USINT; (*Last request*)
		receive_timer : UDINT; (*Receive timer*)
		send_timer : ARRAY[0..19]OF UDINT; (*Send timer*)
		send_buff : ARRAY[0..500]OF USINT; (*Send buffer*)
		receive_buff : ARRAY[0..500]OF USINT; (*Receive buffer*)
		transaction_id : UINT; (*Transaction ID*)
		wrong_message : UDINT; (*Wrong message counter*)
	END_STRUCT;
	modbus_slave_cfg_typ : 	STRUCT 
		p_discrete_inputs : ARRAY[0..255]OF UDINT;
		p_coils : ARRAY[0..255]OF UDINT;
		p_input_registers : ARRAY[0..255]OF UDINT;
		p_holding_registers : ARRAY[0..255]OF UDINT;
	END_STRUCT;
	modbus_slave_internal_typ : 	STRUCT  (*Internal slave variables*)
		step : UINT; (*Internal state in a state machine*)
		substep : step_sub_enum; (*Internal substate in a state machine*)
		i : USINT; (*Loop counter*)
		master_nr : USINT; (*Client ID*)
		master_last : USINT;
		master_info : ARRAY[0..2]OF master_info_typ; (*Client info*)
		master_addr : ARRAY[0..17]OF USINT; (*Client IP address*)
		tcp_open : TcpOpen; (*TcpOpen FUB from AsTCP*)
		tcp_ioctl : TcpIoctl; (*TcpIoctl FUB from AsTCP*)
		tcp_server : TcpServer; (*TcpServer FUB from AsTCP*)
		tcp_send : TcpSend; (*TcpSend FUB from AsTCP*)
		tcp_receive : ARRAY[0..2]OF TcpRecv; (*TcpRecv FUB from AsTCP*)
		tcp_close : TcpClose; (*TcpClose FUB from AsTCP*)
		tcp_ip : CfgGetIPAddr;
		linger_opt : tcpLINGER_typ; (*TcpIoctl linger options*)
		send_buff : ARRAY[0..500]OF USINT; (*Send buffer*)
		send_byte : UINT;
		receive_buff : ARRAY[0..2]OF receive_buff_typ; (*Receive buffers for clients*)
	END_STRUCT;
END_TYPE
