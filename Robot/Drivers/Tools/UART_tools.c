#include <Robot/Drivers/Tools/UART_tools.h>

//
//Envia una cadena de caracteres por el puerto serial
//
void UART_puts(UART_Handle uart_handle,const char* msj){
    while(*msj){
        UART_write(uart_handle,msj,1);
        *msj++;
    }
    UART_putch(uart_handle,'\n');
}
//
//Envia un caracter por el puerto serial
//
void UART_putch(UART_Handle uart_handle,char c_msj){
    UART_write(uart_handle,&c_msj,1);
}
//
//Envia el numero en formato ASCII Hex por el puerto serial
//
extern void UART_putHex(UART_Handle uart_handle,uint32_t number){
    uint32_t n_number=0;
    //Invertir los nibble de MSB - LSB , LSB - MSB
    //Es decir, ordenar la memoria en porciones de 4 bits Big-Endian
    UART_puts(uart_handle,"0x");
    LOOP_EXTRACT:
        n_number<<=4;
        n_number |= number&0xF;
        number>>=4;
        if(number)
            goto LOOP_EXTRACT;
    LOOP_SEND:
        //Enviar los 4bits LSB
        UART_putch( uart_handle, UART_intToHexChar(n_number&0xF));
        n_number >>= 4;
        if(n_number)
            goto LOOP_SEND;
}
//
//Envia el numero en formato ASCII por el puerto serial
//
void UART_putInt(UART_Handle uart_handle,uint32_t number){
    uint32_t n_number=0;
    uint16_t c_number=0;
LOOP_EXTRACT:
    n_number*=10;
    n_number += number%10;
    number/=10;
    c_number++;
    if(number)
        goto LOOP_EXTRACT;
LOOP_SEND:
    UART_putch( uart_handle, UART_intToHexChar(n_number%10) );
    n_number /= 10;
    c_number--;
    if(n_number || c_number){
        goto LOOP_SEND;
    }
}


//
//Envia el numero en formato ASCII por el puerto serial
//
void UART_putFloat(UART_Handle uart_handle,float number){
    uint32_t d_buffer;
    if(number<0){
        UART_putch(uart_handle, '-');
        number *=-1;
    }
//Enviamos la parte entera
UART_putInt(uart_handle,(int)number);

//Para obtener la parte decimal
number*=1000;
//La parte decimal del numero
d_buffer = (int)number;


UART_putch(uart_handle,'.');
//Enviamos la parte decimal
d_buffer = d_buffer%1000;
UART_putch(uart_handle,UART_intToHexChar((d_buffer/100)%10));
UART_putch(uart_handle,UART_intToHexChar((d_buffer/10)%10));
UART_putch(uart_handle,UART_intToHexChar((d_buffer)%10));
//UART_putch(uart_handle,'\n');
}

//
//Convierte el numero entregado en formato ASCII Hexadecimal
//
char UART_intToHexChar(uint8_t digit){
    if(digit<10){
        return digit+'0';
    }
    if(digit>9 && digit<16){
        return digit+'7';
    }
    return '.';
}

//Convierte un caracter en entero
unsigned char UART_CharToInt(char digit){
    if( (digit>='0') && (digit<='9') ){
        return digit-'0';
    }else{
        return 0;
    }
}
//Recupera desde el puerto serie un numero en formato flotante
float UART_readFloat(UART_Handle uart_handle){
    char    rxBuffer=' ';
    int     bufferInt=0;
    int     bufferDec=0;
    register int     counterDec=1;
    unsigned char flag=0;
    float   result=0.0;

    UART_read(uart_handle,&rxBuffer,1);
    if(rxBuffer == '-'){flag=1;}
    while(1){
        if(rxBuffer == '.'){break;}
            bufferInt *=10;
            bufferInt += UART_CharToInt(rxBuffer);
        UART_read(uart_handle,&rxBuffer,1);
    }
    while(1){
        UART_read(uart_handle,&rxBuffer,1);
        if(rxBuffer == ';'){break;}
            bufferDec *=10;
            counterDec *=10;
            bufferDec += UART_CharToInt(rxBuffer);
    }
    result = (float)bufferInt + (float)bufferDec/ (float)counterDec;
    if(flag){
        return result*-1;
    }
    return result;
}
char UART_readChar(UART_Handle uart_handle){
    char    rxBuffer=' ';
    UART_read(uart_handle,&rxBuffer,1);
    return rxBuffer;
}
