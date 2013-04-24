/*******************************************************************************
 * file:   includes.h
 * author: Robert Bartlett-Schneider 
 *         robert@savetherobots.org
 *         http://www.savetherobots.org
 *  
 * Master include file to centralize the includes. 
 *******************************************************************************/

#include <stdio.h>      // Standard C libraries
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "os_cpu.h"     // MCF5206e port header file
#include "os_cfg.h"     // MCF5206e port configuration file
#include "ucos_ii.h"    // ucos master file
#include "imm.h"        // MCF5206e internal memory map
#include "vectors.h"    // MCF5206e exception vectors
#include "led.h"        // MCF5206e evaluation board LED info