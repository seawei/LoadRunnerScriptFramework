#include "lrs.h"
#include "def.h"

vuser_end()
{
	// disconnets from the server
	lrs_close_socket("server");

	// terminates the use of the WinSock DLL
    lrs_cleanup();

    return 0;
}

