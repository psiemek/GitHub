/*
 * settings.c
 *
 *  Created on: Aug 5, 2014
 *      Author: user
 */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "lwip.h"


#include "cfs.h"
#include "cfs-coffee-arch.h"
#include "settings.h"

__settings settings;

void restore_to_default(void)
{
	int file;
	settings.dhcp_enable = 0;
	uip_ipaddr(&settings.ip_address, 192,168,1,30);
	uip_ipaddr(&settings.gate_address, 192,168,1,1);
	uip_ipaddr(&settings.ip_mask, 255,255,255,0);
	settings.default_uip_ethaddr.addr[0]=0x02;
	settings.default_uip_ethaddr.addr[1]=0x00;
	settings.default_uip_ethaddr.addr[2]=0x00;
	//settings.default_uip_ethaddr.addr[3]=0x00;
	//settings.default_uip_ethaddr.addr[4]=0x00;
	//settings.default_uip_ethaddr.addr[5]=0x02;
	settings.default_uip_ethaddr.addr[3] = (unsigned char)(get_hw_number()>>16);
	settings.default_uip_ethaddr.addr[4] = (unsigned char)(get_hw_number()>>8);
	settings.default_uip_ethaddr.addr[5] = (unsigned char)(get_hw_number());

	settings.modbus_port = 502;
	settings.modbus_address = 1;
	cfs_remove("settings.cfg");
	file = cfs_open("settings.cfg", CFS_WRITE);
	cfs_write(file, &settings.dhcp_enable, sizeof(settings));
	NVIC_SystemReset();
}

void restore_settings(void)
{
	//cfs_coffee_format();
	int file;
	file = cfs_open("settings.cfg", CFS_READ);
	if (file != -1)
	{
		//cfs_remove("settings.cfg");
		cfs_read(file, &settings.dhcp_enable, sizeof(settings));
	}
	else
	{
		//Zrob defaultowe ustawienia
		restore_to_default();
	}
}

void save_settings(void)
{
	int file;
	cfs_remove("settings.cfg");
	file = cfs_open("settings.cfg", CFS_WRITE);
	cfs_write(file, &settings.dhcp_enable, sizeof(settings));
}

unsigned long get_hw_number(void)
{
	unsigned long  *unique = (unsigned long *)0x1FFFF7F0;
	return *unique;
}