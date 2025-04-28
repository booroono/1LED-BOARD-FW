/*
 * main.h
 *
 * Created: 2023-01-13 오전 12:36:35
 *  Author: monst
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <atmel_start.h>
#include <stdio.h>
#include <string.h>
#include "bsp.h"
#include "conversion.h"
#include "def.h"
#include "drv_ads8681.h"
#include "drv_hall_sensor.h"
#include "drv_proximity_sensor.h"
#include "ext_control.h"
#include "manual_test.h"
#include "tws_test.h"
#include "usart.h"

#define FW_VER 			126

// Ver 1.26 : 펌웨어 버전 업데이트
// ver 0.25 : test ok led, Prox Test before delay delete
// Ver 0.24 : Prox Test before delay Add
// Ver 0.22 : Jig delay Modify, Conn OS SEQ Change
// Ver 0.13 : Prox Delay edit
// Ver 0.12 : CONN OS Delay ADD
// Ver 0.11 : CONN OS Delay Select ADD
// Ver 0.10 : HW Rev 1.1 
// ver 0.01 : First Coding Done

#endif /* MAIN_H_ */