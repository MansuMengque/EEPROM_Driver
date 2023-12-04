#ifndef		__TIMEOUT_H__
#define		__TIMEOUT_H__
#include "sys.h" 
#ifdef __cplusplus
 extern "C" {
#endif

void delay_init(uint16_t sysclock);
void delay_us(u32 nus);
void delay_ms(u16 nms);

void poweronDelay(void);
void waitStart(u32 nms);
void waitBusy(u32 nms);
void timeOutRest(void);
#ifdef __cplusplus
}
#endif

#endif

