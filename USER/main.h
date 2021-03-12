#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define Main_Buf_Length 32

typedef struct
{
    unsigned char HEAD1;
    unsigned char HEAD2;
    float quaternion_0;
    float quaternion_1;
    float quaternion_2;
    float quaternion_3;
    float acc_0;
    float acc_1;
    float acc_2;
    unsigned char TAIL1;
    unsigned char TAIL2;
}STMAINDATA;    

union MAIN_SEND_DATA
{
    unsigned char buf[Main_Buf_Length],
    STMAINDATA stMainData
};

MAIN_SEND_DATA MainSendData;

#endif 

