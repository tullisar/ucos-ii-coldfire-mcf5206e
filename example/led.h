/*******************************************************************************
 * file:   led.h
 * author: Robert Bartlett-Schneider 
 *         robert@savetherobots.org
 *         http://www.savetherobots.org
 *  
 * Contains information on the 7 x 5 LED array on the board
 *******************************************************************************/

#ifndef _LED_
#define _LED_

#define LED_BASE  0x40000000

#define LED_COL   LED_BASE+0x8
#define LED_ROW   LED_BASE+0x6

#endif