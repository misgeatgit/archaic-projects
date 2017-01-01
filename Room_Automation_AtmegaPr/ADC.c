#include <avr/io.h>
#include"ADC.h"
void InitADC( void)
{
    ADMUX = 0; //Select channel 0
    ADCSRA = 0xC3; //Enable ADC & start 1st
    //dummy
    //conversion
    //Set ADC module prescalar
    //to 8 critical for
    //accurate ADC results
    while (!(ADCSRA & 0x10)); //Check if conversation is
    //ready
    ADCSRA |= 0x10; //Clear conv rdy flag -
    //set the bit
}
unsigned int ReadADC(unsigned char channel)
{
    unsigned int binaryWeightedVoltage, binaryWeightedVoltageLow;
    unsigned int binaryWeightedVoltageHigh; //weighted binary
    //voltage
    ADMUX = channel; //Select channel
    ADCSRA |= 0x43; //Start conversion
    //Set ADC module prescalar
    //to 8 critical for
    //accurate ADC results
    while (!(ADCSRA & 0x10)); //Check if converstion is
    //ready
    ADCSRA |= 0x10; //Clear Conv rdy flag - set
    //the bit
    binaryWeightedVoltageLow = ADCL; //Read 8 low bits first
    //(important)
    //Read 2 high bits,
    //multiply by 256
    binaryWeightedVoltageHigh = ((unsigned int)(ADCH << 8));
    binaryWeightedVoltage =
        binaryWeightedVoltageLow | binaryWeightedVoltageHigh;

    return binaryWeightedVoltage; //ADCH:ADCL
}
