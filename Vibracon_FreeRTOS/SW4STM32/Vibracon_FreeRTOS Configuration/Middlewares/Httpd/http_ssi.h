#ifndef WEB_SSI_H
#define WEB_SSI_H

#include "basictypes.h"

#define SSI_START         "<!--#echo var=\""
#define SSI_END           "\"-->"

#define MAC_ADDRESS_STRING "%02X.%02X.%02X.%02X.%02X.%02X"
#define IP_ADDRESS_STRING  "%d.%d.%d.%d"

/*
 * The command table entry data structure
 */
typedef const struct
{
    CHAR *  command;                /* SSI string to replace  */
    INT    (*func)(void *);         /* actual function to call */
} SSI_CMD;

typedef const struct
{
    CHAR *  language;
    CHAR *  language_code;
} LANGUAGES;

/*inline */uint8 
max_number_of_languages();

/*FSL: external functions*/
extern INT adc_channel0(void *var);

 /* Macros for User InterFace command table entries
 */

#ifndef SSI_CMD_CHANNEL0
#define SSI_CMD_CHANNEL0    \
    {"CHANNEL0",adc_channel0}
#endif

#define SSI_MAX_COMMANDS       		sizeof(SSI_CMD_ARRAY)/sizeof(SSI_CMD )

//languages
#define GREETING_ENGLISH        "Hello!!"
#define GREETING_FRENCH    		  "Salut!!"
#define GREETING_DEUTCH         "Hallo!!"
#define GREETING_CHINESSE       "China"
#define GREETING_ITALIAN        "Chau!!"
#define GREETING_RUSSIAN        "Russia"
#define GREETING_JAPANESE       "Japan"
#define GREETING_ARABIC         "Arabic"
#define GREETING_TAIWAN         "Taiwan"
#define GREETING_PORTUGAL       "Portugal"
#define GREETING_POLISH         "Polish"
#define GREETING_SPANISH        "Hola!!"
#define GREETING_KOREAN         "Korean"

//language codes
#define GREETING_ENGLISH_CODE   "en"
#define GREETING_FRENCH_CODE    "fr"
#define GREETING_DEUTCH_CODE    "de"
#define GREETING_CHINESSE_CODE  "zh"
#define GREETING_ITALIAN_CODE   "it"
#define GREETING_RUSSIAN_CODE   "ru"
#define GREETING_JAPANESE_CODE  "ja"
#define GREETING_ARABIC_CODE    "ar"
#define GREETING_TAIWAN_CODE    "tw"
#define GREETING_PORTUGAL_CODE  "pt"
#define GREETING_POLISH_CODE    "pl"
#define GREETING_SPANISH_CODE   "es"
#define GREETING_KOREAN_CODE    "ko"

#define GREETING_LANGUAGE_CODE_LENGTH  sizeof(GREETING_ENGLISH_CODE)-1

//language structs
#ifndef ENGLISH_OPTION
#define ENGLISH_OPTION    \
    {GREETING_ENGLISH,GREETING_ENGLISH_CODE}
#endif
#ifndef FRENCH_OPTION
#define FRENCH_OPTION    \
    {GREETING_FRENCH,GREETING_FRENCH_CODE}
#endif
#ifndef DEUTCH_OPTION
#define DEUTCH_OPTION    \
    {GREETING_DEUTCH,GREETING_DEUTCH_CODE}
#endif
#ifndef CHINESSE_OPTION
#define CHINESSE_OPTION    \
    {GREETING_CHINESSE,GREETING_CHINESSE_CODE}
#endif
#ifndef ITALIAN_OPTION
#define ITALIAN_OPTION    \
    {GREETING_ITALIAN,GREETING_ITALIAN_CODE}
#endif
#ifndef RUSSIAN_OPTION
#define RUSSIAN_OPTION    \
    {GREETING_RUSSIAN,GREETING_RUSSIAN_CODE}
#endif
#ifndef JAPANESE_OPTION
#define JAPANESE_OPTION    \
    {GREETING_JAPANESE,GREETING_JAPANESE_CODE}
#endif
#ifndef ARABIC_OPTION
#define ARABIC_OPTION    \
    {GREETING_ARABIC,GREETING_ARABIC_CODE}
#endif
#ifndef TAIWAN_OPTION
#define TAIWAN_OPTION    \
    {GREETING_TAIWAN,GREETING_TAIWAN_CODE}
#endif
#ifndef PORTUGAL_OPTION
#define PORTUGAL_OPTION    \
    {GREETING_PORTUGAL,GREETING_PORTUGAL_CODE}
#endif
#ifndef POLISH_OPTION
#define POLISH_OPTION    \
    {GREETING_POLISH,GREETING_POLISH_CODE}
#endif
#ifndef SPANISH_OPTION
#define SPANISH_OPTION    \
    {GREETING_SPANISH,GREETING_SPANISH_CODE}
#endif
#ifndef KOREAN_OPTION
#define KOREAN_OPTION    \
    {GREETING_KOREAN,GREETING_KOREAN_CODE}
#endif

#ifndef MAX_LANGUAGES
  #define MAX_LANGUAGES        sizeof(WEB_LANGUAGES)/sizeof(LANGUAGES)
#endif

/*FSL:prototypes*/
/**
 * Implements a SSI Replacement
 *
 * @param array to replace
 * @param replaced array 
 * @return returned value by function linked to array to replace
 */
uint8 
SSI_parser(uint8 *input, uint8 *output);

#endif
