/******************************************************************************
*                                                  
*  (c) copyright Freescale Semiconductor 2009
*  ALL RIGHTS RESERVED
*
*  File Name:   http_server.c
*                                                                          
*  Description: HTTP Server 
*                                                                              
*  Assembler:   Codewarrior for V1 V6.2.1
*                                            
*  Version:     1.1                                           
*                                                                              
*  Location:    Guadalajara,Mexico                                             
*                                                                              
*                                                  
* UPDATED HISTORY:
*
* REV   YYYY.MM.DD  AUTHOR        DESCRIPTION OF CHANGE
* ---   ----------  ------        --------------------- 
* 1.0   2009.05.19  Mr Alcantara  HTTP 2.0 server released
* 1.1   2009.06.10  Mr Alcantara  Changes to a lower RAM usage
* 
******************************************************************************/                  
/* Freescale  is  not  obligated  to  provide  any  support, upgrades or new */
/* releases  of  the Software. Freescale may make changes to the Software at */
/* any time, without any obligation to notify or provide updated versions of */
/* the  Software  to you. Freescale expressly disclaims any warranty for the */
/* Software.  The  Software is provided as is, without warranty of any kind, */
/* either  express  or  implied,  including, without limitation, the implied */
/* warranties  of  merchantability,  fitness  for  a  particular purpose, or */
/* non-infringement.  You  assume  the entire risk arising out of the use or */
/* performance of the Software, or any systems you design using the software */
/* (if  any).  Nothing  may  be construed as a warranty or representation by */
/* Freescale  that  the  Software  or  any derivative work developed with or */
/* incorporating  the  Software  will  be  free  from  infringement  of  the */
/* intellectual property rights of third parties. In no event will Freescale */
/* be  liable,  whether in contract, tort, or otherwise, for any incidental, */
/* special,  indirect, consequential or punitive damages, including, but not */
/* limited  to,  damages  for  any loss of use, loss of time, inconvenience, */
/* commercial loss, or lost profits, savings, or revenues to the full extent */
/* such  may be disclaimed by law. The Software is not fault tolerant and is */
/* not  designed,  manufactured  or  intended by Freescale for incorporation */
/* into  products intended for use or resale in on-line control equipment in */
/* hazardous, dangerous to life or potentially life-threatening environments */
/* requiring  fail-safe  performance,  such  as  in the operation of nuclear */
/* facilities,  aircraft  navigation  or  communication systems, air traffic */
/* control,  direct  life  support machines or weapons systems, in which the */
/* failure  of  products  could  lead  directly to death, personal injury or */
/* severe  physical  or  environmental  damage  (High  Risk Activities). You */
/* specifically  represent and warrant that you will not use the Software or */
/* any  derivative  work of the Software for High Risk Activities.           */
/* Freescale  and the Freescale logos are registered trademarks of Freescale */
/* Semiconductor Inc.                                                        */ 
/*****************************************************************************/

/* ------------------------ FreeRTOS includes ----------------------------- */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* ------------------------ lwIP includes --------------------------------- */
#include "api.h"

/* ------------------------ Project includes ------------------------------ */
#include "cfs.h"         /* Coffe flash file system */

/* ------------------------ Project includes ------------------------------ */
#include "http_server.h"
#include "http_ssi.h"
#include "http_cgi.h"
#include "utilities.h"
#include "port.h"
#include "string.h"
#include "mem.h"



/* ------------------------ External Variables ----------------------------- */
extern const CHAR *InternalMemoryFilenames[];
extern const uint8 *InternalMemoryPointers[];
extern const uint16 InternalMemoryLengths[];
extern const uint8 number_of_files;
extern const uint8 index_htm[];/*FSL: default webpage*/
/*shared variable between CGI and HTTP request*/
extern LANGUAGES WEB_LANGUAGES[];
extern uint8 default_language;
extern uint8 reset_var;

#if HTTP_SD_CARD_SUPPORT 
/*Handle SD card*/
//FATHandler *SD_Web_Handle;
#endif

/*Read Buffer for App*/  
extern UINT8 *u8Archivo;

//#define WEB_SD_MUTEX_ENTER   xSemaphoreTake((xSemaphoreHandle)SD_Web_Handle->FAT_Mutex, portMAX_DELAY);
//#define WEB_SD_MUTEX_EXIT    xSemaphoreGive((xSemaphoreHandle)SD_Web_Handle->FAT_Mutex);

