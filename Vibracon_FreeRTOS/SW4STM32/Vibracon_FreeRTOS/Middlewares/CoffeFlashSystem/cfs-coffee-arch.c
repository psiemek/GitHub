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
 *	Coffee architecture-dependent functionality for the ESB platform.
 * \author
 * 	Nicolas Tsiftes <nvt@sics.se>
 * 	Niclas Finne <nfi@sics.se>
 */
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "cfs-coffee-arch.h"
#include <string.h>

void Flash_init(uint8_t* flashid);
uint8_t reset(void);
uint8_t erase_block(uint16_t number_of_block);
uint8_t read_byte(ulong addr, uint8_t *_buf, uint16_t size);
int write_byte(const void *_buf, int size, unsigned long addr);
ulong flash_read_sig(uint8_t* flashid);
void delay(__IO uint32_t nCount);
void delayFast(__IO uint32_t nCount);


void
cfs_coffee_arch_init(void)
{
	uint8_t flashid[4];
	Flash_init(flashid);
}

void
cfs_coffee_arch_write(unsigned char *buf, uint32_t size, uint32_t offset)
{
	//write_byte(/*COFFEE_START + */(offset), buf, size);
	write_byte(buf, size, offset);
}

void
cfs_coffee_arch_read(unsigned char *buf, uint32_t size, uint32_t offset)
{
	read_byte(/*COFFEE_START + */(offset), buf, size);
}

void
cfs_coffee_arch_erase(uint16_t sector)
{
	erase_block(sector);
}


#define FLASH_DELAY 1

#define RE_LOW		HAL_GPIO_WritePin(FLASH_RE_GPIO_Port, FLASH_RE_Pin, GPIO_PIN_RESET);
#define RE_HIGH		HAL_GPIO_WritePin(FLASH_RE_GPIO_Port, FLASH_RE_Pin, GPIO_PIN_SET);
#define WE_LOW		HAL_GPIO_WritePin(FLASH_WE_GPIO_Port, FLASH_WE_Pin, GPIO_PIN_RESET);
#define WE_HIGH		HAL_GPIO_WritePin(FLASH_WE_GPIO_Port, FLASH_WE_Pin, GPIO_PIN_SET);
#define CE_LOW		HAL_GPIO_WritePin(FLASH_CE_GPIO_Port, FLASH_CE_Pin, GPIO_PIN_RESET);
#define CE_HIGH		HAL_GPIO_WritePin(FLASH_CE_GPIO_Port, FLASH_CE_Pin, GPIO_PIN_SET);
#define ALE_LOW		HAL_GPIO_WritePin(FLASH_ALE_GPIO_Port, FLASH_ALE_Pin, GPIO_PIN_RESET);
#define ALE_HIGH 	HAL_GPIO_WritePin(FLASH_ALE_GPIO_Port, FLASH_ALE_Pin, GPIO_PIN_SET);
#define CLE_LOW		HAL_GPIO_WritePin(FLASH_CLE_GPIO_Port, FLASH_CLE_Pin, GPIO_PIN_RESET);
#define CLE_HIGH 	HAL_GPIO_WritePin(FLASH_CLE_GPIO_Port, FLASH_CLE_Pin, GPIO_PIN_SET);

void delay(__IO uint32_t nCount)
{
   __IO uint32_t index = 0;

   for(index = (1000 * nCount); index != 0; index--)
   {
   }
}

void delayFast(__IO uint32_t nCount)
{
   __IO uint32_t index = 0;

   for(index = nCount; index != 0; index--)
   {
   }
}

void Flash_init(uint8_t* flashid)
{

	CE_HIGH;
	WE_HIGH;
	RE_HIGH;
	CLE_HIGH;
	ALE_HIGH;
	HAL_GPIO_WritePin(FLASH_WP_GPIO_Port, FLASH_WP_Pin, GPIO_PIN_SET);
	reset();
	flash_read_sig(flashid);
}


#define IS_BUSY	 !HAL_GPIO_ReadPin(FLASH_R_B_GPIO_Port, FLASH_R_B_Pin)

