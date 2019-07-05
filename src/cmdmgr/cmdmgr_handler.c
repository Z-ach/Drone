#include "cmdmgr_handler.h"

void *cmd_handler(void *shared_status){

	SharedStatus *status = (SharedStatus *)shared_status;

	printf("Hello!");

	return NULL;
}
