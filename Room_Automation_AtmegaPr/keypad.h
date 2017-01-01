#ifndef __KEYPAD_H__
#define __KEYPAD_H__
#define MODE_KEY  ((PINB) & (0x01))
#define OK_KEY    ((PINB) & (0x02))
#define BACK_KEY  ((PINB) & (0x04))
#define UP_KEY    ((PINB) & (0x08))
#define FW_KEY    ((PINB) & (0x10))
#define DOWN_KEY  ((PINB) & (0x20))
void initKeypad(void);
void  tempController(void);
void scanKey(void);
void waitAllKeyUp();
#endif