uint8_t flash_get_data(void)
{
	uint8_t tmp = 0;
	tmp |= HAL_GPIO_ReadPin(FLASH_IO0_GPIO_Port, FLASH_IO0_Pin);
	tmp |= HAL_GPIO_ReadPin(FLASH_IO1_GPIO_Port, FLASH_IO1_Pin)<<1;
	tmp |= HAL_GPIO_ReadPin(FLASH_IO2_GPIO_Port, FLASH_IO2_Pin)<<2;
	tmp |= HAL_GPIO_ReadPin(FLASH_IO3_GPIO_Port, FLASH_IO3_Pin)<<3;
	tmp |= HAL_GPIO_ReadPin(FLASH_IO4_GPIO_Port, FLASH_IO4_Pin)<<4;
	tmp |= HAL_GPIO_ReadPin(FLASH_IO5_GPIO_Port, FLASH_IO5_Pin)<<5;
	tmp |= HAL_GPIO_ReadPin(FLASH_IO6_GPIO_Port, FLASH_IO6_Pin)<<6;
	tmp |= HAL_GPIO_ReadPin(FLASH_IO7_GPIO_Port, FLASH_IO7_Pin)<<7;
	return tmp;
}

void in(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/*Configure GPIO pins : FLASH_IO0_Pin FLASH_IO1_Pin FLASH_IO3_Pin FLASH_IO4_Pin
	                           FLASH_IO5_Pin FLASH_IO6_Pin FLASH_IO7_Pin */
	GPIO_InitStruct.Pin = FLASH_IO0_Pin|FLASH_IO1_Pin|FLASH_IO3_Pin|FLASH_IO4_Pin
						  |FLASH_IO5_Pin|FLASH_IO6_Pin|FLASH_IO7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin : FLASH_IO2_Pin */
	GPIO_InitStruct.Pin = FLASH_IO2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(FLASH_IO2_GPIO_Port, &GPIO_InitStruct);
}

void out(uint8_t data)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/*Configure GPIO pins : FLASH_IO0_Pin FLASH_IO1_Pin FLASH_IO3_Pin FLASH_IO4_Pin
	                           FLASH_IO5_Pin FLASH_IO6_Pin FLASH_IO7_Pin */
	GPIO_InitStruct.Pin = FLASH_IO0_Pin|FLASH_IO1_Pin|FLASH_IO3_Pin|FLASH_IO4_Pin
						  |FLASH_IO5_Pin|FLASH_IO6_Pin|FLASH_IO7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin : FLASH_IO2_Pin */
	GPIO_InitStruct.Pin = FLASH_IO2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(FLASH_IO2_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(FLASH_IO0_GPIO_Port, FLASH_IO0_Pin, (data>>0)&0x01);
	HAL_GPIO_WritePin(FLASH_IO1_GPIO_Port, FLASH_IO1_Pin, (data>>1)&0x01);
	HAL_GPIO_WritePin(FLASH_IO2_GPIO_Port, FLASH_IO2_Pin, (data>>2)&0x01);
	HAL_GPIO_WritePin(FLASH_IO3_GPIO_Port, FLASH_IO3_Pin, (data>>3)&0x01);
	HAL_GPIO_WritePin(FLASH_IO4_GPIO_Port, FLASH_IO4_Pin, (data>>4)&0x01);
	HAL_GPIO_WritePin(FLASH_IO5_GPIO_Port, FLASH_IO5_Pin, (data>>5)&0x01);
	HAL_GPIO_WritePin(FLASH_IO6_GPIO_Port, FLASH_IO6_Pin, (data>>6)&0x01);
	HAL_GPIO_WritePin(FLASH_IO7_GPIO_Port, FLASH_IO7_Pin, (data>>7)&0x01);
}

void all_high(void)
{
	RE_HIGH;
	WE_HIGH;
	CE_HIGH;
	ALE_HIGH;
	CLE_HIGH;
}

void set_command(uint8_t command)
{
	CLE_HIGH;
	RE_HIGH;
	ALE_LOW;
	out(command);
	asm("mov r0,r0");
	WE_LOW;
	asm("mov r0,r0");
	WE_HIGH;

}

void set_address(uint8_t address)
{
	CLE_LOW;
	ALE_HIGH;
	RE_HIGH;
	out(address);
	asm("mov r0,r0");
	delay(FLASH_DELAY);
	//osDelay(FLASH_DELAY);
	WE_LOW;
	delay(FLASH_DELAY);
	//osDelay(FLASH_DELAY);
	asm("mov r0,r0");
	WE_HIGH;
}

