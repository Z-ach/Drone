#include "cmdmgr_buffer.h"

typedef struct CommandNode{
	struct CommandNode *next;
	Command command;

} CommandNode;

typedef enum {
	CMD_BUF_MAX = 10
} Constants;

static CommandNode *cmd_node_head;
static uint8_t cmd_buf_size;

OperationStatus init_cmd_buf(void){
	OperationStatus status = STATUS_OK;
	cmd_buf_size = 0;
	return status;
}

BufferStatus insert_into_cmd_buf(Command cmd){
	//Ensure there is room in the buffer
	if(cmd_buf_size >= CMD_BUF_MAX)
		return CMD_BUFFER_FULL;

	CommandNode *new_cmd = malloc(sizeof(CommandNode));
	new_cmd->command = cmd;
	new_cmd->next = NULL;

	//Check if buffer is empty
	if(cmd_buf_size == 0){
		cmd_node_head = new_cmd;
	}else{
		CommandNode *iter = cmd_node_head;
		while(iter->next != NULL){
			iter = iter->next;
		}
		iter->next = new_cmd;
	}
	cmd_buf_size++;
	LOG_CMD("Insert Success\n");
	return CMD_BUFFER_OK;
}

BufferStatus fetch_next_cmd(Command *cmd){
	if(cmd_buf_size == 0) return CMD_BUFFER_EMPTY;

	//fetch command at head
	*cmd = cmd_node_head->command;

	//store next so head can be freed
	CommandNode *next = cmd_node_head->next;
	free(cmd_node_head);

	//move head to next
	cmd_node_head = next;

	//LOG_CMD("count:\t%d\nmode:\t%d\n", cmd->counter, cmd->mode);
	cmd_buf_size--;
	return CMD_BUFFER_OK;
}
