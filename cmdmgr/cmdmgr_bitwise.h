#include <stdio.h>
#include <stdint.h>

#include "../ctrlmgr/ctrlmgr_ctrl.h"


OperationStatus extract_l_bits(uint8_t *parameter, uint8_t position){

	OperationStatus status = STATUS_OK; //Assume that everything is ok
	
	//status = assert_equality(parameter, NULL, NOT_EQUAL);
	if(parameter == NULL){
		status = STATUS_FAIL;
	}else{
		
	}
	return status;
}

OperationStatus extract_r_bits(uint8_t *parameter, uint8_t position){
	


}