void write_data(uint8_t data)
{
	CLE_LOW;
	ALE_LOW;
	RE_HIGH;
	out(data);
	asm("mov r0,r0");
	delay(FLASH_DELAY);
	//osDelay(FLASH_DELAY);
	WE_LOW;
	asm("mov r0,r0");
	delay(FLASH_DELAY);
	//osDelay(FLASH_DELAY);
	WE_HIGH;
}



void read_data(uint8_t *data)
{
	CLE_LOW;
	ALE_LOW;
	WE_HIGH;
	RE_LOW;
	asm("mov r0,r0");
	asm("mov r0,r0");
	delay(FLASH_DELAY);
	//osDelay(FLASH_DELAY);
	*data = flash_get_data();
	RE_HIGH;

}

uint8_t wait_for_busy(void)
{
    //while(!(PINB&0x04));
	//delayFast(50);
	//osDelay(1);
	while(IS_BUSY);
	return 1;
}

ulong flash_read_sig(uint8_t* flashid)
{
	uint8_t sig[4] = {0,};
	CE_LOW;
	set_command(0x90);
	set_address((uint8_t)0x00);			// Młodsza połowa adresu
	in();
	read_data(&sig[0]);
	read_data(&sig[1]);
	read_data(&sig[2]);
	read_data(&sig[3]);
	flashid[0] = sig[0];
	flashid[1] = sig[1];
	flashid[2] = sig[2];
	all_high();
	return *((ulong*)sig);
}

uint8_t check_status(void)
{
	uint8_t tmp1 = 0;
	do
	{
		set_command(0x70);								// Sprawdź status
		in();
		asm("mov r0,r0");
		asm("mov r0,r0");
		delay(FLASH_DELAY);
		//osDelay(FLASH_DELAY);
		read_data(&tmp1);
	}while(!(tmp1&0x40));
	if ( tmp1 & 0x01)
		return 0;									// Wyskoczył błąd kasowania bloku
	else
		return 1;									// OK blok skasowany
}

uint8_t reset(void)
{
	set_command(0xFF);								// Sprawdź status
	return (wait_for_busy());
}

// *******************************************
// * Funkcja kasuje zawartość bloku z flash  *
// * blok = 128 Kb = 64 strony				 *
// * zwraca 0 -> błąd						 *
// *		1 -> operacja się udała			 *
// *******************************************
uint8_t erase_block(uint16_t number_of_block)
{
	uint8_t result;
	CE_LOW;
	asm("mov r0,r0");
	set_command(0x60);								// Rozkaz "Erase block"
	set_address((uint8_t)(number_of_block << 6));			// Młodsza połowa adresu
	set_address((uint8_t)(number_of_block >> 2));		// Starsza połowa adresu
	set_command(0xD0);								// Potwierdzenie komendy
	if(!(wait_for_busy()))
	{
		all_high();
		return 0;				// Poczekaj 25us
	}
	result = check_status();
	all_high();
	return result;
}

// *******************************************
// * Funkcja czyta 1 bajt z flash 			 *
// * number_of_sector -> max 0x0003FFFF		 *
// * zwraca 0 -> błąd						 *
// *		1 -> operacja się udała			 *
// *******************************************
uint16_t page_read(ulong address, uint8_t *buffer, uint16_t lenght)
{
	uint16_t i, result = lenght;
	uint8_t temp;

	CE_LOW;
	asm("mov r0,r0");
	set_command(0x00);								// Rozkaz "Read sector"
	set_address((uint8_t)(address));					// Adres pierwszego bajtu w sektorze
	set_address((uint8_t)((address >> 8) & 0x07));	// Najmłodszy bajt adresu
	set_address((uint8_t)(address >> 11));		 	// Środkowy bajt adresu
	set_address((uint8_t)(address >> 19));		 	// Najstarszy bajt adresu
	set_command(0x30);								// Potwierdzenie komendy
	in();
	i = wait_for_busy();
	if(!(i))
	{
		all_high();
		return 0;									// Poczekaj 25us
	}
	for (i=0; i<lenght; i++)
	{
		if (((address%2048)+i)>2048)	//Koniec page
		{
			result = i-1;
			break;
		}
		read_data(&temp);
		*buffer = ~temp;
		buffer++;
	}
	all_high();

	return result;
}