/*FSL:sprintf prototype*/
//INT sprintf(CHAR *, const CHAR *, ... );

/********************Private Functions ***************************************/

/**
 * Handle each request: timeout, close or request: HTTP 2.0 compliant
 *
 * @param connection descriptor
 * @param temporal buffer to be used for internal HTTP server
 * @return zero if session must be kept, otherwise session will be closed
 */
uint8
HTTP_ProcessConnection( web_session *connfd_web, uint8 *space )
{
  /*pointer to connection*/
  struct netconn *connfd;
  /*receiver buffers*/
  struct netbuf *inbuf;
  struct pbuf *q;
  
  /*connection assignment*/
  connfd = (struct netconn *)connfd_web->connection;

  /*if reception is OK: wait for REQUEST from client: request can be so longer than 700 bytes, for example*/
  netconn_rcv_req((void *)connfd, NULL, (void **)&inbuf, NETCONN_RCV_NETBUFFER);
    
  if( connfd->last_err == ERR_OK )
  {              
      /*reset timeout counter*/
      connfd_web->timeout = HTTP_WEB_TIMEOUT;
      /*get network buffer*/
      q = inbuf->ptr;
      
      //is this a GET method: assumig GET or POST id is at the first pbuf packet
      if( strstr((const CHAR*)q->payload,HTTP_GET_REQUEST) != NULL )
      {
         HTTP_ProcessRequest(connfd,(void *)q,space, GET_FLAG);
      }
      else if( strstr((const CHAR*)q->payload,HTTP_POST_REQUEST) != NULL )
      {
         HTTP_ProcessRequest(connfd,(void *)q,space, POST_FLAG);
         
         /*reset request processing*/
         if( reset_var == TRUE )
         {
            /*client closing the session*/
            netconn_close(connfd);
            netconn_delete(connfd);
            
            /*issue a MCU reset command*/
            //MCU_reset();
         }
      }
      else
      {
         /*FSL:not supported request*/
         HTTP_SendError(connfd);
         printf("HTTPD: Not supported request\n");
      }
      /*free pbuf memory*/
      netbuf_delete(inbuf);
  }
  /*session: tracking timeout*/
  else if( connfd->last_err == ERR_TIMEOUT )
  {
     if( !(connfd_web->timeout--) )
     {
        /*client closing the session*/
        netconn_close(connfd);
        netconn_delete(connfd);
        printf("HTTPD: Client closing the session\n");
  
        /*close the session*/
        return 1;      
     }
  }
  /*out of memory or session closed*/
  else
  {
      /*client closing the session*/
      netconn_close(connfd);
      netconn_delete(connfd);
      printf("HTTPD: Out of memory or session closed\n");
      /*close the session!!*/
      return 1;    
  }
  
  /*keeping the session*/
  return 0;
}

/**
 * Implements a HTTP GET or POST requests: send web page per request
 *   Note: no case sensitive
 *
 * @param connection descriptor
 * @param HTTP request
 * @param temporal buffer 
 * @param GET or POST request 
 * @return none
 */
