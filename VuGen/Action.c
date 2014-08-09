#include "lrs.h"
#include "socket.h"
#include "def.h"

Action()
{
	int ret;

	// need create a parameter (name="ID", type="Vuser ID")
	int id = atoi(lr_eval_string("<ID>"));

	LR_TEST(id, MSGID_SAMPLE, "server", "sample");

    return 0;
}

