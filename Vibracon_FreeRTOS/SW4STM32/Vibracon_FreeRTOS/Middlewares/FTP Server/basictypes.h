/*
 * basictypes.h
 *
 *  Created on: 5 sty 2016
 *      Author: HP
 */

#ifndef MIDDLEWARES_FTP_SERVER_BASICTYPES_H_
#define MIDDLEWARES_FTP_SERVER_BASICTYPES_H_

/***********************************************************************/
/*
 * The basic data types
 */

typedef unsigned char		uint8;  /*  8 bits */
typedef unsigned short int	uint16; /* 16 bits */
typedef unsigned long int	uint32; /* 32 bits */

typedef signed char			int8;   /*  8 bits */
typedef signed short int	int16;  /* 16 bits */
typedef signed long int		int32;  /* 32 bits */

typedef volatile uint8		vuint8;  /*  8 bits */
typedef volatile uint16		vuint16; /* 16 bits */
typedef volatile uint32		vuint32; /* 32 bits */

/*FSL: more basic types*/

typedef char CHAR;
typedef unsigned char UCHAR;
typedef signed char SCHAR;
typedef int INT;
typedef unsigned int UINT;

/*FSL: even more typedefs!*/
/* Typedefs */
typedef unsigned char     UINT8;  		/*unsigned 8 bit definition */
typedef unsigned short    UINT16; 		/*unsigned 16 bit definition*/
typedef unsigned long     UINT32; 		/*unsigned 32 bit definition*/
typedef signed char       INT8;   		/*signed 8 bit definition */
typedef short      		    INT16;  		/*signed 16 bit definition*/
typedef long int    	    INT32;  		/*signed 32 bit definition*/

/* TypeDefs */
typedef union
{
	UINT8  bytes[4];
	UINT32 lword;
}T32_8;

typedef union
{
	UINT8  u8[2];
	UINT16 u16;
}T16_8;


#endif /* MIDDLEWARES_FTP_SERVER_BASICTYPES_H_ */