void
HTTP_ProcessRequest(struct netconn *pcb, void *request, uint8 *temporal_buffer, uint8 flag)
{
     /*parsing variables*/
     CHAR *end_temp;
     CHAR *buffer;
     UINT16 u16Length;
     /*joining temp pbuf*/
     struct pbuf *q;
  
     q = (struct pbuf *)request;
  
     /*pointing to a new buffer*/
     if( flag == GET_FLAG )
     {
        buffer = (CHAR *)((uint32)q->payload + (uint32)(str_len(HTTP_GET_REQUEST)));
     }
     else if( flag == POST_FLAG )
     {
        buffer = (CHAR *)((uint32)q->payload + (uint32)(str_len(HTTP_POST_REQUEST)));
     }
  
     //looking for file's name: assuming name's at first linked-list packet
     if((end_temp = (CHAR *)strchr((const CHAR *)buffer,' ')) != NULL)
     {
          if((u16Length = (uint8)(end_temp - buffer)) != 0 )
          {
              //build name string: assuming name's at first linked-list packet
          		memcpy(temporal_buffer,buffer,u16Length);//cpy file's name for http request
          	  temporal_buffer[u16Length] = '\0';        //string delimitator

              /*FSL:regular POST request processing*/
              if(flag == POST_FLAG)
              {
                 if( !CGI_parser((int8*)temporal_buffer,(void*)q) )
                 {
           	        //cgi not found
          	        HTTP_SendError(pcb);          	        
          	        return;                 
                 }
              }

              /*turn it into upper case*/
              strupr((char *)temporal_buffer);
          		
          		/*FSL:regular GET request processing*/
          		
              /*look for file - internal memory*/
              if( !HTTP_GetFileFromInternalMemory(pcb, temporal_buffer) )
              {                 
                 //TODO: more support if file was succesfully found and sent
              }
              #if HTTP_SD_CARD_SUPPORT
              /*look for file - SD card*/
              else if( !HTTP_GetFileFromSDcard(pcb, temporal_buffer) )
              {
                 //TODO: more support if file was succesfully found and sent
              }
              #endif
              #if HTTP_TASKS_STACK_USE == 1
          	  //default service: tasks.htm: just for debugging!!!
          	  else if(!strcmp((char *)temporal_buffer,"TASKS.HTM"))
          	  {
                  HTTP_ShowTasksStatus(pcb);  
          	  }
          	  #endif
          	  else
          	  {                        	  
          	      //web page error
          	      HTTP_SendError(pcb);         	    
          	  }
        	}
        	else//send DEFAULT WEB PAGE!!!!
        	{
        		  //get language
        		  default_language = HTTP_GetLanguageIndex((void *)q);
        		  
        		  //force a dynamic web page even if file's name is NULL 
        		  //ie http://www.freescale.com/
        		  strcpy((char *)temporal_buffer, ".SHTML" );
        		  HTTP_SendWebPage(pcb, temporal_buffer, (uint8 *)index_htm, WEBPAGE_INDEX_LENGTH);
        	}
     }
     return;  
}

#if HTTP_SD_CARD_SUPPORT
/**
 * Look for a file from SDcard
 *
 * @param connection descriptor
 * @param file's name
 * @return zero if OK, otherwise file not found
 */
uint8 HTTP_GetFileFromSDcard(struct netconn *pcb, uint8 *array)
{  
  uint8 dynamic_proc;
  UINT16 u16Length;
  UINT8 result = 1;
  int filedesc;
  
  
  /*FSL: SD Mutex Enter*/
//  WEB_SD_MUTEX_ENTER
  
  /*Look for file on SD card: uppercase is needed*/
  //if( !FAT_FileOpen(SD_Web_Handle,u8Archivo,(uint8 *)array,READ) )
  if ((filedesc = cfs_open((char *)array, CFS_READ)) >= 0)
  {
     /*check file's name for dynamic support if ends with selected extension*/
     /*(ie. SHTML, FSL, etc)*/
     dynamic_proc = HTTP_IsDynamicExtension(array);

     /*chuncked header for BLOCK support*/
     HTTP_SendHeader(pcb,array,FALSE,TRUE);
     
     //while( (u16Length=FAT_FileRead(SD_Web_Handle,u8Archivo)) == BLOCK_SIZE )
     while ((u16Length = cfs_read(filedesc, u8Archivo, BLOCK_SIZE)) == BLOCK_SIZE )
     {     
       /*start sending SD BLOCKs*/     
       if( dynamic_proc )/*dynamic process*/
       {
          HTTP_SendDynamicData(pcb,array,u8Archivo,BLOCK_SIZE,SDCARD_BLOCK_FEATURE);
       }
       /*static processing*/
       else
       {
          HTTP_SendDynamicPacket(pcb,u8Archivo,BLOCK_SIZE,NETCONN_COPY);
       }     
     }
     if(u16Length)
     {
       /*start sending remaining SD BLOCK*/     
       if( dynamic_proc )/*dynamic process*/
       {
          HTTP_SendDynamicData(pcb,array,u8Archivo,u16Length,SDCARD_BLOCK_FEATURE);
       }
       /*static processing*/
       else
       {
          HTTP_SendDynamicPacket(pcb,u8Archivo,u16Length,NETCONN_COPY);
       }      
     }    
     cfs_close(filedesc);
     /*send last web page packet*/
     HTTP_SendDynamicFooter(pcb);
     
     result = 0;/*file found*/
  }

  /*SD Mutex Exit*/
//  WEB_SD_MUTEX_EXIT
  
  return result;/*file not found*/
}
#endif

/**
 * Look for a file from internal flash system
 *
 * @param connection descriptor
 * @param file's name
 * @return zero if OK, otherwise file not found
 */
