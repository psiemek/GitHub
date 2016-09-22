#ifndef WEB_CGI_H
#define WEB_CGI_H

#include "basictypes.h"

/*
 * The command table entry data structure
 */
typedef const struct
{
    CHAR *  command;                /* CGI string id  */
    INT    (*func)(CHAR *, void *); /* actual function to call */
} CGI_CMD;

/*FSL:prototypes*/

/*FSL: extern functions*/

INT
reset_configuration_process(CHAR *name, void *request);
INT
save_configuration_process(CHAR *name, void *request);


/*
 * Macros for User InterFace command table entries
 */

#ifndef CGI_RESET_CONFIGURATION
#define CGI_RESET_CONFIGURATION    \
    {"reset.cgi",reset_configuration_process}
#endif

#ifndef CGI_SAVE_SETTINGS
#define CGI_SAVE_SETTINGS    \
    {"save.cgi",save_configuration_process}
#endif

#define CGI_MAX_COMMANDS       		sizeof(CGI_CMD_ARRAY)/sizeof(CGI_CMD )

/**
 * Implements a CGI call
 *
 * @param CGI name
 * @param argument to be used for CGI called function 
 * @return returned value by function linked to execute by POST request
 */
uint8 
CGI_parser(int8 *name, void *request);

#endif
