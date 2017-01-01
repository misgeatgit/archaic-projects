#ifndef  __LCD_H__
#define  __LCD_H__
void putCommand(unsigned char command);
typedef enum {RIGHT=1,LEFT=0} direction;
typedef enum {ROW_0=0,ROW_1=1} position;
direction cursorDirecn;
position  cursorPosn;
void initLCD(void);
void clearLCD();
void putChar(unsigned char character);
void putString(char * string);
void dispStatScreen(void);
void setCursorPosn(direction,position,unsigned char );
char * intToString(unsigned int );
//void dispSettScreen(void);
#endif