uint8
HTTP_GetFileFromInternalMemory(struct netconn *pcb, uint8 *name)
{
   uint8 i;
   
   //look for the file in internal memory
   for(i=0;i<number_of_files;i++)
   {
      //a file matchs with internal flash system
      if(!strcmp((char *)name, InternalMemoryFilenames[i]))
      {
          /*data and data length*/
          uint8 *file_pointer = (uint8 *)(InternalMemoryPointers[i]);
          uint16 file_length = (uint16)InternalMemoryLengths[i];

          /*send web page*/
          HTTP_SendWebPage(pcb, (uint8 *)name, file_pointer, file_length);                     
          return 0;
      }
   }   
   return 1;/*file not found*/  
}

/**
 * Returns Language greeting to be used for web page
 *
 * @param HTTP request to look for language string
 * @return index to language to use
 */
uint8
HTTP_GetLanguageIndex(void *string)
{                                                   
    uint8 i;
    uint8 *buffer;
    
    /*search for string on linked list: reusing SD buffer*/
    buffer = search_string_linked_list(string, u8Archivo, (uint8 *)HTTP_CNTNT_LNG_REQUEST, (uint8 *)STRING_END);
   
    for(i=0;i<max_number_of_languages();i++)
    {
      //contains array
      if(!strncmp((const CHAR *)buffer,(const CHAR *)WEB_LANGUAGES[i].language_code,GREETING_LANGUAGE_CODE_LENGTH))
      {
          return i;
      }          
    }
   
    return 0;//default English
}

/**
 * Sends a complete web page: header and data
 *   internally select between static or dynamic processing
 *
 * @param connection descriptor
 * @param buffer to use for parsing
 * @param web page data
 * @param web page data length
 * @return none
 */
void HTTP_SendWebPage(struct netconn *pcb, uint8 *array_to_send, uint8 *dynamic_info, uint16 dynamic_info_length)
{
     /*get if a dynamic process is needed*/
     uint8 dynamic_flag = HTTP_IsDynamicExtension(array_to_send);
     
     //send header
     HTTP_SendHeader(pcb, array_to_send, dynamic_info_length, dynamic_flag);
     //send data
     HTTP_SendData(pcb, dynamic_flag, array_to_send, dynamic_info, dynamic_info_length);
     
     return;  
}

/**
 * Return TRUE if file's name ends with SHTML or FSL extension, which means
 *   they need a dynamic processing
 *
 * @param file's name
 * @return none
 */
uint8
HTTP_IsDynamicExtension(uint8 *array_to_send)
{     
     if( (strstr((char *)array_to_send,".SHTML") != NULL) || (strstr((char *)array_to_send,".FSL") != NULL))
     {
          return TRUE;
     }     
     return FALSE;  
}

/**
 * Send a web page header: process it for static or dynamic depending on dynamic flag
 *
 * @param connection descriptor
 * @param buffer to use for parsing
 * @param web page data length to be used for static header. NULL for dynamic
 * @param flag to select between static or dynamic data processing
 * @return none
 */
void
HTTP_SendHeader(struct netconn *pcb, uint8 *array_to_send, uint16 dynamic_info_length, uint8 dynamic_flag)
{
#if HTTP_DYNAMIC_SUPPORT == 1
     /*get MIME type to be sent with the HTTP response*/
     uint8 *dynamic_type = MIME_GetMediaType(array_to_send);

     //FSL: build HTTP 200 OK and that stuff!!
     strcpy( (char *)array_to_send, HTTP_HEADER_OK_PART1 );
     strcat( (char *)array_to_send, (char *)dynamic_type);
     
     if(!dynamic_flag)
     {                                         
#endif
       strcat( (char *)array_to_send, HTTP_HEADER_OK_PART2 );
       sprintf( (CHAR *)array_to_send, "%s%d", array_to_send, dynamic_info_length );
       strcat( (char *)array_to_send, STRING_DOUBLE_END );//static terminator
#if HTTP_DYNAMIC_SUPPORT == 1
     }
     else
     {
       strcat( (char *)array_to_send, HTTP_DYMAMIC_HEADER );//dynamic header
     }
#endif          
     /*send header*/
     netconn_write(pcb, &array_to_send[0], (uint16)strlen((char *)array_to_send), NETCONN_COPY); //NETCONN_COPY );
}