uint8_t read_byte(ulong addr, uint8_t *_buf, uint16_t size)
{
	unsigned long i=0;
	if ((addr%2048) > 0)
	{
		i = page_read(addr, _buf, size);
		_buf+=i;
		size-=i;
		addr+=i;
		i=0;
	}
	if (size > 2048)
	{
		for(i = 0; i < (size/2048);i++)
		{
			page_read(addr+i*2048, _buf, 2048);
			_buf+=2048;
		}
	}
	else
		page_read(addr+i*2048, _buf, size-i*2048);

	return 1;

}


// **********************************************
// * Funkcja zapisuje sektor 2048 bajtów na flash *
// * address -> adres fizyczny				 	*
// * zwraca 0 -> błąd						 	*
// *		1 -> operacja się udała			 	*
// **********************************************
uint16_t program_page(ulong address, uint8_t *buffer, uint16_t lenght)
{
	uint16_t i;
	uint16_t result = lenght;
	uint8_t temp;

	/*
	 * A0 - A11 Column address
	 * A12 - A17 Page address
	 * A18 - A27 Block address
	 */
	CE_LOW;
	asm("mov r0,r0");
	set_command(0x80);								// Rozkaz "Write page"
	//set_address((uint8_t)(address));				// Młodszy bajt kolumny
	set_address(0x00);								// Młodszy bajt kolumny
	//set_address((uint8_t)((address >> 8) & 0x0F));	// Starszy bajt kolumny
	set_address(0x00);								// Starszy bajt kolumny
	set_address((uint8_t)(address >> 11)); 			// Młodszy bajt wiersza
	set_address((uint8_t)(address >> 19)); 			// Starszy bajt wiersza
	//nowe
	asm("mov r0,r0");
	set_command(0x85);								//Rozkaz "Sequential input"
	set_address((uint8_t)(address));				// Młodszy bajt kolumny
	set_address((uint8_t)((address >> 8) & 0x07));	// Starszy bajt kolumny
	//nowe
	out(0x00);
	for (i=0; i<lenght; i++)						// Bajty danych
	{
		if (((address%2048)+i)>2048)	//Koniec page
		{
			result = i-1;
			break;
		}
		temp = ~(*buffer);
		write_data(temp);
		buffer++;
	}
	set_command(0x10);								// Rozkaz "Program Command"
	in();
	if(!(wait_for_busy()))
	{
		all_high();
		return 0;									// Poczekaj 25us
	}

	//result = check_status();
	all_high();
	return result;
}

int
write_byte(const void *_buf, int size, unsigned long addr)

{
  unsigned long i=0;
  if ((addr%2048) > 0)
  {
	  if (size>2047){
		  i=1;
	  }
	i = program_page(addr, _buf, size);
	_buf+=i;
	size-=i;
	addr+=i;
	i=0;
  }
  if (size > 2048)
  {
	  for(i = 0; i < (size/2048);i++)
	  {
		program_page(addr+i*2048, _buf, 2048);
		_buf+=2048;
	  }
  }
  else
  {
	  program_page(addr+i*2048, _buf, size-i*2048);
  }

  return 1;
}














/*
// ******************************************************
// * Funkcja zapisuje zerami całą stronę			 	*
// * address -> adres fizyczny				 			*
// * zwraca 0 -> błąd						 			*
// *		1 -> operacja się udała			 			*
// ******************************************************
uint8_t write_zeroes(uint16_t page)
{
	uint16_t i;
	uint8_t result;

	CE_LOW;
	asm("mov r0,r0");
	set_command(0x80);								// Rozkaz "Write sector"
	set_address(0);									// Młodszy bajt kolumny
	set_address(0);									// Starszy bajt kolumny
	set_address((uint8_t)(page)); 					// Młodszy bajt wiersza
	set_address((uint8_t)(page >> 8)); 				// Starszy bajt wiersza
	out(0x00);
	for (i=0; i<5; i++)							// Bajty danych
	{
		write_data(0x00);
	}
	set_command(0x10);								// Rozkaz "Program Command"
	in();
	if(!(wait_for_busy()))
	{
		all_high();
		return 0;									// Poczekaj 25us
	}

	result = check_status();
	all_high();
	return result;
}
*/

