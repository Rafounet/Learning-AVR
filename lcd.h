#ifndef LCD_H
#define LCD_H
/*************************************************************************
Title  :   C include file for the HD44780U LCD library (lcd.c)
Author:    Peter Fleury <pfleury@gmx.ch>  http://tinyurl.com/peterfleury
File:      $Id: lcd.h,v 1.14.2.4 2015/01/20 17:16:07 peter Exp $
Software:  AVR-GCC 4.x
Hardware:  any AVR device, memory mapped mode only for AVR with 
           memory mapped interface (AT90S8515/ATmega8515/ATmega128)
 ***************************************************************************/
 
#include <inttypes.h>
#include <avr/pgmspace.h>
 
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 405
#error "This library requires AVR-GCC 4.5 or later, update to newer AVR-GCC compiler !"
#endif

 
/*
* LCD and target specific definitions below can be defined in a separate include file with name lcd_definitions.h instead modifying this file 
* by adding -D_LCD_DEFINITIONS_FILE to the CDEFS section in the Makefile
* All definitions added to the file lcd_definitions.h will override the default definitions from lcd.h
*/
#ifdef _LCD_DEFINITIONS_FILE
#include "lcd_definitions.h"
#endif
 
 
#ifndef LCD_CONTROLLER_KS0073 
#define LCD_CONTROLLER_KS0073 0  
#endif
 
#ifndef LCD_LINES
#define LCD_LINES           2     
#endif
#ifndef LCD_DISP_LENGTH
#define LCD_DISP_LENGTH    16     
#endif
#ifndef LCD_LINE_LENGTH
#define LCD_LINE_LENGTH  0x40     
#endif
#ifndef LCD_START_LINE1
#define LCD_START_LINE1  0x00     
#endif
#ifndef LCD_START_LINE2
#define LCD_START_LINE2  0x40     
#endif
#ifndef LCD_START_LINE3
#define LCD_START_LINE3  0x14     
#endif
#ifndef LCD_START_LINE4
#define LCD_START_LINE4  0x54     
#endif
#ifndef LCD_WRAP_LINES
#define LCD_WRAP_LINES      0     
#endif
 
 
#define LCD_IO_MODE      1            
#if LCD_IO_MODE

#ifndef LCD_PORT
#define LCD_PORT         PORTA        
#endif
#ifndef LCD_DATA0_PORT
#define LCD_DATA0_PORT   LCD_PORT     
#endif
#ifndef LCD_DATA1_PORT
#define LCD_DATA1_PORT   LCD_PORT     
#endif
#ifndef LCD_DATA2_PORT
#define LCD_DATA2_PORT   LCD_PORT     
#endif
#ifndef LCD_DATA3_PORT
#define LCD_DATA3_PORT   LCD_PORT     
#endif
#ifndef LCD_DATA0_PIN
#define LCD_DATA0_PIN    0            
#endif
#ifndef LCD_DATA1_PIN
#define LCD_DATA1_PIN    1            
#endif
#ifndef LCD_DATA2_PIN
#define LCD_DATA2_PIN    2            
#endif
#ifndef LCD_DATA3_PIN
#define LCD_DATA3_PIN    3            
#endif
#ifndef LCD_RS_PORT
#define LCD_RS_PORT      LCD_PORT     
#endif
#ifndef LCD_RS_PIN
#define LCD_RS_PIN       4            
#endif
#ifndef LCD_RW_PORT
#define LCD_RW_PORT      LCD_PORT     
#endif
#ifndef LCD_RW_PIN
#define LCD_RW_PIN       5            
#endif
#ifndef LCD_E_PORT
#define LCD_E_PORT       LCD_PORT     
#endif
#ifndef LCD_E_PIN
#define LCD_E_PIN        6            
#endif
 
#elif defined(__AVR_AT90S4414__) || defined(__AVR_AT90S8515__) || defined(__AVR_ATmega64__) || \
       defined(__AVR_ATmega8515__)|| defined(__AVR_ATmega103__) || defined(__AVR_ATmega128__) || \
       defined(__AVR_ATmega161__) || defined(__AVR_ATmega162__)
/*
  * memory mapped mode is only supported when the device has an external data memory interface
  */
#define LCD_IO_DATA      0xC000    /* A15=E=1, A14=RS=1                 */
#define LCD_IO_FUNCTION  0x8000    /* A15=E=1, A14=RS=0                 */
#define LCD_IO_READ      0x0100    /* A8 =R/W=1 (R/W: 1=Read, 0=Write   */
 
#else
#error "external data memory interface not available for this device, use 4-bit IO port mode"

