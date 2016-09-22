/*
 * Copyright (c) 2008, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *	Coffee architecture-dependent header for the ESB platform.
 * \author
 * 	Nicolas Tsiftes <nvt@sics.se>
 * 	Niclas Finne <nfi@sics.se>
 */

#ifndef CFS_COFFEE_ARCH_H
#define CFS_COFFEE_ARCH_H

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
//#include "contiki-conf.h"
//#include "dev/eeprom.h"

#define COFFEE_SECTOR_SIZE		131072UL
#define COFFEE_PAGE_SIZE		2048UL
#define COFFEE_START			0 //COFFEE_SECTOR_SIZE
#define COFFEE_SIZE			((1024UL * 2048UL * 64UL) - COFFEE_START)
#define COFFEE_NAME_LENGTH		16
#define COFFEE_MAX_OPEN_FILES		8
#define COFFEE_FD_SET_SIZE		6
#define COFFEE_LOG_TABLE_LIMIT		256
#define COFFEE_DYN_SIZE			2048UL//22528UL //2048UL
#define COFFEE_LOG_SIZE			1024 //(COFFEE_PAGE_SIZE*4)

#define COFFEE_IO_SEMANTICS     1
#define COFFEE_MICRO_LOGS		1
#define COFFEE_APPEND_ONLY      0
#define COFFEE_EXTENDED_WEAR_LEVELLING 0

#define COFFEE_WRITE(buf, size, offset)	cfs_coffee_arch_write((uint8_t *)(buf), size, offset);


#define COFFEE_READ(buf, size, offset) cfs_coffee_arch_read((uint8_t *)(buf), size, offset);


#define COFFEE_ERASE(sector) cfs_coffee_arch_erase(sector)

void cfs_coffee_arch_init(void);
void cfs_coffee_arch_write(uint8_t *buf, uint32_t size, uint32_t offset);
void cfs_coffee_arch_read(uint8_t *buf, uint32_t size, uint32_t offset);
void cfs_coffee_arch_erase(uint16_t sector);
uint32_t ecc_calc(uint8_t* buff);
uint8_t ecc_decode(uint8_t* buff, uint32_t ecc1);
int coffee_file_test(void);

typedef int32_t coffee_page_t;

typedef unsigned long ulong;

#endif /* !COFFEE_ARCH_H */
