#ifndef CTRLMGR_STAT_H
#define CTRLMGR_STAT_H

/*	Status Codes	*/
typedef enum {
	STATUS_OK				= 0x00,
	STATUS_FAIL				= 0x01,
	STATUS_CRITICAL_FAIL	= 0xFF
} OperationStatus;

#endif