#include "tm4c123gh6pm.h"
#include "GPIO_Driver.h"
#include <stdint.h>
#include <stdio.h>
#include "GPTM_Driver.h"
#include "PWM_Driver.h"
#include "Clock_Driver.h"
#include "UART_Driver.h"

void PWM_Intialization(void);
void PortA_Intialization(void);
void UART_Intialization(void);
void GPTM_Number_1_Intialization(void);
void GPTM_Number_3_Intialization(void);

void Send_Data_UART(char Data[10]);
void New_Line_Message(void);
float counter = 0;
float speed = 0;
char mesg[12];
int j;
int i=0;
int main()
{
  Debug_Intialization();
  Set_Clock_MHz(16,80);
  PWM_Intialization();
  PortA_Intialization();
  UART_Intialization();
  GPTM_Number_1_Intialization();
  GPTM_Number_3_Intialization();

      CLK_Enable('F');
      //To unlock Port F
      GPIO_PORT_UNLOCK('F');
      //To make pins 1,2,and 3 Digital
      PIN_ANALOG_DIGITAL('F',1,"Digital");
      //To make pins 1,2,and 3 General I/O
      PIN_FUNCTION_SELECT('F',1,"General I/O");
      //To make pins 1,2,and 3 output
      PIN_INPUT_OUTPUT('F',1,"Output");
      //To make sure all leds are off
      PIN_WRITE_DIGITAL('F',1,"LOW");
      
      GPTM_Delay_Unit(1,250,"Millisecond");
      PWM_Set_Duty_Cycle(0,0,'B',50);
      while(1)
      {       
        Enable_Timer_Number(3,"Enable");
        GPTM_Delay_Unit(1,200,"Millisecond");
        counter = Timer_Number_Counts_Capture(3);
        speed = ((counter/8)*60000)/200;
        Enable_Timer_Number(3,"Disable");
        Timer_Number_Reset_Counts(3);
        
        char text[2];
        sprintf(text,"%0.3f,\n", speed);
        Send_Data_UART(text);
        New_Line_Message();
      }
    /*while(1)
    {
     for(i=1;i<100;i++)
     {  
       PWM_Set_Duty_Cycle(0,0,'B',i);
          for(j=1;j<50;j++)
          {
            
              Enable_Timer_Number(3,"Enable");
              GPTM_Delay_Unit(1,200,"Millisecond");
              counter = Timer_Number_Counts_Capture(3);
              speed = ((counter/8)*60000)/200;              
              char text[10];
              sprintf(text,"%0.3f,%d", speed,i);
              Send_Data_UART(text);
              New_Line_Message();                                
              Enable_Timer_Number(3,"Disable");
              Timer_Number_Reset_Counts(3);
          }     
          if(i==100)
          {
                 PWM_Set_Duty_Cycle(0,0,'B',0);
          }
     }
    }*/

}

void PWM_Intialization(void)
{
  CLK_PWM_Enable(0);
  CLK_Enable('B');
  PIN_FUNCTION_SELECT('B',7,"Alternative");
  PIN_CONFIGURE('B',7,"PWM");
  PIN_ANALOG_DIGITAL('B',7,"Digital");
  Use_PWM_Divisor(80,40);
  PWM_Generator_Enable(0,0,"Disable");
  PWM_Action_For_Load(0,0,'B',"LOW");
  PWM_Action_Comparator_Down(0,0,'B',"HIGH");
  PWM_Set_Frequency(0,0,10000,40);
  PWM_Set_Duty_Cycle(0,0,'B',0);
  PWM_Generator_Enable(0,0,"Enable");
  PWM_Module_Start(0);
}
void GPTM_Number_1_Intialization(void)
{
  CLK_Enable_GPTM_Module(1);
  Enable_Timer_Number(1,"Disable");
  Timer_Number_Option(1,"16");
  Timer_Number_Mode(1,"periodic");           //Enable or Disable Analog and Digital
  Timer_Number_Prescaler(1,80000000,320000);
}
void GPTM_Number_3_Intialization(void)
{
  CLK_Enable('B');
  PIN_INPUT_OUTPUT('B',2,"Input");
  PIN_ANALOG_DIGITAL('B',2,"Digital");
  PIN_FUNCTION_SELECT('B',2,"Alternative");
  PIN_CONFIGURE('B',2,"Capture");

  CLK_Enable_GPTM_Module(3);
  Enable_Timer_Number(3,"Disable");
  Timer_Number_Option(3,"16");
  Timer_Number_Mode(3,"capture");           //Enable or Disable Analog and Digital
  Timer_Number_Count_limit(3,65000);
  Timer_Number_Prescaler(3,80000000,400000);
  Timer_Number_Event_Mode(3,"Positive_Edge");
}

void PortA_Intialization(void)
{
  CLK_Enable('A');
  
  PIN_ANALOG_DIGITAL('A',7,"Digital");
  PIN_ANALOG_DIGITAL('A',6,"Digital");
  
  PIN_FUNCTION_SELECT('A',7,"General I/O");
  PIN_FUNCTION_SELECT('A',6,"General I/O");
  
  PIN_INPUT_OUTPUT('A',7,"Output");
  PIN_INPUT_OUTPUT('A',6,"Output");
  
  PIN_WRITE_DIGITAL('A',7,"HIGH");
  PIN_WRITE_DIGITAL('A',6,"LOW");
}

void UART_Intialization(void)
{

	CLK_Enable_UART_Module(0);                                                //Enable Clock for UART0
	Enable_UART_Module(0,"Disable");                                          //Disable UART0 
  Baud_Rate_UART_Module(0,9600,80);                                            //Specify 9600 Baud rate
  Word_Length_UART_Module(0,8);                                             //Specify 8Bits word length
	Enable_UART_Module(0,"Enable");                                           //Enable UART0
	PIN_ANALOG_DIGITAL('A',0,"Digital");                                      //Enable Digital For Pin A0
  PIN_ANALOG_DIGITAL('A',1,"Digital");                                      //Enable Digital For Pin A1
  PIN_FUNCTION_SELECT('A',0,"Alternative");                                 //Select Alternative Function for pin A0
  PIN_FUNCTION_SELECT('A',1,"Alternative");                                 //Select Alternative Function for pin A1
  PIN_CONFIGURE('A',0,"UART");                                              //Configure A0 as UART
  PIN_CONFIGURE('A',1,"UART");                                              //Configure A1 as UART
}

void Send_Data_UART(char Data[10])
{
          //char message1[]={"To Turn Red on Press R"};
          int length = strlen(Data);
          for(int i=0;i<length;i++)
          {
            Transmit_UART_Module(0,Data[i]);
          }
          
}
void New_Line_Message(void)
{
          Transmit_UART_Module(0,10);   
          Transmit_UART_Module(0,13); 
}