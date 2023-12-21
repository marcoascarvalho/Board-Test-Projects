/*************************************************************************
 *
 *    TestCore
 *
 *    (c) Copyright Dixtal 2010
 *
 *    File name   : TestMain.c
 *    Description : Define a test Main for LPC2136. This file contains
 *                  peripheral initializations (timers, interrupts, PWMs,
 *                  etc) according used by TestApp.c
 *
 *    History :
 *    1. Data        : Jan 31, 2010
 *       Author      : Marco Carvalho
 *       Description : Create
 *
 *    $Revision: 1.0$
**************************************************************************/
#include "includes.h"

extern VOID TestAppMainLoop();

//const char UART_Menu[] = "\r\nUart commands\r\n'1' Get Time\r\n'2' Get Date\r\n'?' Help\n\r";

/*************************************************************************
 * Function Name: IRQSub
 * Parameters: void
 * Return: void
 *
 * Description: IRQ subroutine
 *		
 *************************************************************************/
#pragma vector=0x18
__irq __arm void IRQ_ISR_Handler (void)
{
    void (*interrupt_function)();
    unsigned int vector;

    vector = VICVectAddr;     // Get interrupt vector.
    interrupt_function = (void(*)())vector;
    (*interrupt_function)();  // Call vectored interrupt function.
}

/*************************************************************************
 * Function Name: IRQSub
 * Parameters: void
 * Return: void
 *
 * Description: FIQ subroutine
 *		
 *************************************************************************/
#pragma vector=0x1c
__fiq __arm void FIQ_ISR_Handler (void)
{
    void (*interrupt_function)();
    unsigned int vector;

    vector = VICVectAddr;     // Get interrupt vector.
    interrupt_function = (void(*)())vector;
    (*interrupt_function)();  // Call vectored interrupt function.
}

/*************************************************************************
 * Function Name: NonVectISR
 * Parameters: void
 * Return: void
 *
 * Description: non vectored callback subroutine
 *		
 *************************************************************************/
void NonVectISR(void)
{
}

/*************************************************************************
 * Function Name: ClearFlag
 * Parameters: void
 * Return: void
 *
 * Description: clear arg
 *		
 *************************************************************************/
void ClearFlag (void* arg)
{

    int* pFlag = arg;
    *pFlag = 0;

}

/*************************************************************************
 * Function Name: Dly100us
 * Parameters: void *arg
 * Return: void
 *
 * Description: Timer1 CH0 subroutine - delay [100us]
 *		
 *************************************************************************/
//void Dly100us(void *arg)
//{
//int Flag = 1;
//int Delay = (int)arg;
  // Stop Timer 1
  //TIMER_Stop(TIMER1);
  // Stop Reset Timer 1 counter
  //TIMER_Reset(TIMER1);
  // Set action of match module CH0
  // TIMER_SetMatchAction(TIMER1, CH0, TimerAction_Interrupt | TimerAction_StopTimer,
  //Delay usec_T1*100, ClearFlag, (void *)&Flag, DONOTHING);
  // Start Timer 1
  //TIMER_Start(TIMER1);
  // Wait expire of delay
  //while(Flag);
//}

/*************************************************************************
 * Function Name: SysInit
 * Parameters: void
 * Return: int
 *
 * Description: Hardware initialize
 *		
 *************************************************************************/
