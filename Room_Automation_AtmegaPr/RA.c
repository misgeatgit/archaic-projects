#include "LCD.h"
#include "globals.h"
#include "ADC.h"
#include "keypad.h"
#include <util/delay.h>
#include <avr/eeprom.h>
extern unsigned int CURRENT_TEMP;
extern color LIGHT_COLOR;
extern weight BRIGHTNESS;
extern unsigned int CRITICAL_FAN_ON_TEMP ;
extern unsigned int CRITICAL_HEATER_ON_TEMP;
void lightController(void);
void  tempController(void);
int main(){   
    InitADC();
    //reading prev configurations
    LIGHT_COLOR=eeprom_read_byte(LCOLOR_VALUE_MEMLOC);
    BRIGHTNESS=eeprom_read_byte(BR_VALUE_MEMLOC);
    CRITICAL_FAN_ON_TEMP=eeprom_read_byte(FAN_ON_VALUE_MEMLOC ) ;
    CRITICAL_HEATER_ON_TEMP=eeprom_read_byte(HEATER_ON_VALUE_MEMLOC);  

    CURRENT_TEMP=(unsigned int)(ReadADC(0x40)/2); 
    dispStatScreen();
    while(1){
        tempController();
        lightController();   
        if((2*CURRENT_TEMP)!=(unsigned int)ReadADC(0x40)){ //if there is a temprature change
            CURRENT_TEMP=(unsigned int)ReadADC(0x40)/2;
            if(CURRENT_TEMP<10)
                setCursorPosn(RIGHT,ROW_0,6);   
            setCursorPosn(RIGHT,ROW_0,5);//cursor set to where temp in number is displayed
            putString(intToString(CURRENT_TEMP));
        }   
        scanKey(); 
    }//END OF WHILE   
    return 0;
}

void lightController(){
    DDRD=DDRD | 0xfc;// PD2-PD7 output for light control
    switch(LIGHT_COLOR){
        case RED:{
                     PORTD= (PORTD | 0b00000100) & 0b11000111;
                 } break;
        case GREEN:{
                       PORTD= (PORTD | 0b00001000) & 0b11001011;  
                   } break;
        case BLUE:{
                      PORTD=(PORTD |  0b00010000) & 0b11010011;
                  } break;
        case WHITE:{
                       PORTD= (PORTD | 0b00100000) & 0b11100011;
                   } break;
        default:break;
    }
}

void tempController(void){
    DDRD=DDRD | 0x03;// PD0 and PD1 output for fan and heater control
    if(CURRENT_TEMP>=CRITICAL_FAN_ON_TEMP){
        PORTD=PORTD | 0x02;
    }
    if(CURRENT_TEMP<=CRITICAL_HEATER_ON_TEMP){
        PORTD=PORTD | 0X01;
    }
    if(CURRENT_TEMP<CRITICAL_FAN_ON_TEMP &&  CURRENT_TEMP>CRITICAL_HEATER_ON_TEMP){
        PORTD=PORTD & 0XFC;
    }

}
