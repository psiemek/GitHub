#include "http_cgi.h"
#include "stdlib.h"
#include "string.h"

//#include "setget.h"
#include "utilities.h"
#include "port.h"


struct _settings
{
	uint8 ip[4];
	uint8 netmask[4];
	uint8 gateway[4];
	uint8 macaddress[6];
	uint16 modbusTCPport;
	uint8 modbusaddress;
	uint8 checksum;
};

extern void WriteSettings(void);

extern struct _settings settings;

/*variable to hold if a reset must be issued*/
uint8 reset_var = FALSE;
/*reusing buffers for CGI processing*/
UINT8 *u8Archivo;

/*list of cgi's for this process*/
CGI_CMD CGI_CMD_ARRAY[]  = 
{
  CGI_RESET_CONFIGURATION,
  CGI_SAVE_SETTINGS
};

/**
 * Implements a CGI call
 *
 * @param CGI name
 * @param argument to be used for CGI called function 
 * @return returned value by function linked to execute by POST request
 */
uint8 
CGI_parser(int8 *name, void *request)
{
     uint8 i;

     for(i=0;i<CGI_MAX_COMMANDS;i++)
     {
        if(!strncmp((const CHAR *)name,(const CHAR *)CGI_CMD_ARRAY[i].command,strlen(CGI_CMD_ARRAY[i].command)))//contains array
        {
            return CGI_CMD_ARRAY[i].func((CHAR *)name,request);//execute function if matches
        }
     }
     //CGI: no replacement was found!!!
     return 0;
}

//***************************************************************************//

INT
reset_configuration_process(CHAR *name, void *request)
{
    /*stop watchdog, let's see what happens*/
    (void)request;

    /*file for POST request*/
    strcpy(name,"X");
    reset_var = 1;
    
    return 1;
}
INT
save_configuration_process(CHAR *name, void *request)
{
    /*FSL:start configuration processing*/
    uint8 *string;
    uint8 temp[18];


    /*variable holding ROM info to be write*/

    /*search for string on linked list: reusing SD buffer*/
    if((string = search_string_linked_list(request, u8Archivo, (uint8 *)STRING_DOUBLE_END, NULL)) != NULL)
    {
        /*parse string*/

    	//ip address
		parse_mac_ip_address_string((uint8 **)&string,(uint8 *)&temp,'=','&');
		if(temp[0] != 0)/*only write if different to NULL*/
		  ip_convert_address(temp,(CHAR *)&settings.ip[0]);
		//mask address
		parse_mac_ip_address_string((uint8 **)&string,(uint8 *)&temp,'=','&');
		if(temp[0] != 0)/*only write if different to NULL*/
		  ip_convert_address(temp,(CHAR *)&settings.netmask[0]);
		//gateway address
		parse_mac_ip_address_string((uint8 **)&string,(uint8 *)&temp,'=','&');
		if(temp[0] != 0)/*only write if different to NULL*/
		  ip_convert_address(temp,(CHAR *)&settings.gateway[0]);
		//mac address
		parse_mac_ip_address_string((uint8 **)&string,(uint8 *)&temp,'=','&');
		if(temp[0] != 0)/*only write if different to NULL*/
		  parse_ethaddr(temp,&settings.macaddress[0]);
		//Modbus TCP Port
		settings.modbusTCPport = (uint16)parse_number((uint8 **)&string,'=',10);
		//Modbus Address
		settings.modbusaddress = (uint8)parse_number((uint8 **)&string,'=',10);


        WriteSettings();
    }

    /*file for POST request*/
    strcpy(name,"INDEX.SHTML");

    return 1;
}