#endif
 
 
#ifndef LCD_DELAY_BOOTUP
#define LCD_DELAY_BOOTUP   16000      
#endif
#ifndef LCD_DELAY_INIT
#define LCD_DELAY_INIT      5000      
#endif
#ifndef LCD_DELAY_INIT_REP
#define LCD_DELAY_INIT_REP    64      
#endif
#ifndef LCD_DELAY_INIT_4BIT
#define LCD_DELAY_INIT_4BIT   64      
#endif
#ifndef LCD_DELAY_BUSY_FLAG
#define LCD_DELAY_BUSY_FLAG    4      
#endif
#ifndef LCD_DELAY_ENABLE_PULSE
#define LCD_DELAY_ENABLE_PULSE 1      
#endif
 
 
/* instruction register bit positions, see HD44780U data sheet */
#define LCD_CLR               0      /* DB0: clear display                  */
#define LCD_HOME              1      /* DB1: return to home position        */
#define LCD_ENTRY_MODE        2      /* DB2: set entry mode                 */
#define LCD_ENTRY_INC         1      /*   DB1: 1=increment, 0=decrement     */
#define LCD_ENTRY_SHIFT       0      /*   DB2: 1=display shift on           */
#define LCD_ON                3      /* DB3: turn lcd/cursor on             */
#define LCD_ON_DISPLAY        2      /*   DB2: turn display on              */
#define LCD_ON_CURSOR         1      /*   DB1: turn cursor on               */
#define LCD_ON_BLINK          0      /*     DB0: blinking cursor ?          */
#define LCD_MOVE              4      /* DB4: move cursor/display            */
#define LCD_MOVE_DISP         3      /*   DB3: move display (0-> cursor) ?  */
#define LCD_MOVE_RIGHT        2      /*   DB2: move right (0-> left) ?      */
#define LCD_FUNCTION          5      /* DB5: function set                   */
#define LCD_FUNCTION_8BIT     4      /*   DB4: set 8BIT mode (0->4BIT mode) */
#define LCD_FUNCTION_2LINES   3      /*   DB3: two lines (0->one line)      */
#define LCD_FUNCTION_10DOTS   2      /*   DB2: 5x10 font (0->5x7 font)      */
#define LCD_CGRAM             6      /* DB6: set CG RAM address             */
#define LCD_DDRAM             7      /* DB7: set DD RAM address             */
#define LCD_BUSY              7      /* DB7: LCD is busy                    */
 
 /* set entry mode: display shift on/off, dec/inc cursor move direction */
#define LCD_ENTRY_DEC            0x04   /* display shift off, dec cursor move dir */
#define LCD_ENTRY_DEC_SHIFT      0x05   /* display shift on,  dec cursor move dir */
#define LCD_ENTRY_INC_           0x06   /* display shift off, inc cursor move dir */
#define LCD_ENTRY_INC_SHIFT      0x07   /* display shift on,  inc cursor move dir */
 
/* display on/off, cursor on/off, blinking char at cursor position */
#define LCD_DISP_OFF             0x08   /* display off                            */
#define LCD_DISP_ON              0x0C   /* display on, cursor off                 */
#define LCD_DISP_ON_BLINK        0x0D   /* display on, cursor off, blink char     */
#define LCD_DISP_ON_CURSOR       0x0E   /* display on, cursor on                  */
#define LCD_DISP_ON_CURSOR_BLINK 0x0F   /* display on, cursor on, blink char      */
 
/* move cursor/shift display */
#define LCD_MOVE_CURSOR_LEFT     0x10   /* move cursor left  (decrement)          */
#define LCD_MOVE_CURSOR_RIGHT    0x14   /* move cursor right (increment)          */
#define LCD_MOVE_DISP_LEFT       0x18   /* shift display left                     */
#define LCD_MOVE_DISP_RIGHT      0x1C   /* shift display right                    */
 
/* function set: set interface data length and number of display lines */
#define LCD_FUNCTION_4BIT_1LINE  0x20   /* 4-bit interface, single line, 5x7 dots */
#define LCD_FUNCTION_4BIT_2LINES 0x28   /* 4-bit interface, dual line,   5x7 dots */
#define LCD_FUNCTION_8BIT_1LINE  0x30   /* 8-bit interface, single line, 5x7 dots */
#define LCD_FUNCTION_8BIT_2LINES 0x38   /* 8-bit interface, dual line,   5x7 dots */
 
 
#define LCD_MODE_DEFAULT     ((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC) )
 
 
 
extern void lcd_init(uint8_t dispAttr);
 
 
extern void lcd_clrscr(void);
 
 
extern void lcd_home(void);
 
 
extern void lcd_gotoxy(uint8_t x, uint8_t y);
 
 
extern void lcd_putc(char c);
 
 
extern void lcd_puts(const char *s);

 
extern void lcd_puts_p(const char *progmem_s);
 

extern void lcd_command(uint8_t cmd);

 
extern void lcd_data(uint8_t data);
 
 
#define lcd_puts_P(__s)         lcd_puts_p(PSTR(__s))
 
#endif //LCD_H
