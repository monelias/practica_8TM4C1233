#include "lib/include.h"
extern void Configurar_UART1(void)
{
SYSCTL->RCGCUART  = (1<<1);   //Paso 1 (RCGCUART) pag.344 UART/modulo1 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<1);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port B
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOB->AFSEL = (1<<1) | (1<<0);
    //GPIO Port Control (GPIOPCTL) PB0-> U0Rx PB1-> U0Tx pag.688
    // (GPIOB->PCTL&0xFFFFFF00) | 0x00000011;// 
    GPIOB->PCTL = (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOB->DEN = (1<<0) | (1<<1);//PB1 PB0
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART1->CTL = (0<<9) | (0<<8) | (0<<0);
 
    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 25,000,000 / (16*57600) = 27.1267
    UARTFBRD[DIVFRAC] = integer(.1267 * 64 + 0.5)
    */
    UART1->IBRD = 27;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART1->FBRD = 7;
    //  UART Line Control (UARTLCRH) pag.916
    UART1->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART1->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART1->CTL = (1<<0) | (1<<8) | (1<<9);
}

extern void Configurar_UART0(void)
{
    SYSCTL->RCGCUART  = (1<<0);   //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<0);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port A
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOA->AFSEL = (1<<1) | (1<<0);
    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOA->DEN = (1<<0) | (1<<1);//PA1 PA0
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART0->CTL = (0<<9) | (0<<8) | (0<<0);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 25,000,000 / (16*57600) = 27.1267
    UARTFBRD[DIVFRAC] = integer(.1267 * 64 + 0.5)
    */
    UART0->IBRD = 162;//27;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART0->FBRD = 49;//7;
    //  UART Line Control (UARTLCRH) pag.916
    UART0->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART0->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART0->CTL = (1<<0) | (1<<8) | (1<<9);



}

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART1->FR & (1<<4)) != 0 );
    v = UART1->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART1->FR & (1<<5)) != 0 );
    UART1->DR = c;
}

extern void printString(char* string, int Size)
{
    int i = 0;
    while (string[i] > 47) 
    {
        printChar(string[i]);
        i++;
    }
}

extern int readString(char delimitador, char *string)
{
   int i = 0;
   char c = readChar();
   while(c != delimitador)
   { 
            string[i] = c;
            i++;
            c = readChar();
   }
   return i;
}

//Experimento 2

//El envio es su nombre  (rave) 

// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 

extern void invertString(char delimitador, char *string, int Size)
{
    int tam,dec,k = 0;
    int cont = 1;
    if (Size < 10)
    tam = Size + (Size-1);
    else
    tam = 18 + ((Size - 9)*3);
    char strong[tam];
    while(k<tam)
    {
            strong[k] = string[Size-(cont)];
            if(cont <= 9)
            {
            strong[k + 1]= cont + '0';
            k = k + 2;
            }
            else 
            {
                if (cont%10 == 0)
                dec = cont / 10;
                else;
                strong[k + 1] = dec + '0';
                strong[k + 2] = (cont%10) + '0' ;
                k = k + 3;
            }
            cont++;
    }
    for(int g = 0;g<=tam-1;g++)
        string[g]=strong[g];
    for(int n = k;n < 100;n++)
    string[n] = 0;
    return;
} 