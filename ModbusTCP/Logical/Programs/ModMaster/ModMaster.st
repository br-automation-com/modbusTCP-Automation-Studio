PROGRAM _INIT
	brsstrcpy(ADR(ip), ADR('192.168.86.193'));				 (* Slave device IP address *)
	brsstrcpy(ADR(interface), ADR('IF5'));					 (* Ethernet interface used on this PLC *)
	
	(* --------------------------------------------------------------------
	  Reset FUNCTION blocks AND data structure on startup (SGC)
	  --------------------------------------------------------------------- *)
    brsmemset(ADR(ModbusTcpMaster), 0, SIZEOF(ModbusTcpMaster));
    brsmemset(ADR(MasterConfig), 0, SIZEOF(MasterConfig));
    brsmemset(ADR(logger), 0, SIZEOF(logger));

	(* --------------------------------------------------------------------
	  Sample for reading/writing data from/to a modbusTCP slave
	  --------------------------------------------------------------------- *)
	MasterConfig.action_enable[0].cyclic 	:= 1;			(* Read data cyclic *)
	MasterConfig.action_param[0].p_pv		:= ADR(D_In);	(* Store data in this variable *)
	MasterConfig.action_param[0].quantity 	:= 20;			(* Number of items to read *)
	MasterConfig.action_param[0].unit 		:= 1;			(* Unit address - ignore if not specified *)
	MasterConfig.action_param[0].start_addr := 16#0;		(* Read from this modbus address *)
	MasterConfig.action_param[0].timer 		:= 500;			(* Refresh timer for this item *)
	MasterConfig.action_param[0].type 		:= 2;			(* Function code for this item (2 := read discrete inputs) *)

	MasterConfig.action_enable[1].cyclic 	:= 1;			(* Write data cyclic *)
	MasterConfig.action_param[1].p_pv		:= ADR(D_Out); 	(* Take data from this address *)
	MasterConfig.action_param[1].quantity 	:= 20; 			(* Number of items to write *)
	MasterConfig.action_param[1].unit 		:= 1;			(* Unit address - ignore if not specified *)
	MasterConfig.action_param[1].start_addr := 16#0;		(* Write to this modbus address *)
	MasterConfig.action_param[1].timer 		:= 500; 		(* Refresh timer for this item *)
	MasterConfig.action_param[1].type 		:= 15; 			(* Function code for this item (15 := write multiple coils) *)

	MasterConfig.action_enable[2].cyclic 	:= 1; 			(* Read data cyclic *)
	MasterConfig.action_param[2].p_pv		:= ADR(A_In); 	(* Store data in this variable *)
	MasterConfig.action_param[2].quantity 	:= 20; 			(* Number of items to read *)
	MasterConfig.action_param[2].unit 		:= 1;			(* Unit address - ignore if not specified *)
	MasterConfig.action_param[2].start_addr := 16#0; 		(* Read from this modbus address *)
	MasterConfig.action_param[2].timer 		:= 500; 		(* Refresh timer for this item *)
	MasterConfig.action_param[2].type 		:= 4; 			(* Function code for this item (4 := read inputs) *)

	MasterConfig.action_enable[3].cyclic 	:= 1; 			(* Write data cyclic *)
	MasterConfig.action_param[3].p_pv		:= ADR(A_Out); 	(* Take data from this address *)
	MasterConfig.action_param[3].quantity 	:= 20; 			(* Number of items to write *)
	MasterConfig.action_param[3].unit 		:= 1;			(* Unit address - ignore if not specified *)
	MasterConfig.action_param[3].start_addr := 16#0; 		(* Write to this modbus address *)
	MasterConfig.action_param[3].timer 		:= 500; 		(* Refresh timer for this item *)
	MasterConfig.action_param[3].type 		:= 16; 			(* Function code for this item (16 := write holding register) *)
	
	(* --------------------------------------------------------------------
	  Sample for reading data from a modbusTCP slave
	  --------------------------------------------------------------------- *)
	ModbusTcpMaster.enable 			:= 1;						(* Start master *)
	ModbusTcpMaster.device			:= ADR(interface); 			(* Ethernet device string *)
	ModbusTcpMaster.slave_ip_addr 	:= ip; 						(* Transfer slave device IP address *)
	ModbusTcpMaster.p_cfg 			:= ADR(MasterConfig); 		(* Pointer to configuration *)
	ModbusTcpMaster.cycle_time 		:= 10; 						(* Master cycle time in ms *)
	
	(* The LOG information will show communication details. Enable for debugging, 
	  disable for production to save system resources *)
	ModbusTcpMaster.log.data_lines := 20; 						(* Number of logging lines *)
	ModbusTcpMaster.log.pData := ADR(logger); 					(* Pointer to logger string array *)
	StartStop := 1;
END_PROGRAM


PROGRAM _CYCLIC
	(* --------------------------------------------------------------------
	  Cyclic function call
	  --------------------------------------------------------------------- *)
	IF(StartStop) THEN
		ModbusTcpMaster();
	END_IF

	(* --------------------------------------------------------------------
	  Reset last error information
	  --------------------------------------------------------------------- *)
	IF(reset_error) THEN
		brsmemset(ADR(ModbusTcpMaster.last_error), 0, 2);
		reset_error := 0;
	END_IF
END_PROGRAM
