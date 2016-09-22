#include "http_ssi.h"
#include "stdlib.h"

#include "utilities.h"

/*shared variable*/
uint8 default_language;

SSI_CMD SSI_CMD_ARRAY[]  = 
{
  SSI_CMD_CHANNEL0,

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
adc_channel0(void *var)
{    
    static uint8 counter1 = 0;
    
    sprintf(var,"%d",counter1++);
    
    return 1;
}