/*
uint8_t xor_bits(uint8_t a,uint8_t mask)
{
	uint8_t ret = 0;

	if(mask&(0x01))
	{
		ret^=a>>0;
	}
	if(mask&(0x02))
	{
		ret^=a>>1;
	}
	if(mask&(0x04))
	{
		ret^=a>>2;
	}
	if(mask&(0x08))
	{
		ret^=a>>3;
	}
	if(mask&(0x10))
	{
		ret^=a>>4;
	}
	if(mask&(0x20))
	{
		ret^=a>>5;
	}
	if(mask&(0x40))
	{
		ret^=a>>6;
	}
	if(mask&(0x80))
	{
		ret^=a>>7;
	}

	return ret&0x01;
}

unsigned char tmp[18];

ulong ecc_calc(uint8_t* buff)
{
	uint8_t pall=0;
	ulong ecc = 0;
	uint8_t i;

    memset(tmp,0,18);

	for ( i=0;;i++)
	{
		pall^=*buff;

		if(i&0x01)
			tmp[1]^= *buff;
		else
			tmp[0]^= *buff;

		if(i&0x02)
			tmp[3]^= *buff;
		else
			tmp[2]^= *buff;

		if(i&0x04)
			tmp[5]^= *buff;
		else
			tmp[4]^= *buff;

		if(i&0x08)
			tmp[7]^= *buff;
		else
			tmp[6]^= *buff;

		if(i&0x10)
			tmp[9]^= *buff;
		else
			tmp[8]^= *buff;

		if(i&0x20)
			tmp[11]^= *buff;
		else
			tmp[10]^= *buff;

		if(i&0x40)
			tmp[13]^= *buff;
		else
			tmp[12]^= *buff;

		if(i&0x80)
			tmp[15]^= *buff;
		else
			tmp[14]^= *buff;

		tmp[16]^= *buff;

		buff++;

		if(i==0xFF) break;
	}

	for ( i=0;;i++,buff++)
	{
		pall^=*buff;

		if(i&0x01)
			tmp[1]^= *buff;
		else
			tmp[0]^= *buff;

		if(i&0x02)
			tmp[3]^= *buff;
		else
			tmp[2]^= *buff;

		if(i&0x04)
			tmp[5]^= *buff;
		else
			tmp[4]^= *buff;

		if(i&0x08)
			tmp[7]^= *buff;
		else
			tmp[6]^= *buff;

		if(i&0x10)
			tmp[9]^= *buff;
		else
			tmp[8]^= *buff;

		if(i&0x20)
			tmp[11]^= *buff;
		else
			tmp[10]^= *buff;

		if(i&0x40)
			tmp[13]^= *buff;
		else
			tmp[12]^= *buff;

		if(i&0x80)
			tmp[15]^= *buff;
		else
			tmp[14]^= *buff;

		tmp[17]^= *buff;

		if(i==0xFF) break;

	}

	ecc |= xor_bits(pall,0x55);
	ecc |= xor_bits(pall,0xAA)<<1;
	ecc |= xor_bits(pall,0x33)<<2;
	ecc |= xor_bits(pall,0xCC)<<3;
	ecc |= xor_bits(pall,0x0F)<<4;
	ecc |= xor_bits(pall,0xF0)<<5;

	for(i=0;i<18;i++)
	{
		ecc |= ((ulong)xor_bits(tmp[i],0xFF))<<(i+6);
	}
	return ecc;
}


uint8_t ecc_decode(uint8_t* buff,ulong ecc1)
{
	ulong ecc2,wynByte=0,wynBit=0,err=0;

	uint8_t d;
	ulong ecco1 = 0;
	ulong ecco2 = 0;

    ecc2 = ecc_calc(buff);

	err = ecc1^ecc2;
	if( err == 0 )
		return 1;
	else{

		for(d=0;d<24;d+=2)
		{
			if( (err&(1ul<<d)) == ((err>>1ul)&(1ul<<d)) )
				return 0;
		}

		for(d=0;d<24;d++)
		{
			if(d%2){
				ecco1 |= ((ecc1)&(1ul<<d))>>(d-(d/2));
				ecco2 |= ((ecc2)&(1ul<<d))>>(d-(d/2));
			}
		}

		wynBit = (ecco1^ecco2)&0x0007;
		wynByte = ((ecco1^ecco2)>>3);
        buff[wynByte]^=1<<wynBit;
		return 1;
	}
}

*/




