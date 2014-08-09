#include "lrs.h"
#include "def.h"
#include "socket.h"

vuser_init()
{
	// initializes the WinSock DLL
    lrs_startup(257);

	// loads the external DLL (see the project 'plugin')
	lr_load_dll("plugin.dll");

	// sets a timeout for connecting to a socket.
	lrs_set_connect_timeout(SEAWEI_TIMEOUT_CONNECT, 0);

	// connect to the server
	lrs_create_socket("server", "TCP", SERVER_ADDR, LrsLastArg);

    return 0;
}
