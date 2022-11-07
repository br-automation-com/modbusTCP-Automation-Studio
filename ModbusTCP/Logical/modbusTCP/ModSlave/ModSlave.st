PROGRAM _INIT
	brsstrcpy(ADR(interface), ADR('IF5'));					 // Ethernet interface used on this PLC 

	// --------------------------------------------------------------------
	//  Reset function blocks and data structure on startup (SGC)
	// --------------------------------------------------------------------- 
	brsmemset(ADR(ModbusTcpSlave), 0, SIZEOF(ModbusTcpSlave));
	brsmemset(ADR(SlaveConfig), 0, SIZEOF(SlaveConfig));
	brsmemset(ADR(logger), 0, SIZEOF(logger));

	// --------------------------------------------------------------------
	//  Create default IO mapping FOR digital AND analog area
	// --------------------------------------------------------------------- 
	FOR idx:=0 TO SIZEOF(DO_Bool)-1 DO
		SlaveConfig.p_coils[idx] := ADR(DO_Bool[idx]);
	END_FOR;
	FOR idx:=0 TO SIZEOF(DI_Bool)-1 DO
		SlaveConfig.p_discrete_inputs[idx]	:= ADR(DI_Bool[idx]);
	END_FOR;
	FOR idx:=0 TO SIZEOF(AO_Word)/SIZEOF(AO_Word[0])-1 DO
		SlaveConfig.p_holding_registers[idx] := ADR(AO_Word[idx]);
	END_FOR;
	FOR idx:=0 TO SIZEOF(AI_Word)/SIZEOF(AI_Word[0])-1 DO
		SlaveConfig.p_input_registers[idx]	:= ADR(AI_Word[idx]);
	END_FOR;
	
	// --------------------------------------------------------------------
	//  Configure FUNCTION block instance
	// --------------------------------------------------------------------- 
	ModbusTcpSlave.enable			:= 1; 					// Enable function block 
	ModbusTcpSlave.device			:= ADR(interface);		// Ethernet device string 
	ModbusTcpSlave.p_cfg			:= ADR(SlaveConfig); 	// IO configuration 
	ModbusTcpSlave.p_log 			:= ADR(logger);			// Pointer to logger string array. The logger information will show communication details. Enable for debugging, disable for production to save system resources.
	ModbusTcpSlave.master_timeout 	:= 60000; 				// Disconnect master after x miliseconds when no request was sent, 0 = disabled 

	StartStop := 1;
END_PROGRAM

PROGRAM _CYCLIC
	// --------------------------------------------------------------------
	// Cyclic function call
	// --------------------------------------------------------------------- 
	IF(StartStop) THEN
		ModbusTcpSlave();
	END_IF

	// --------------------------------------------------------------------
	// Reset last error information
	// --------------------------------------------------------------------- 
	IF(reset_error) THEN
		brsmemset(ADR(ModbusTcpSlave.last_error), 0, 2);
		reset_error := 0;
	END_IF
END_PROGRAM