#define TESTCOFFEE 1
#define DEBUG_CFS 1
#if TESTCOFFEE
#if DEBUG_CFS
#include <stdio.h>
#define PRINTF_CFS(...) printf(__VA_ARGS__)
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF_CFS(...)
#endif

#include "cfs.h"
#include "cfs-coffee.h"
//#include "lib/crc16.h"
//#include "lib/random.h"
#include <stdio.h>

#define FAIL(x) PRINTF("FAILED\n");error = (x); goto end;

#define FILE_SIZE 512

unsigned short
random_rand(void)
{
/* In gcc int rand() uses RAND_MAX and long random() uses RANDOM_MAX=0x7FFFFFFF */
/* RAND_MAX varies depending on the architecture */

  return (unsigned short)rand();
}

int
coffee1_file_test(void)
{
	uint16_t i;
	uint8_t tab[2048];
	int wfd;
	cfs_coffee_format();
	for (i=0; i<2048; i++)
	{
		tab[i]=i;
	}
	wfd = cfs_open("T1", CFS_WRITE | CFS_APPEND);
	cfs_write(wfd, tab, 2048);
	cfs_write(wfd, &tab[1], 200);
	cfs_close(wfd);
	wfd = cfs_open("T1",CFS_READ);
	memset(tab,0,2048);
	cfs_read(wfd, tab, 2048);
	memset(tab,0,2048);
	cfs_read(wfd, tab, 2048);
	return 1;
}

