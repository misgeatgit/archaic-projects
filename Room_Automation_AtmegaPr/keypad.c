#include "globals.h"
#include "LCD.h"
#include "keypad.h"
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
typedef  enum {FALSE=0,TRUE=1} keyStatus;
extern  weight BRIGHTNESS;
static unsigned char fwCount=0x00; 
keyStatus EXIT=FALSE ,MAIN_SETT_TRIGGERED=FALSE;
extern unsigned int CRITICAL_FAN_ON_TEMP,CRITICAL_HEATER_ON_TEMP;
void initKeypad();
void setBrightness(void);
void setLightColor(void);
void setCriticalTemps(void);
void scanKey()
{
    initKeypad();   
    if(MODE_KEY){ //checking if user has entered the mode key for setting the profile 
        _delay_ms(20);//Wait for a debounce       
        waitAllKeyUp(); 
        initKeypad();
        clearLCD();
        putString(" PRESS FW KEY");// directing the user   
        while(1)
        {    
            if(FW_KEY || MAIN_SETT_TRIGGERED){ 
                MAIN_SETT_TRIGGERED=FALSE;//ensuring no pseudo re-entrance to the if statement	  
                fwCount=(fwCount>0x02)?(fwCount=0x01):(fwCount+0x01);   
                if(fwCount==0x01){
                    clearLCD();
                    putString(" BRIGHTNESS");		
                }
                if(fwCount==0x02){
                    clearLCD();
                    putString(" LIGHT COLOR");		
                }
                if(fwCount==0x03){
                    clearLCD();
                    putString(" CRITICAL TEMPRATURE");		
                }
                _delay_ms(DEBOUNCE_TIME); //wait for FW key debounce     
                waitAllKeyUp();  
            }

            if(OK_KEY){	
                waitAllKeyUp();	  
                switch(fwCount){
                    case 0x01:{
                                  setBrightness();
                              }					   
                              break;
                    case 0x02:{ 
                                  setLightColor();

                              }
                              break;
                    case 0x03:{ 
                                  setCriticalTemps();

                              }
                              break;
                    default:{

                            }
                            break; 
                }
                _delay_ms(DEBOUNCE_TIME);
            } 
            //fwCount=0x00; //reset 

            if(MODE_KEY){
                waitAllKeyUp();
                dispStatScreen();
                _delay_ms(20);
                break;
            }   

        }//END OF WHILE


    }

}   //END OF scanKey Defn



void initKeypad()
{

    DDRB=DDRB | 0b11000000; //PIN B0-B5 set as input  
}