/**
 * Send a web page data: process it for static or dynamic depending on dynamic flag
 *
 * @param connection descriptor
 * @param flag to select between static or dynamic data processing
 * @param buffer to use for parsing
 * @param static data
 * @param static data length
 * @return none
 */
/*static*/ void
HTTP_SendData(struct netconn *pcb, uint8 dynamic_flag, uint8 *array_to_send, uint8 *dynamic_info, uint16 dynamic_info_length)
{
#if HTTP_DYNAMIC_SUPPORT == 1
    /*dynamic process*/
    if( dynamic_flag ) 
    {
       HTTP_SendDynamicData(pcb, (uint8 *)array_to_send, dynamic_info, dynamic_info_length, FALSE/*send footer*/);
    }
    /*static process*/
    else
    {
#endif                                
       //FSL: send as it is: OK
       netconn_write(pcb, &dynamic_info[0], (uint16)dynamic_info_length, NETCONN_COPY);
#if HTTP_DYNAMIC_SUPPORT == 1
    }
#endif  
}

/**
 * Send a web page data replacing strings with SSI rules
 *
 * @param connection descriptor
 * @param temporal buffer for replacing
 * @param static data to be parsed with SSI rules
 * @param static data length 
 * @param send "Transfer-Encoding: chunked" footer  
 * @return none
 */
void
HTTP_SendDynamicData(struct netconn *pcb, uint8 *array_to_send, uint8 *dynamic_info, uint16 dynamic_info_length, uint8 flag)
{
     //FSL: dynamic web page implementation:
     uint8 *index_a,*index_b,*index_c;  
     uint8 *dynamic_start = (uint8 *)dynamic_info;
     uint8 *temporal_buffer = (uint8*)array_to_send;
     uint8 *dynamic_start_temp;
     uint16 dynamic_length = dynamic_info_length;
     
     while(dynamic_length > 0)
     {
       if(( index_a = (uint8 *)strstr((char *)dynamic_start,SSI_START) ) != NULL)
       {
          if(( index_c = (uint8 *)strstr((char *)index_a,SSI_END) ) != NULL)
          {
              index_b = index_a + str_len(SSI_START);/*considering NULL character*/
              
              //send no dynamic part
              HTTP_SendDynamicPacket(pcb, &dynamic_start[0], ( u16_t )(index_a - dynamic_start), FALSE);
              
              //replace with SSI parser
              if( SSI_parser( index_b, temporal_buffer ) )
              {
                 //send dynamic information get from SSI replacement
                 HTTP_SendDynamicPacket(pcb, temporal_buffer, ( u16_t )strlen((char *)temporal_buffer), NETCONN_COPY);
              }
              //update web page index
              dynamic_start_temp = dynamic_start;
              dynamic_start = index_c + str_len(SSI_END)/*considering NULL character*/;
              dynamic_length = dynamic_length -  (uint16)(dynamic_start - dynamic_start_temp);
          }
          else
          {
              //send static content
              HTTP_SendDynamicPacket(pcb, dynamic_start, ( uint16 )dynamic_length, FALSE);
              break;
          }
       }
       else
       {
          //send static content
          HTTP_SendDynamicPacket(pcb, dynamic_start, ( uint16 )dynamic_length, FALSE);
          break;
       }
     }
     if(!flag)
     {
       /*FSL: end terminator for chunked encondig*/
       HTTP_SendDynamicFooter(pcb);  
     }     
     return;/*FSL: page has already been sent*/  
}

/**
 * Send a web page with all tasks' status
 *
 * @param connection descriptor 
 * @return none
 */
void
HTTP_ShowTasksStatus(struct netconn *pcb)
{
      portCHAR *cDynamicPage;
      
 #if 0
      /* FSL: get the space from heap*/
      if( (cDynamicPage = (portCHAR *)pvPortMalloc(160/*guessing max 8 tasks*/)) == NULL)
      {
      	    //error web page!!!
      	    HTTP_SendError(pcb);
      	    return;
      }
#else
      /*reusing web buffer*/
      cDynamicPage = (portCHAR *)u8Archivo;
#endif      
      /* Write out the HTTP OK Header and Data. */
      netconn_write(pcb, HTTP_DYNAMIC_HEADER, ( u16_t )str_len( HTTP_DYNAMIC_HEADER ), NETCONN_COPY );
      HTTP_SendDynamicPacket(pcb, HTTP_TASK_STATUS_START, ( u16_t )str_len( HTTP_TASK_STATUS_START ), FALSE );

      /*Get list of tasks and their status*/
      //TODO: vTaskList( (char * )cDynamicPage);
      HTTP_SendDynamicPacket(pcb, cDynamicPage, ( u16_t )strlen( cDynamicPage ), NETCONN_COPY );
      
      /*last HTML content: web page signature*/
      HTTP_SendDynamicPacket(pcb, HTTP_TASK_STATUS_END, ( u16_t )str_len( HTTP_TASK_STATUS_END ), FALSE );
      /*generate HTML footer */
      HTTP_SendDynamicFooter(pcb);
#if 0      
      /*FSL: free memory*/
      vPortFree(cDynamicPage);
#endif      
      return;  
}

