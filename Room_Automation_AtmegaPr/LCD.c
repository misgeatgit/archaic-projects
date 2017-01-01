#include "globals.h"
#include "LCD.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
char buffer[4];  //for holding values converted from int to string
extern  color   LIGHT_COLOR;
extern unsigned int CURRENT_TEMP;  
extern status  FAN,LAMP,HEATER;  
void putCommand(unsigned char d)
{
    DDRC = 0xff; //set PORTC as output
    DDRD = DDRD|0xC0; //make PORTD[7:6] output
    PORTD = PORTD&0x7f; //RS=0
    PORTC = d;
    PORTD = PORTD|0x40; //E=1
    _delay_ms(1);
    PORTD = PORTD&0xbf; //E=0

}
void clearLCD(void){
    putCommand(0x1);
    _delay_ms(3);
}
void initLCD(){ 
    putCommand(0x38); //one line, 5x7 char
    _delay_ms(1);
    putCommand(0x0C); //display on cursor off
    _delay_ms(1);
    putCommand(0x01); //display clear-1.64 ms
    _delay_ms(2);
    putCommand(0x06); //entry mode set
    _delay_ms(2);
    /*putCommand(0x01); //clear display, cursor at home
      _delay_ms(2);*/
    putCommand(0x01); //clear display, cursor at home
    _delay_ms(2);
    putCommand(0x80);
    _delay_ms(1);
}
void putChar(unsigned char c)
{
    DDRC = 0xff; //set PORTC as output
    DDRD = DDRD|0xC0; //make PORTD[7:6] output
    PORTC = c;
    PORTD = PORTD|0x80; //RS=1
    _delay_ms(1);
    PORTD = PORTD|0x40; //E=1
    PORTD = PORTD&0xbf; //E=0
    _delay_ms(2);
}  
void putString(char * string){
    while(*string!='\0'){
        putChar(*string);  
        string++;
    }  
}
void dispStatScreen(){  

    initLCD();
    clearLCD(); //clear everything from the lcd
    _delay_ms(3);
    putCommand(0x02); //return cursor home
    _delay_ms(3);
    putCommand(0x04); //shift cursor to the left
    _delay_ms(1);
    itoa(CURRENT_TEMP,buffer,10);//conversion of int to char array
    putString(" TEMP ");
    putString(buffer);  
    putString("c FAN ");
    if(FAN==OFF)
        putString("OFF"); 
    if(FAN==ON)
        putString("ON");  
    putCommand(0xC0); //cursor to the second row
    putString("LIGHT");
    if(LAMP==OFF)
        putString(" OFF");
    if(LAMP==ON){
        int color=LIGHT_COLOR;
        switch(color){
            case RED: putString(" RED ");
                      break;
            case GREEN:putString(" GREEN ");
                       break;
            case BLUE:putString(" BLUE ");
                      break;
            case WHITE:putString(" WHITE ");
                       break;
            default:putString("");	
                    break;	  
        }
    }
    /*
       if(HEATER==ON) 
       putString("HEATER ON");
       if(HEATER==OFF)
       putString("HEATER OFF");*/
}

void setCursorPosn(direction cursorDrcn,position cursorPosn,unsigned char column){
    if(cursorDrcn==RIGHT){
        if(cursorPosn==ROW_0){
            putCommand(0x80); _delay_ms(3);
        }
        if(cursorPosn==ROW_1){
            putCommand(0xC0); _delay_ms(3);
        }
        for(unsigned char count=0;count<=column;count++){
            putCommand(0x14); _delay_ms(3);//redirecting cursor to the right
        }
    }
    if(cursorDrcn==LEFT){
        if(cursorPosn==ROW_0){
            putCommand(0x80); _delay_ms(3);
        }
        if(cursorPosn==ROW_1){
            putCommand(0xC0); _delay_ms(3);
        }
        for(unsigned char count=0;count<=column;count++){
            putCommand(0x18); _delay_ms(2);//edirecting cursor to the left
        }
    }

}

char * intToString(unsigned int value){ 
    itoa(value,buffer,10);
    return buffer;
}
