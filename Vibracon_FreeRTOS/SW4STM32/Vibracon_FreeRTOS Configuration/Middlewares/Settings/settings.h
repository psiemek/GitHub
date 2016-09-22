/*
 * settings.h
 *
 *  Created on: Aug 5, 2014
 *      Author: user
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

typedef struct  _settings
{
	unsigned char dhcp_enable;
	uip_ipaddr_t ip_address;
	uip_ipaddr_t ip_mask;
	uip_ipaddr_t gate_address;
	struct uip_eth_addr default_uip_ethaddr;
	unsigned int modbus_port;
	unsigned int modbus_address;
}__settings;

void restore_to_default(void);
void restore_settings(void);
void save_settings(void);
unsigned long get_hw_number(void);

#endif /* SETTINGS_H_ */