int SysInit(void)
{
    // Initialize the system
#ifdef FLASH
    //if (SYS_Init(FOSC, FCCLK, VPBDIV1, USER_FLASH, 0x0001FF0F,0x87FE01F1,0,0xFFFFFFFF))
    if (SYS_Init(FOSC, FCCLK, VPBDIV1, USER_FLASH, 0x0001FF0F,0,0,0))
    {
        return 1;
    }
#else
    if (SYS_Init(FOSC, FCCLK, VPBDIV1, USER_RAM,   0x0001FF0F,0x87FE01F1,0,0xFFFFFFFF))
    {
      return 1;
    }
#endif

    // Initialize Serial Interface
    if (UART_Init(UART0))
    {
        return 1;
    }
    if (UART_Init(UART1))
    {
        return 1;
    }

    // Initialize Timers
    //if (TIMER_Init(TIMER0, TIMER_PRECISION))
    //{
    //    return 1;
    //}
    //if (TIMER_Init(TIMER1, TIMER_PRECISION))
    //{
    //    return 1;
    //}

    // Initialize RTC
    //if (RTC_Init(0))	
    //{
    //    return 1;
    //}

    // initialize VIC
    VIC_Init();
    VIC_SetProtectionMode(UserandPrivilegedMode);
    // Enable interrupts non vectored interrupts
    VIC_EnableNonVectoredIRQ(NonVectISR);

    // UART0 interrupt
    VIC_SetVectoredIRQ(UART0_ISR,VIC_Slot0,VIC_UART0);
    VIC_EnableInt(1<<VIC_UART0);

    // UART1 interrupt
    VIC_SetVectoredIRQ(UART1_ISR,VIC_Slot1,VIC_UART1);
    VIC_EnableInt(1<<VIC_UART1);

    // Timer0 interrupt
    //VIC_SetVectoredIRQ(TIMER0_ISR,VIC_Slot2,VIC_TIMER0);
    //VIC_EnableInt(1<<VIC_TIMER0);

    // Timer1 interrupt
    // VIC_SetVectoredIRQ(TIMER1_ISR,VIC_Slot3,VIC_TIMER1);
    //VIC_EnableInt(1<<VIC_TIMER1);

    // UART1 interrupt
    //VIC_SetVectoredIRQ(UART1_ISR,VIC_Slot4,VIC_UART1);
    //VIC_EnableInt(1<<VIC_UART1);

    // RTC interrupt
    //VIC_SetVectoredIRQ(RTC_ISR,VIC_Slot5,VIC_RTC);
    //VIC_EnableInt(1<<VIC_RTC);

    //UART_PutStringByPolling(UART0, "\n\rDixtal Test Init Finish!\n\r");
    //UART_PutStringByPolling(UART0, "======Start Testing======\n\r");
    //UART_PutStringByPolling(UART1, "\n\rDixtalTest Init Finish!\n\r");
    //UART_PutStringByPolling(UART1, "======Start Testing======\n\r");

    return 0;
}

/*************************************************************************
 * Function Name: UserStart
 * Parameters: void
 * Return: void
 *
 * Description: User demo subroutine
 *		
 *************************************************************************/
void UserStart(void)
{

    /* System time init */
    //TIMER_SetMatchAction(TIMER0, CH0, TimerAction_Interrupt | TimerAction_ResetTimer,
    //                    1sec_T0/TICK_PER_SECOND, SetSysTickFlag, (void *)&TickSysFlag, DONOTHING);

    //TIMER_Start(TIMER0);

    /* HD44780 LCD driver init */
    //HD44780_PowerUpInit();

    /* Buttons Init */
    //ButtonsInit();

    /* Light Init */
    // LightInit();

    /* Menu Init */
    //MenuInit(MENU_ENGLISH_LANG,MENU_WELCOME_SHOW,NULL,MENU_WELCOM_DLY);

    /* RTC_Enable */
    //RTC_Enable();

}

/*************************************************************************
 * Function Name: main
 * Parameters: void
 * Return: void
 *
 * Description: Main subroutine
 *		
 *************************************************************************/
void main ()
{

    if(SysInit() == 0)
    {
        // Start user program
        __enable_interrupt();
        /* Enable/Configure Peripherals to be used in TestApp */
        UserStart();
    }

       /*
       always send a dummy byte, just to make its first appearance to the master.
       It was verified that the proper operation of UART (sending by interrupt)
       requested a initial sending by polling
    */
    UART_PutCharByPolling(UART0, 0xAA);
    UART_PutCharByPolling(UART0, 0xBB);
    UART_PutCharByPolling(UART0, 0xCC);
    UART_PutCharByPolling(UART0, 0xDD);

    /*
       in sequence send go-ahead or busy (in this case go-ahead)
    */

    AnswerSync();

   /* Running all the Test Aplication here */

    TestAppMainLoop();

}
