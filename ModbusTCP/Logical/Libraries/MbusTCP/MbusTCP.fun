
FUNCTION_BLOCK MBslave (*ModbusTCP slave implementation*)
	VAR_INPUT
		enable : BOOL; (*Enable execution*)
		device : UDINT; (*Ethernet device string*)
		port : UINT; (*Port number of the slave (default 502)*)
		p_cfg : REFERENCE TO modbus_slave_cfg_typ; (*Pointer to data*)
		log : modbus_log_typ; (*Log of FUB runtime actions*)
		master_timeout : UDINT; (*Master has to send at least one request within this time (ms).  (0 to disable)*)
	END_VAR
	VAR_OUTPUT
		my_ip : STRING[20]; (*Device IP address*)
		status : UINT; (*Execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
		last_error : UINT; (*Error number of data request from the client*)
	END_VAR
	VAR
		internal : modbus_slave_internal_typ; (*Internal variables and FUBs*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK MBmaster (*ModbusTCP master implementation*)
	VAR_INPUT
		enable : BOOL; (*Enable execution*)
		device : UDINT; (*Ethernet device string*)
		port : UINT; (*Port number of the slave (default 502)*)
		slave_ip_addr : STRING[18]; (*IP address of the slave*)
		p_cfg : REFERENCE TO modbus_master_cfg_typ; (*Pointer to data*)
		log : modbus_log_typ; (*Log of FUB runtime actions*)
		cycle_time : UDINT; (*Task class cycle time in ms*)
	END_VAR
	VAR_OUTPUT
		last_error : UINT; (*Error number of data response from the slave*)
		my_ip : STRING[20]; (*Device IP address*)
		status : UINT; (*Execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR
	VAR
		internal : modbus_master_internal_typ; (*Internal variables and FUBs*)
	END_VAR
END_FUNCTION_BLOCK
