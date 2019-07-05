#ifndef CMDMGR_PUB_H
#define CMDMGR_PUB_H

#include <stdint.h>


/*	Command Modes	*/
typedef enum {
    TAKE_OFF	=	0x00,
    LAND		=	0x01,
    HOVER		=	0x02,
    PATROL		=	0x03
} CommandMode;

/*	Parameters		*/
typedef enum {
    LOCATION_STAY	=	0x00
} Location;

typedef enum {
    EMERGENCY_OFF	= 0x00,
    EMERGENCY_ON	= 0x02
} Emergency;


/*	Data Types		*/
typedef struct{
    struct TakeOff{
        uint8_t altitude;
    } TakeOff;

    struct Land{
        uint8_t location;
        uint8_t emergency;
    } Land;

    struct Hover{
        uint8_t location;
        uint8_t maintain;
    } Hover;

    struct Patrol{
        uint8_t location_a;
        uint8_t location_b;
    } Patrol;
} Parameters;

typedef enum {
    STATUS_WAITING		= 0x00,
    STATUS_EXECUTING	= 0x01,
    STATUS_FINISHED		= 0x02,
    STATUS_FAILED		= 0xFF
} CommandStatus;

typedef struct {
    uint8_t counter;
    CommandMode mode;
    Parameters params;
    CommandStatus status;
} Command;

typedef enum {
    CMD_BUFFER_OK		= 0x00,
    CMD_BUFFER_FULL		= 0x01,
    CMD_BUFFER_EMPTY	= 0x02,
    CMD_BUFFER_ERR		= 0xFF
} BufferStatus;

#endif //DRONE_CMDMGR_PUB_