/*--------------------------------------------------------------------------*/
int
coffee_file_test(void)
{
  int error;
  int wfd, rfd, afd;
  unsigned char buf[256], buf2[11];
  int r, i, j, total_read;
  unsigned offset;

  cfs_coffee_format();

  cfs_remove("T1");
  cfs_remove("T2");
  cfs_remove("T3");
  cfs_remove("T4");
  cfs_remove("T5");
  wfd = rfd = afd = -1;

  for(r = 0; r < sizeof(buf); r++) {
    buf[r] = r;
  }

  PRINTF("TEST 1\n");

  /* Test 1: Open for writing. */
  wfd = cfs_open("T1", CFS_WRITE);
  if(wfd < 0) {
    FAIL(-1);
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("2\n");

  /* Test 2: Write buffer. */
  r = cfs_write(wfd, buf, sizeof(buf));
  if(r < 0) {
    FAIL(-2);
  } else if(r < sizeof(buf)) {
    FAIL(-3);
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("3\n");

  /* Test 3: Deny reading. */
  r = cfs_read(wfd, buf, sizeof(buf));
  if(r >= 0) {
    FAIL(-4);
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("4\n");

  /* Test 4: Open for reading. */
  rfd = cfs_open("T1", CFS_READ);
  if(rfd < 0) {
    FAIL(-5);
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("5\n");

  /* Test 5: Write to read-only file. */
  r = cfs_write(rfd, buf, sizeof(buf));
  if(r >= 0) {
    FAIL(-6);
  }
  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("7\n");

  /* Test 7: Read the buffer written in Test 2. */
  memset(buf, 0, sizeof(buf));
  r = cfs_read(rfd, buf, sizeof(buf));
  if(r < 0) {
    FAIL(-8);
  } else if(r < sizeof(buf)) {
    PRINTF_CFS("r=%d\n", r);
    FAIL(-9);
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("8\n");

  /* Test 8: Verify that the buffer is correct. */
  for(r = 0; r < sizeof(buf); r++) {
    if(buf[r] != r) {
      PRINTF_CFS("r=%d. buf[r]=%d\n", r, buf[r]);
      FAIL(-10);
    }
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("9\n");

  /* Test 9: Seek to beginning. */
  if(cfs_seek(wfd, 0, CFS_SEEK_SET) != 0) {
    FAIL(-11);
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("10\n");

  /* Test 10: Write to the log. */
  r = cfs_write(wfd, buf, sizeof(buf));
  if(r < 0) {
    FAIL(-12);
  } else if(r < sizeof(buf)) {
    FAIL(-13);
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("11\n");

  /* Test 11: Read the data from the log. */
  cfs_seek(rfd, 0, CFS_SEEK_SET);
  memset(buf, 0, sizeof(buf));
  r = cfs_read(rfd, buf, sizeof(buf));
  if(r < 0) {
    FAIL(-14);
  } else if(r < sizeof(buf)) {
    FAIL(-15);
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("12\n");

  /* Test 12: Verify that the data is correct. */
  for(r = 0; r < sizeof(buf); r++) {
    if(buf[r] != r) {
      FAIL(-16);
    }
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("13\n");

  /* Test 13: Write a reversed buffer to the file. */
  for(r = 0; r < sizeof(buf); r++) {
    buf[r] = sizeof(buf) - r - 1;
  }
  if(cfs_seek(wfd, 0, CFS_SEEK_SET) != 0) {
    FAIL(-17);
  }
  r = cfs_write(wfd, buf, sizeof(buf));
  if(r < 0) {
    FAIL(-18);
  } else if(r < sizeof(buf)) {
    FAIL(-19);
  }
  if(cfs_seek(rfd, 0, CFS_SEEK_SET) != 0) {
    FAIL(-20);
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("14\n");

  /* Test 14: Read the reversed buffer. */
  cfs_seek(rfd, 0, CFS_SEEK_SET);
  memset(buf, 0, sizeof(buf));
  r = cfs_read(rfd, buf, sizeof(buf));
  if(r < 0) {
    FAIL(-21);
  } else if(r < sizeof(buf)) {
    PRINTF_CFS("r = %d\n", r);
    FAIL(-22);
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("15\n");

  /* Test 15: Verify that the data is correct. */
  for(r = 0; r < sizeof(buf); r++) {
    if(buf[r] != sizeof(buf) - r - 1) {
    	FAIL(-23); 		//blad tego nie przechodzi
    }
  }

  cfs_close(rfd);
  cfs_close(wfd);

  if(cfs_coffee_reserve("T2", FILE_SIZE) < 0) {
    FAIL(-24);
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("16\n");

  /* Test 16: Test multiple writes at random offset. */
  for(r = 0; r < 100; r++) {
    wfd = cfs_open("T2", CFS_WRITE | CFS_READ);
    if(wfd < 0) {
      FAIL(-25);
    }
    offset = random_rand() % FILE_SIZE;
    for(r = 0; r < sizeof(buf); r++) {
      buf[r] = r;
    }
    if(cfs_seek(wfd, offset, CFS_SEEK_SET) != offset) {
      FAIL(-26);
    }
    if(cfs_write(wfd, buf, sizeof(buf)) != sizeof(buf)) {
      FAIL(-27);
    }
    if(cfs_seek(wfd, offset, CFS_SEEK_SET) != offset) {
      FAIL(-28);
    }
    memset(buf, 0, sizeof(buf));
    if(cfs_read(wfd, buf, sizeof(buf)) != sizeof(buf)) {
      FAIL(-29);
    }
    for(i = 0; i < sizeof(buf); i++) {
      if(buf[i] != i) {
        PRINTF_CFS("buf[%d] != %d\n", i, buf[i]);
        FAIL(-30);
      }
    }
  }
  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("17\n");

  /* Test 17: Append data to the same file many times. */
#define APPEND_BYTES 3000
#define BULK_SIZE 10
  for(i = 0; i < APPEND_BYTES; i += BULK_SIZE) {
    afd = cfs_open("T3", CFS_WRITE | CFS_APPEND);
    if(afd < 0) {
      FAIL(-31);
    }
    for(j = 0; j < BULK_SIZE; j++) {
      buf[j] = 1 + ((i + j) & 0x7f);
    }
    if((r = cfs_write(afd, buf, BULK_SIZE)) != BULK_SIZE) {
      PRINTF_CFS("Count:%d, r=%d\n", i, r);
      FAIL(-32);
    }
    cfs_close(afd);
  }

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("18\n");

  /* Test 18: Read back the data written in Test 17 and verify. */
  afd = cfs_open("T3", CFS_READ);
  if(afd < 0) {
    FAIL(-33);
  }
  total_read = 0;
  while((r = cfs_read(afd, buf2, sizeof(buf2))) > 0) {
    for(j = 0; j < r; j++) {
      if(buf2[j] != 1 + ((total_read + j) & 0x7f)) {
        FAIL(-34);
      }
    }
    total_read += r;
  }
  if(r < 0) {
    PRINTF_CFS("FAIL:-35 r=%d\n", r);
    FAIL(-35);
  }
  if(total_read != APPEND_BYTES) {
    PRINTF_CFS("FAIL:-35 total_read=%d\n", total_read);
    FAIL(-35);
  }
  cfs_close(afd);

  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("19\n");

  /* T4 */
  /*
   * file T4 and T5 writing forces to use garbage collector in greedy mode
   * this test is designed for 10kb of file system
   */
#define APPEND_BYTES_1 2000
#define BULK_SIZE_1 10
  for(i = 0; i < APPEND_BYTES_1; i += BULK_SIZE_1) {
    afd = cfs_open("T4", CFS_WRITE | CFS_APPEND);
    if(afd < 0) {
      FAIL(-36);
    }
    for(j = 0; j < BULK_SIZE_1; j++) {
      buf[j] = 1 + ((i + j) & 0x7f);
    }

    if((r = cfs_write(afd, buf, BULK_SIZE_1)) != BULK_SIZE_1) {
      PRINTF_CFS("Count:%d, r=%d\n", i, r);
      FAIL(-37);
    }
    cfs_close(afd);
  }

  afd = cfs_open("T4", CFS_READ);
  if(afd < 0) {
    FAIL(-38);
  }

  total_read = 0;
  while((r = cfs_read(afd, buf2, sizeof(buf2))) > 0) {
    for(j = 0; j < r; j++) {
      if(buf2[j] != 1 + ((total_read + j) & 0x7f)) {
        PRINTF_CFS("FAIL:-39, total_read=%d r=%d\n", total_read, r);
        FAIL(-39);
      }
    }
    total_read += r;
  }
  if(r < 0) {
    PRINTF_CFS("FAIL:-40 r=%d\n", r);
    FAIL(-40);
  }
  if(total_read != APPEND_BYTES_1) {
    PRINTF_CFS("FAIL:-41 total_read=%d\n", total_read);
    FAIL(-41);
  }
  cfs_close(afd);

  /* T5 */
  PRINTF("PASSED\n");
  PRINTF("TEST ");
  PRINTF("20\n");
#define APPEND_BYTES_2 1000
#define BULK_SIZE_2 10
  for(i = 0; i < APPEND_BYTES_2; i += BULK_SIZE_2) {
    afd = cfs_open("T5", CFS_WRITE | CFS_APPEND);
    if(afd < 0) {
      FAIL(-42);
    }
    for(j = 0; j < BULK_SIZE_2; j++) {
      buf[j] = 1 + ((i + j) & 0x7f);
    }

    if((r = cfs_write(afd, buf, BULK_SIZE_2)) != BULK_SIZE_2) {
      PRINTF_CFS("Count:%d, r=%d\n", i, r);
      FAIL(-43);
    }

    cfs_close(afd);
  }

  afd = cfs_open("T5", CFS_READ);
  if(afd < 0) {
    FAIL(-44);
  }
  total_read = 0;
  while((r = cfs_read(afd, buf2, sizeof(buf2))) > 0) {
    for(j = 0; j < r; j++) {
      if(buf2[j] != 1 + ((total_read + j) & 0x7f)) {
        PRINTF_CFS("FAIL:-45, total_read=%d r=%d\n", total_read, r);
        FAIL(-45);
      }
    }
    total_read += r;
  }
  if(r < 0) {
    PRINTF_CFS("FAIL:-46 r=%d\n", r);
    FAIL(-46);
  }
  if(total_read != APPEND_BYTES_2) {
    PRINTF_CFS("FAIL:-47 total_read=%d\n", total_read);
    FAIL(-47);
  }
  cfs_close(afd);

  PRINTF("PASSED\n");

  error = 0;
end:
  cfs_close(wfd);
  cfs_close(rfd);
  cfs_close(afd);
  return error;
}
#endif /* TESTCOFFEE */
