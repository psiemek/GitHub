#include "http_cgi.h"
#include "stdlib.h"
#include "string.h"

//#include "setget.h"
#include "utilities.h"
#include "port.h"

/*variable to hold if a reset must be issued*/
uint8 reset_var = FALSE;
/*reusing buffers for CGI processing*/
UINT8 *u8Archivo;

/*list of cgi's for this process*/
CGI_CMD CGI_CMD_ARRAY[]  = 
{
  CGI_RESET_CONFIGURATION
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
     return NULL;
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
