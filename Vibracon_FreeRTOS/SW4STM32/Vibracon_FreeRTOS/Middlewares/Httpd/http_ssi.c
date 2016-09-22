#include "http_ssi.h"
#include "stdlib.h"
#include "stdint.h"

#include "utilities.h"
//#include "netif.h"


//extern uint8_t IP_ADDRESS[4];
//extern uint8_t NETMASK_ADDRESS[4];
//extern uint8_t GATEWAY_ADDRESS[4];
//extern struct netif gnetif;

struct _settings
{
	uint8_t ip[4];
	uint8_t netmask[4];
	uint8_t gateway[4];
	uint8_t macaddress[6];
	uint16_t modbusTCPport;
	uint8_t modbusaddress;
	uint8_t checksum;
};
extern struct _settings settings;

/*shared variable*/
uint8 default_language;

SSI_CMD SSI_CMD_ARRAY[]  = 
{
  IPADDRESS,
  NETMASK,
  GATEWAY,
  MACADDRESS,
  MODBUSPORT,
  MODBUSADDR

};

LANGUAGES WEB_LANGUAGES[] =
{
  ENGLISH_OPTION,
  FRENCH_OPTION,  
  DEUTCH_OPTION,  
  CHINESSE_OPTION,
  ITALIAN_OPTION, 
  RUSSIAN_OPTION, 
  JAPANESE_OPTION,
  ARABIC_OPTION,  
  TAIWAN_OPTION,  
  PORTUGAL_OPTION,
  POLISH_OPTION,  
  SPANISH_OPTION, 
  KOREAN_OPTION   
};

/*FSL:sprintf prototype*/
//INT sprintf(CHAR *, const CHAR *, ... );

/**
 * Implements a SSI Replacement
 *
 * @param array to replace
 * @param replaced array 
 * @return returned value by function linked to array to replace
 */
uint8 
SSI_parser(uint8 *input, uint8 *output)
{
     uint8 i;

     for(i=0;i<SSI_MAX_COMMANDS;i++)
     {
        if(!strncmp((const CHAR *)input,(const CHAR *)SSI_CMD_ARRAY[i].command,strlen(SSI_CMD_ARRAY[i].command)))//contains array
        {
            return SSI_CMD_ARRAY[i].func(output);//execute function if matches
        }
     }
     //SSI: no replacement was found!!!
     return NULL;
}

//***************************************************************************//

/*inline */uint8 
max_number_of_languages()
{
  return MAX_LANGUAGES;
}

INT
send_ipaddress(void *var)
{

	//sprintf(var,"%d.%d.%d.%d",IP_ADDRESS[0],IP_ADDRESS[1],IP_ADDRESS[2],IP_ADDRESS[3]);
	sprintf(var,"%d.%d.%d.%d",settings.ip[0],settings.ip[1],settings.ip[2],settings.ip[3]);
	return 1;
}

INT
send_netmask(void *var)
{

	sprintf(var,"%d.%d.%d.%d",settings.netmask[0],settings.netmask[1],settings.netmask[2],settings.netmask[3]);

	return 1;
}

INT
send_gateway(void *var)
{

	sprintf(var,"%d.%d.%d.%d",settings.gateway[0],settings.gateway[1],settings.gateway[2],settings.gateway[3]);

	return 1;
}

INT
send_macaddress(void *var)
{
	//struct netif netif = gnetif;

	//sprintf(var,"%02X.%02X.%02X.%02X.%02X.%02X", netif.hwaddr[0],netif.hwaddr[1],netif.hwaddr[2],netif.hwaddr[3], netif.hwaddr[4], netif.hwaddr[5]);
	sprintf(var,"%02X.%02X.%02X.%02X.%02X.%02X", settings.macaddress[0],settings.macaddress[1],settings.macaddress[2],settings.macaddress[3], settings.macaddress[4], settings.macaddress[5]);

	return 1;
}

INT
send_modbusport(void *var)
{

	sprintf(var,"502");

	return 1;
}

INT
send_modbusaddr(void *var)
{

	sprintf(var,"%d",0);
	//sprintf(var,"%s",counter1++);

	return 1;
}

