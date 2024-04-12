/**********************************************************************************************************************
 // FileName:        main.c
 // Dependencies:    msp.h, stdint.h
 // Processor:       MSP432
 // Board:           MSP432P401R
 // Program version: CCS V10 TI
 // Company:         Texas Instruments
 // Description:     ACCESO A MEMORIA DIRECTA DE REGISTROS DE PERIFERICO GPIO.
 // Authors:         ALFREDO CHACON
 // Updated:         03/2021
 //Nota: no se están usando las estructuras definidas en los sorucefiles dados por el BSP de TI,
 //      se incializan derectamente las direcciones de memoria de los registros de los perifericos en cuestión
 ************************************************************************************************************************/

/************************************************
  HEADER FILES
 ************************************************/
#include<stdint.h>
#include "msp.h"
/************************************************
  DEFINICIONES DE CONSTANTES Y/O MASCARAS
 ************************************************/
#define RETARDO (100000)
#define RETARDO2 (10000)
#define PIN0    (0x01)  //definimos el numero Hexadecimal segun el Pin del PUERTO
#define PIN1    (0x02)
#define PIN2    (0x04)
#define PIN4    (0x10)
#define FALSE   0
#define TRUE    1
/************************************************
    PROTOTIPO DE FUNCION
************************************************/
extern uint8_t Gpio_pin_in( uint_fast16_t pin_);

/*****************************************************************************
 * Function: MAIN
 * Preconditions: NINGUNA.
 * Overview:
 * Input: NINGUNA.
 * Output: NINGUNA
 *
 *****************************************************************************/
void main(void)
{
    /************************************************
          DECLARACION DE VARIABLES
    ************************************************/
    uint32_t i;
    uint8_t bandera = 0;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // DETIENE EL TIMER DEL WATCHDOG
    /************************************************
          DEREFERENCIA DE MEMORIA PARA SU MODIFICACION
     ************************************************/
    (*((volatile uint8_t *)(0x040004c04))) &= ~0x10;                               //PIN P1.4 COMO ENTRADA                          (p1_dir)
    (*((volatile uint8_t *)(0x040004c06))) |= 0x10;                                //HABILITAMOS RESISTENCIA                        (p1_ren)
    (*((volatile uint8_t *)(0x040004c02))) |= 0x10;                                //PULL UP                                        (p1_out)

    (*((volatile uint8_t *)(0x040004c05))) |= 0x07;                                //PIN P2.0, P2.1, P2.2  COMO SALIDA, LEDS RGB    (p2_dir)
    (*((volatile uint8_t *)(0x040004c03))) &= ~0x07;                               // APAGADOS                                      (p2_out)
        while(1){
            if ( Gpio_pin_in(0x10) != 1)                // ENTRA AL PRESIONAR EL SWITCH DEL PUERTO P1.4 (CUANDO ESTA EN BAJO)
            {
                if(bandera == FALSE)
                    {
                        bandera = TRUE;                                 //BANDERA EN ALTO
                        while (bandera==TRUE){
                            (*((volatile uint8_t *)(0x040004c03))) &= ~0x07;                           //APAGA BLUE
                            (*((volatile uint8_t *)(0x040004c03))) |= 0x01;                            //ENCIENDE RED
                            for(i=RETARDO; i>0 ;i--);
                            (*((volatile uint8_t *)(0x040004c03))) &= ~0x01;                           //APAGA RED
                            (*((volatile uint8_t *)(0x040004c03))) |= 0x02;                            //ENCIENDE GREEN
                            for(i=RETARDO; i>0 ;i--);
                            (*((volatile uint8_t *)(0x040004c03))) &= ~0x02;                           //APAGA GREEN
                            (*((volatile uint8_t *)(0x040004c03))) |= 0x04;                            //ENCIENDE BLUE
                            for(i=RETARDO; i>0 ;i--);
                         if ( Gpio_pin_in(0x10) != 1) bandera=FALSE;    // ENTRA CUANDO PRESIONAMOS BOTON DE P1.4
                         while( Gpio_pin_in(0x10) != 1);                //MIENTRAS ESTE PRESIONADO EL BOTON
                        }

                    }
                (*((volatile uint8_t *)(0x040004c03)))  &= ~0x07;                         //APAGA TODOS
               // while( GPIO_PIN_IN(0x10) != 1);
            }
            for(i=RETARDO2; i>0 ;i--);
        }
}
/*****************************************************************************
 * Function: Gpio_pin_in
 * Preconditions: PUERTO1 SELECCIONADO.
 * Overview: LEE EL PIN DESEADO
 * Input: pin_.
 * Output: 0/1
 *
 *****************************************************************************/
uint8_t Gpio_pin_in( uint_fast16_t pin_)
{
    uint_fast16_t inputPinValue;
    inputPinValue = (*((volatile uint8_t *)(0x040004c00))) & (pin_);        // LEE EL REGISTRO QUE INDICA EL VALOR DE ENTRADA DE UN PIN (p1_in)
    if (inputPinValue > 0)                  // RETORNA EL VALOR 1 o 0
        return (0x01);
    return (0x00);
}
