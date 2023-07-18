#ifndef RTC_H_
#define RTC_H_



extern bool alarm_flag;

void InitRTC();
void RTCInterrupt();

unsigned int RTCGetTime();
void RTCSetCounter(uint32_t value);

unsigned int RTCGetAlarm();
void RTCSetAlarm(uint32_t value);

#endif
