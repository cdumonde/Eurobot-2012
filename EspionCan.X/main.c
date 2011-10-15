#include <stdio.h>
#include <p18f2680.h>
#include <delays.h>
#include <usart.h>


#define led PORTCbits.RC0

#pragma config OSC = HS
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRT = OFF
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LPT1OSC = OFF
#pragma config PBADEN = OFF
#pragma config DEBUG = OFF
#pragma config XINST = OFF
#pragma config BBSIZ = 1024
#pragma config LVP = OFF

/////VARIABLES GLOBALES ////
char x;

/////*INTERRUPTIONS*/////
void high_isr(void);
void low_isr(void);

#pragma code high_vector=0x08
void high_interrupt(void)
{
     _asm GOTO high_isr _endasm
}
#pragma code low_vector=0x18
void low_interrupt(void)
{
     _asm GOTO low_isr _endasm
}
#pragma code

#pragma interrupt high_isr
void high_isr(void)
{
    if(PIE1bits.RCIE && PIR1bits.RCIF)
    {
        x = ReadUSART();
    }
       
}

#pragma interrupt low_isr
void low_isr(void)
{

}

////////*PROGRAMME PRINCIPAL*////////
void main (void)
{
    //initialisations
        ADCON1 = 0x0F ;
	ADCON0 = 0b00000000;
	WDTCON = 0 ;

        /* Direction des ports I in, O out*/
	TRISA = 0b11111111 ;
	TRISB = 0b01111011 ;  //canTX en sortie
	TRISC = 0b11111110;

        /* Etat des sorties */
        PORTA = 0b11111111 ;
	PORTB = 0b11111111 ;
	PORTC = 0b01111111 ;

        /*Config Rs232*/
   OpenUSART( USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE
                & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 63 );

    /*Autorisation des Imteruptions Générales */
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1; /* Autorise interruptions haut niveau. */
    INTCONbits.PEIE = 1; /*Autorise interruptions bas niveau.*/

    while(1)
    {
        printf("hello\n");
        led=1;
        Delay10KTCYx(200);
        led=0;
        Delay10KTCYx(200);
    }
}