/**
 * Send a packet with "Transfer-Encoding: chunked" rules
 *
 * @param connection descriptor
 * @param data to send
 * @param data length
 * @param copy or a reference to be used for data  
 * @return none
 */
void
HTTP_SendDynamicPacket(struct netconn *pcb, const void *data, u16_t len, u8_t apiflags)
{
   CHAR number[10];/*hold number*/
   
   if(len)
   {
     sprintf(number,"\r\n%x\r\n",len);
     
     //chunk length
     netconn_write(pcb,number,strlen(number),NETCONN_COPY);
     //generated data
     netconn_write(pcb,data,len,apiflags);    
   }
}

/**
 * Send "Transfer-Encoding: chunked" footer
 *
 * @param connection descriptor
 * @return none
 */
void
HTTP_SendDynamicFooter(struct netconn *pcb)
{
   //generated data
   netconn_write(pcb,"\r\n0\r\n\r\n",str_len("\r\n0\r\n\r\n"),NETCONN_COPY);
}

/**
 * Send page not found error to HTTP connection
 *
 * @param connection descriptor
 * @return none
 */
void
HTTP_SendError(struct netconn *pcb)
{
     //header
     netconn_write(pcb, HTTP_WRONG_HEADER, str_len(HTTP_WRONG_HEADER), NETCONN_COPY );
     //data
     netconn_write(pcb, HTTP_WRONG_DATA, str_len(HTTP_WRONG_DATA), NETCONN_COPY );
}


void serviceTask(void *conn)
{
	uint8 *temporal_buffer;
	if( (temporal_buffer=(uint8 *)pvPortMalloc( HTTP_WORKING_SPACE )) == NULL )
	{
	  goto http_server_exit;
	}
	if ((u8Archivo=(uint8 *)pvPortMalloc( BLOCK_SIZE+1 )) == NULL)
	{
		goto http_server_exit;
	}
	netconn_set_timeout((void *)conn,10/*0x1*//*timeout*/);
	HTTP_ProcessConnection( &conn, temporal_buffer );

	http_server_exit:
	 	netconn_disconnect(conn);
		netconn_delete(conn);
		vPortFree(temporal_buffer);
		vPortFree(u8Archivo);
		printf("HTTPD: Task %d killed\n", conn);
		conn = NULL;
		//vTaskDelete( NULL );
}


/********************Public Functions ***************************************/

/**
 * Start an embedded HTTP server Task: 1 client and multiple files per transfer
 *
 * @param none
 * @return none
 */
void
HTTP_Server_Task( void *pvParameters )
{

	struct netconn *conn = NULL;
	struct netconn *newconn = NULL;
	conn = netconn_new(NETCONN_TCP);
	if (netconn_bind(conn, IP_ADDR_ANY, HTTP_PORT) != ERR_OK) goto server_exit;
	if (netconn_listen(conn) != ERR_OK) goto server_exit;
    /*set timeout for this connection*/
    netconn_set_timeout((void *)conn,10/*0x1*//*timeout*/);

	for (;;)
	{
		if (netconn_accept(conn, &newconn) != ERR_OK) goto server_exit;
		// creating new task
		printf("HTTPD: Create service and new socket - %d\n", newconn);
		//xTaskCreate( serviceTask, "svc", 2048/*256*/,(void*)newconn, DEFAULT_THREAD_PRIO, NULL);
		serviceTask(newconn);
	}
	server_exit:
		netconn_delete(conn);
		netconn_delete(newconn);
		conn = NULL;
		//vTaskDelete( NULL );
}

void HTTP_Server_init(void)
{
	sys_thread_new("HTTPServer", HTTP_Server_Task, NULL, 2048/*128*/, DEFAULT_THREAD_PRIO);
}

