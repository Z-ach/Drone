#pragma once

#include <stdio.h>


/*	Status Codes	*/
typedef enum {
	STATUS_OK				= 0x00,
	STATUS_FAIL				= 0x01,
	STATUS_CRITICAL_FAIL	= 0xFF
} OperationStatus;

typedef enum {
	NOT_EQUAL	= 0x0,
	EQUAL		= 0x1
} Equality;

/*	Assertion-Like Helpers	*/
OperationStatus assert_equalilty(void *var1, void *var2, Equality equality);
