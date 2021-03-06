#ifndef UART_TOOLS_H
#define UART_TOOLS_H
    #include <stdint.h>
    #include <ti/drivers/UART.h>
    #define UART_endl(UART_HANDLE) UART_putch(UART_HANDLE,'\n')
    //
    //Envia una cadena de caracteres por el puerto serial
    //
    extern void UART_puts(UART_Handle,const char*);
    //
    //Envia un caracter por el puerto serial
    //
    extern void UART_putch(UART_Handle,char);
    //
    //Envia el numero en formato ASCII Hex por el puerto serial
    //
    extern void UART_putHex(UART_Handle,uint32_t);
    //
    //Envia el numero en formato ASCII por el puerto serial
    //
    extern void UART_putInt(UART_Handle,uint32_t);
    //
    //Envia el numero en formato ASCII por el puerto serial
    //
    extern void UART_putFloat(UART_Handle,float);
    //
    //Convierte el numero entregado en formato ASCII Hexadecimal
    //
    extern char UART_intToHexChar(uint8_t);
    //
    //Convierte el caracter en entero
    //
    extern unsigned char UART_CharToInt(char digit);
    //
    //Recupera desde el puerto serie un numero en formato flotante
    //
    extern float UART_readFloat(UART_Handle);
    //
    //Recupera desde el puerto serie un caracter ascii
    //
    extern char UART_readChar(UART_Handle);

#endif
