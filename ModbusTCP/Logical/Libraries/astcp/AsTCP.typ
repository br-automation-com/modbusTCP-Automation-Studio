(********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: AsTcp
 * File: AsTcp.typ
 * Author: feinerr
 * Created: 09.08.2006
 ********************************************************************
 * Data types of library AsTcp
 ********************************************************************)

TYPE
    tcpLINGER_typ		: STRUCT	(*linger*)
		lOnOff			: UDINT;	(*linger on/off*)
		lLinger			: UDINT;	(*linger time (seconds)*)
	END_STRUCT;
	
	tcpSO_ADDRESS_typ	: STRUCT	(*socket address*)
		pPort			: UDINT;	(*pointer where to store the portnumber*)
		pIpAddr			: UDINT;	(*pointer where to store the socket ip address*)
	END_STRUCT;	
END_TYPE
