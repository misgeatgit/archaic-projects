#ifndef  __GLOBALS_H__ 
#define  __GLOBALS_H__ 
unsigned int CRITICAL_FAN_ON_TEMP ;
unsigned int CRITICAL_HEATER_ON_TEMP;
unsigned int CURRENT_TEMP;
typedef enum {RED=0,GREEN,BLUE,WHITE} color ;
typedef enum {OFF=0,ON=1} status;
typedef enum {LOW=0,MEDIUM,HIGH} weight;
typedef enum {BR_STATE,LIGHT_STATE,HEATER_FAN_TEMP} confParam;
#define DEBOUNCE_TIME 20
#define BR_VALUE_MEMLOC (unsigned char *)0x01
#define LCOLOR_VALUE_MEMLOC (unsigned char *)0x02
#define FAN_ON_VALUE_MEMLOC (unsigned char *)0x03
#define HEATER_ON_VALUE_MEMLOC (unsigned char *)0x04
color LIGHT_COLOR;
status FAN;
status HEATER;
status LAMP;
weight BRIGHTNESS;
void saveConfiguration(confParam );
#endif

