FUNCTION_BLOCK CfgGetInaNode			(*reads the node number from the AR registry for a INA device; asynchronous execution*)			
	VAR_INPUT							
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
		InaNode			: USINT;		(*INA node number*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetInaNode			(*enters the node number into the AR registry for INA device; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
		InaNode			: USINT;		(*INA node number*)
		Option			: UDINT;		(*options: cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetIPAddr				(*reads IP address from the AR registry for Ethernet device; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
		pIPAddr			: UDINT;		(*IP address given as a pointer*)
		Len				: USINT;		(*length of the string (pIPAddr) -> ideally, Len = 16*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetIPAddr				(*enters IP address into the AR registry for Ethernet device; asynchronous execution*)
	VAR_INPUT	
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
		pIPAddr			: UDINT;		(*IP address given as pointer, valid range "1.0.0.1" - "247.255.255.255"*)
		Option			: UDINT;		(*options:  cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetSubnetMask			(*reads subnet mask from the AR registry for Ethernet device; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
		pSubnetMask		: UDINT;		(*subnet mask*)
		Len				: USINT;		(*length of the string (pSubnetMask) -> ideally, Len = 16*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
		i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetSubnetMask			(*enters subnet mask into the AR registry for Ethernet device; asynchronous execution*)			
	VAR_INPUT							
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
		pSubnetMask		: UDINT;		(*subnet mask given as a pointer, valid range "1.0.0.1" - "255.255.255.254"*)
		Option			: UDINT;		(*options: cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetDefaultGateway		(*reads default gateway from AR registry for Ethernet device; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
		pGateway		: UDINT;		(*default gateway address given as pointer*)
		Len				: USINT;		(*length of the string (pGateway) -> ideally, Len = 16*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetDefaultGateway		(*enters default gateway into AR registry for Ethernet device; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
		pGateway		: UDINT;		(*default gateway address given as pointer, valid range "1.0.0.1" - "247.255.255.255"*)
		Option			: UDINT;		(*options: cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetBroadcastAddr		(*reads broadcast address from AR registry for Ethernet device; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
		pBroadcastAddr	: UDINT;		(*broadcast address given as a pointer*)
		Len				: USINT;		(*length of the string (pBroadcastAddr) -> ideally, Len = 16*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetBroadcastAddr		(*enters broadcast address into the AR registry for Ethernet device; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
		pBroadcastAddr	: UDINT;		(*broadcast address given as a pointer, valid range "1.0.0.1" - "255.255.255.255"*)
		Option			: UDINT;		(*options: cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetEthBaudrate		(*reads the baud rate from the AR registry for Ethernet device; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
		Baudrate		: UDINT;		(*baud rate, e.g. cfgETHBAUDRATE_AUTO, cfgETHBAUDRATE_10, cfgETHBAUDRATE_10FD ...*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetEthBaudrate		(*enters the baud rate into the AR registry for Ethernet device; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
		Baudrate		: UDINT;		(*Ethernet baud rate, e.g. cfgETHBAUDRATE_AUTO, cfgETHBAUDRATE_10, cfgETHBAUDRATE_10FD ...*)
		Option			: UDINT;		(*options: cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetEthConfigMode		(*reads the configuration mode; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
		ConfigMode		: UDINT;		(*configuration mode: cfgCONFIGMODE_MANUALLY, cfgCONFIGMODE_DHCPCLIENT*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetEthConfigMode		(*modifies the configuration mode for Ethernet device; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
		ConfigMode		: UDINT;		(*configuration mode: cfgCONFIGMODE_MANUALLY, cfgCONFIGMODE_DHCPCLIENT*)
		Option			: UDINT;		(*options: cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetHostName			(*reads a host name for a target; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pHostName		: UDINT;		(*hostname*)
		Len				: USINT;		(*length of the string (HostName)*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetHostName			(*sets a host name for a target; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pHostName		: UDINT;		(*hostname*)
		Option			: UDINT;		(*options: cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetMacAddr			(*reads the MAC address of an Ethernet device*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDevice			: UDINT;		(*INA device name given as pointer (syntax: SL<x>.SS<y>.IF<z>)*)
		pMacAddr		: UDINT;		(*MAC address (pointer to buffer where the MAC address should be written)*)
		Len				: USINT;		(*length of the MAC address (buffer - pMacAddr) -> normally 6 bytes*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetFTPServer			(*enables/disables the FTP server*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		Option			: UDINT;		(*options: cfgOPTION_DISABLE, cfgOPTION_ENABLE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetTimeOffset			(*returns the configured time offset*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, 0xXXXX = see help*)
		timeoffset		: INT;			(*configured time offset*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetTimeOffset			(*sets a time offset*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
        timeoffset		: INT;			(*configured time offset*)
        option			: UDINT;		(*options: cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
        status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetSntpServer			(*starts the SNTP server*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
        start			: UDINT;		(*1 Starts the SNTP server*)
        option			: UDINT;		(*Options: cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
        status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetSntpClient			(*starts and stops the SNTP client*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		run				: UDINT;		(*0 stop client, 1 start client*)
		pServerAddr		: UDINT;		(*pointer to a string with the IP address of the SNTP server*)
		sntpcInterval	: UINT;			(*interval in minutes that it takes for the SNTP client to synchronize the time with the server*)
		rtcInterval		: UINT;			(*interval in minutes that it takes to synchronize the RTC with the software time during SNTP operation*)
		option			: UDINT;		(*options: cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, 0xXXXX = see help*)
		syncStatus		: USINT;		(*0 Client not active, 1 Client already active, but time not received from server, ...*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetSntpcData			(*returns the parameters relevant for the SNTP client*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pServerAddr		: UDINT;		(*pointer to a long enough string to contain the server address or server name*)
		Len				: USINT;		(*length of the string (pServerAddr)*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, 0xXXXX = see help*)
		run				: UDINT;		(*1 Client active, 0 Client inactive*)
        sntpcInterval	: UINT;			(*interval that it takes for the SNTP client to synchronize the time with the server*)
        rtcInterval		: UINT;			(*interval that it take to synchronize the RTC with the software time during SNTP operation*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetSntpsData			(*returns whether the SNTP server is active*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, 0xXXXX = see help*)
		run				: UDINT;		(*1 server active, 0 server inactive*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetTimeZone			
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
	END_VAR

	VAR_OUTPUT
		timezoneID		: UINT;			
        status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetTimeZone			
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		timezoneID		: UINT;			
		option			: UDINT;		
	END_VAR

	VAR_OUTPUT
        status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetTimeZoneInfo		
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		timezoneID		: UINT;			
	END_VAR

	VAR_OUTPUT
		gmt				: INT;			
		gmtDst			: INT;			
        status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetTimeZoneEntry		
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		timezoneID		: UINT;			
		year			: UINT;			
	END_VAR

	VAR_OUTPUT
		dateDst			: DATE_AND_TIME;	
		dateNormal		: DATE_AND_TIME;	
        status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetDst				
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
	END_VAR

	VAR_OUTPUT
		dst				: BOOL;			
        status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetDst				
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		dst				: BOOL;			
		option			: UDINT;		
	END_VAR

	VAR_OUTPUT
        status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetCIFSDomain				(*cifs - reads domain name from the AR registry; asynchronous execution*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		pDomain			: UDINT;		(*Domainname given as a pointer*)
		Len				: USINT;		(*length of the string (pDomain) -> ideally, Len = 16*)
	END_VAR

	VAR_OUTPUT
		isWorkgroup		: USINT;		(*specifies if Workgroup or Domain*)
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetCIFSDomain			(*cifs - enters domain into the AR registry; asynchronous execution*)
	VAR_INPUT	
		enable			: BOOL;			(*enables execution*)
		pDomain			: UDINT;		(*Domainname given as pointer*)
		isWorkgroup		: USINT;		(*specifies if Workgroup or Domain cfgOPTION_NON_WORKGROUP, cfgOPTION_WORKGROUP*)
		Option			: UDINT;		(*options:  cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetCIFSDefaultuser	(*cifs - reads default user from the AR registry; asynchronous execution*)
	VAR_INPUT
		enable				: BOOL;			(*enables execution*)
		pDefaultuser		: UDINT;		(*Default Username given as a pointer*)
		lenUser				: USINT;		(*length of the string (pDefaultuser) -> ideally, Len = 16*)
		pDefaultpassword	: UDINT;		(*Default Password given as a pointer*)
		lenPassword			: USINT;		(*length of the string (pDefaultpassword) -> ideally, Len = 16*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetCIFSDefaultuser	(*cifs - enters default user into the AR registry; asynchronous execution*)
	VAR_INPUT	
		enable				: BOOL;			(*enables execution*)
		pDefaultuser		: UDINT;		(*Default Username given as pointer*)
		pDefaultpassword	: UDINT;		(*Default Password given as a pointer*)
		Option				: UDINT;		(*options:  cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetDhcpServer 	(*dhcp - starts or stops dhcp server*)
	VAR_INPUT	
		enable				: BOOL;			(*enables execution*)
		start				: UDINT;		(*1 = start server; 0 = stop server*)
		option				: UDINT;		(*options:  cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetDhcpsData 	(*dhcp - configures dhcp server*)
	VAR_INPUT	
		enable				: BOOL;			(*enables execution*)
		pInterfaceList		: UDINT;		(*provided interfaces*)
		pLeases				: UDINT;		(*provided leases*)
		numLeases			: UDINT;		(*number of leases*)
		option				: UDINT;		(*options:  cfgOPTION_VOLATILE, cfgOPTION_NON_VOLATILE*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetDhcpsData 	(*dhcp - reads configuration of dhcp server*)
	VAR_INPUT	
		enable				: BOOL;			(*enables execution*)
		pInterfaceList		: UDINT;		(*provided interfaces*)
		len					: UDINT;		(*length of the string (pInterfaces)*)
		pLeases				: UDINT;		(*provided leases*)
		numLeases			: UDINT;		(*number of leases*)
	END_VAR

	VAR_OUTPUT
		startMode		: UDINT;		(*1 = server is working; 0 = server is not working*)
		numNeeded		: UDINT;		(*needed length*)
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, ERR_FUB_BUSY, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgSetSdmStatus			(*enables/disables data refresh of system diagnostics manager*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
		start			: UDINT;		(*options: cfgOPTION_ENABLE_SDM; cfgOPTION_DISABLE_SDM*)
		Option			: UDINT;		(*reserve*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, 0xXXXX = see help*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK CfgGetSdmStatus			(*returns whether the system diagnostics manager is active*)
	VAR_INPUT
		enable			: BOOL;			(*enables execution*)
	END_VAR

	VAR_OUTPUT
		status			: UINT;			(*execution status: ERR_OK, ERR_FUB_ENABLE_FALSE, 0xXXXX = see help*)
		run				: UDINT;		(*status: cfgSTATUS_SDM_ENABLED, cfgSTATUS_SDM_DISABLED*)
	END_VAR

	VAR
        i_state			: UINT;			(*internal variable*)
		i_result		: UINT;			(*internal variable*)
        i_tmp			: UDINT;		(*internal variable*)
	END_VAR
END_FUNCTION_BLOCK
