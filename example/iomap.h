/*******************************************************************************
 * file:   iomap.h
 * author: Robert Bartlett-Schneider 
 *         robert@savetherobots.org
 *         http://www.savetherobots.org
 *  
 * Contains information on the MCF5206e C3 evaluation board's I/O memory map
 *******************************************************************************/

#ifndef _IOMAP_
#define _IOMAP_

#define IOBASE          0x40000000
    
#define AUDIO_COMMAND   (IOBASE+0x0)
#define AUDIO_STATUS    (IOBASE+0x2)

#define SWITCH_COLUMN   (IOBASE+0x4)    // 2 bit byte write only
#define SWITCH_ROW      (IOBASE+0x4)    // 4 bit byte read only

#define LED_ROW         (IOBASE+0x6)    // byte write only
#define LED_COLUMN      (IOBASE+0x8)    // byte write only

#define SPEAKER_OUT     (IOBASE+0xA)    // word write only
#define MICROPHONE_IN   (IOBASE+0xA)    // word read only

#define LCDBASE         0x50000000

#define LCD_COMMAND     (LCDBASE+0x0)   // byte read/write
#define LCD_DATA        (LCDBASE+0x2)   // byte read/write