void setBrightness(void)
{
    unsigned char fwCount=0x01; 
    clearLCD();
    putString(" now press FW to");
    putCommand(0xC0);//shifting cursor to 2nd row
    putString(" set brightness");    
    while(1){  
        if(FW_KEY){  
            waitAllKeyUp();          
            fwCount=(fwCount>0x02)?(fwCount=0x01):(fwCount+0x01);	   	  
            if(fwCount==0x01){
                clearLCD();
                putString(" LOW medium high ");
            }
            if(fwCount==0x02){
                clearLCD();
                putString(" low MEDIUM high");
            }
            if(fwCount==0x03){
                clearLCD();
                putString(" low medium HIGH");
            }
        }
        _delay_ms(20); //wait for FW key debounce

        if(OK_KEY){ 
            waitAllKeyUp();       
            switch(fwCount){
                case 0x01:			        
                    BRIGHTNESS=LOW;
                    break;
                case 0x02: 
                    BRIGHTNESS=MEDIUM;
                    break;
                case 0x03: 
                    BRIGHTNESS=HIGH;
                    break;
                default:
                    break; 
            }
            saveConfiguration(BR_STATE); //save brightness setting
            break;
        }

        if(MODE_KEY){
            waitAllKeyUp();
            MAIN_SETT_TRIGGERED=TRUE;
            break;
        }

    }//END OF WHILE

}
void setLightColor(void)
{
    unsigned char fwCount=0x01; 
    clearLCD();
    putString(" now press FW to");
    putCommand(0xC0);//shifting cursor to 2nd row
    putString("set lightColor"); 
    while(1)
    {
        if(FW_KEY){  
            waitAllKeyUp();          
            fwCount=(fwCount>0x03)?(fwCount=0x01):(fwCount+0x01);	   	  
            if(fwCount==0x01){
                clearLCD();
                putString(" RED green blue ");
                putCommand(0xC0);
                putString("white");
            }
            if(fwCount==0x02){
                clearLCD();
                putString(" red GREEN blue ");
                putCommand(0xC0);
                putString("white");
            }
            if(fwCount==0x03){
                clearLCD();
                putString(" red green BLUE ");
                putCommand(0xC0);
                putString("white");
            }
            if(fwCount==0x04){
                clearLCD();
                putString(" red green blue ");
                putCommand(0xC0);
                putString("WHITE");
            }
        }

        if(OK_KEY){ 
            waitAllKeyUp();       
            switch(fwCount){
                case 0x01:			        
                    LIGHT_COLOR=RED;
                    break;
                case 0x02: 
                    LIGHT_COLOR=GREEN;
                    break;
                case 0x03: 
                    LIGHT_COLOR=BLUE;
                    break; 
                case 0x04:
                    LIGHT_COLOR=WHITE;
                    break;			         
                default:
                    break; 
            }
            saveConfiguration(LIGHT_STATE);//save lighting state

            break;
        }

        if(MODE_KEY){
            waitAllKeyUp();
            MAIN_SETT_TRIGGERED=TRUE;
            break;
        }

    } //END OF WHILE
}
void setCriticalTemps(void)
{   
    unsigned int tempFanOnTemp=CRITICAL_FAN_ON_TEMP;
    unsigned int tempHeaterOnTemp=CRITICAL_HEATER_ON_TEMP;
    unsigned char fwCount=0x00; 
    clearLCD();
    putString(" press FW to set");
    putCommand(0xC0);//shifting cursor to 2nd row
    putString(" fan/heaterTemp"); 
    while(1)
    {     
        if(FW_KEY){
            waitAllKeyUp();
            fwCount=(fwCount>0x02)?fwCount=0x01:(fwCount+0x01);
            switch(fwCount){
                case 0x01:{		 
                              clearLCD();
                              putString(" FAN ON TEMP");
                              putCommand(0xC0);
                              putString(intToString(CRITICAL_FAN_ON_TEMP));		  
                          } break;
                case 0x02:{
                              clearLCD();
                              putString(" HEATER ON TEMP");
                              putCommand(0xC0);
                              putString(intToString(CRITICAL_HEATER_ON_TEMP));	
                          }break;
                default: break;
            }
        }

        if(UP_KEY){
            waitAllKeyUp();
            switch(fwCount){
                case 0x01:{
                              tempFanOnTemp+=1;
                              clearLCD();
                              putString(" FAN ON TEMP");
                              putCommand(0xC0);
                              putString(intToString(tempFanOnTemp)); 
                          }break;
                case 0x02:{
                              tempHeaterOnTemp+=1;
                              clearLCD();
                              putString(" HEATER ON TEMP");
                              putCommand(0xC0);
                              putString(intToString(tempHeaterOnTemp));
                          }break;
                default: break;
            }
        }
        if(DOWN_KEY){
            waitAllKeyUp();
            switch(fwCount){
                case 0x01:{
                              tempFanOnTemp-=1;
                              clearLCD();
                              putString(" FAN ON TEMP");
                              putCommand(0xC0);
                              putString(intToString(tempFanOnTemp)); 
                          }break;
                case 0x02:{
                              tempHeaterOnTemp-=1;
                              clearLCD();
                              putString(" HEATER ON TEMP");
                              putCommand(0xC0);
                              putString(intToString(tempHeaterOnTemp));
                          }break;
                default: break;
            }
        } 
        if(OK_KEY){
            waitAllKeyUp();
            CRITICAL_FAN_ON_TEMP=tempFanOnTemp;
            CRITICAL_HEATER_ON_TEMP=tempHeaterOnTemp; 
            saveConfiguration(HEATER_FAN_TEMP); //saving temprature values        
            break;
        } 
        if(MODE_KEY){
            waitAllKeyUp();
            MAIN_SETT_TRIGGERED=TRUE;
            break;
        }

    }//END OF WHILE

}

void waitAllKeyUp(){
    while(MODE_KEY || OK_KEY || BACK_KEY || FW_KEY || UP_KEY || DOWN_KEY) ;
}

void saveConfiguration(confParam cp){
    switch(cp){
        case BR_STATE:{
                          eeprom_write_byte(BR_VALUE_MEMLOC,(unsigned char)BRIGHTNESS);
                      } break;
        case LIGHT_STATE:{
                             eeprom_write_byte(LCOLOR_VALUE_MEMLOC,(unsigned char)LIGHT_COLOR);
                         }  break;
        case HEATER_FAN_TEMP:{
                                 eeprom_write_byte(FAN_ON_VALUE_MEMLOC ,(unsigned char)CRITICAL_FAN_ON_TEMP);
                                 eeprom_write_byte(HEATER_ON_VALUE_MEMLOC,(unsigned char)CRITICAL_HEATER_ON_TEMP);
                             } break;
        default: break;
    }

}
