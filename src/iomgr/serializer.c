#include "serializer.h"

char res_buf[12] = {0};

/*
Serialize float to custom format. Null terminator not applied,
buffer is assumed to have enough space to hold 4 additional chars.
*/
void serialize_float(float flt, char *buf){
    int sign_bit = flt > 0 ? 0 : 1;
    if(sign_bit){
        flt *= -1;
    }

    snprintf(res_buf, 12, "%.6e", flt);

    float buf_val = flt;

    while(buf_val > 10){


    }
    
}