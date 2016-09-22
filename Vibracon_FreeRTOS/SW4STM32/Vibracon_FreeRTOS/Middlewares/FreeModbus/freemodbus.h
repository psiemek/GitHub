/* ------------------------ Defines --------------------------------------- */

#ifndef freemodbus_h
#define freemodbus_h

#define mainMB_TASK_PRIORITY    ( tskIDLE_PRIORITY + 3 )
#define PROG                    "FreeModbus"
#define REG_INPUT_START         1000
#define REG_INPUT_NREGS         4
#define REG_HOLDING_START       2000
#define REG_HOLDING_NREGS       30


void     vMBServerTask( void );

#endif

/*
 * Mapa rejestrow Modbus
 * 0 - Tryb pracy kanal 1:
 * 		0 - Wylaczony
 * 		1 - Wlaczony ustawienia z Modbus
 * 		2 - Wlaczony ustawienia z WWW
 * 		3 - Wlaczony ustawienia z wejsc cyfrowych
 * 		4 - Wlaczony ustawienia z potencjometrow
 * 		5 - Wlaczony ustawienia z wejsc analogowych
 * 		6 - Wlaczony ustawienia z petli pradowej
 * 1 - Czestotliwosc pracy kanalu 1:
 * 		0 - 100Hz generacja we wszystkich polowkach napiecia
 * 		1 - 50Hz generacja w co drugiej polowce
 * 2 - Opoznienie zalaczenia kanalu 1 (0 - 65001) - Od tego zalezy moc na kanale
 * 3 - Tryb pracy kanal 2:
 * 		0 - Wylaczony
 * 		1 - Wlaczony ustawienia z Modbus
 * 		2 - Wlaczony ustawienia z WWW
 * 		3 - Wlaczony ustawienia z wejsc cyfrowych
 * 		4 - Wlaczony ustawienia z potencjometrow
 * 		5 - Wlaczony ustawienia z wejsc analogowych
 * 		6 - Wlaczony ustawienia z petli pradowej
 * 4 - Czestotliwosc pracy kanalu 2:
 * 		0 - 100Hz generacja we wszystkich polowkach napiecia
 * 		1 - 50Hz generacja w co drugiej polowce
 * 5 - Opoznienie zalaczenia kanalu 2 (0 - 65001) - Od tego zalezy moc na kanale
 * 6 - Wartosc na potencjometrze nr 1
 * 7 - Wartosc na potencjometrze nr 2
 * 8 - Wartosc na wejsciu napieciowym 1
 * 9 - Wartosc na wejsciu napieciowym 2
 * 10 - Wartosc na wejsciu pradowym 1
 * 11 - Wartosc na wejsciu pradowym 2
 * 12 - Wartosc napiecia na czujniku temperatury
 * 13 - Temperatura na strukturze procesora
 *
 *
 *
 *
 */
