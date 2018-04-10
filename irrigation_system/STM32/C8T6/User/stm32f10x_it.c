#include "stm32f10x_it.h"
#include "main.h"

u8  USart8266_temp[200];

u8 Humi_int =0 ; 
u8 Humi_deci=0 ;
u8 Temp_int =0 ; 
u8 Temp_deci=0 ;




static u8 count = 0;
u8 Time_Flag=0;
u16 time=0;
u16 time_mode=0;

void USART2_IRQHandler ( void )
{ 
        if ( USART_GetITStatus ( USART2, USART_IT_RXNE ) != RESET )
        {
                USart8266_temp[count]  = USART_ReceiveData( USART2 );
                count++;
                if(USART_ReceiveData( USART2 ) == '*')
                {
                        count = 0;
                        Down_Control();
                }
        }
}

void  BASIC_TIM_IRQHandler (void)
{
        if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
        { 
                time++;
                if(time>=2500)           //��ʱ2.5���ϴ�һ������
                {
                        time=0;
                        Updata();
                }
                if(Mode==Entering_Mode) //һ�����˳�����Ա¼��ģʽ
                {
                        time_mode++;
                        if(time_mode>=60000)
                        {
                                time_mode=0;
                                Mode=General_Mode;
                        }
                }
                
                TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update); 
        } 
}

void ADC_IRQHandler(void)
{ 
        if (ADC_GetITStatus(ADCx,ADC_IT_EOC)==SET) 
        {
                // ��ȡADC��ת��ֵ
                ADC_ConvertedValue = ADC_GetConversionValue(ADCx);
        }
        ADC_ClearITPendingBit(ADCx,ADC_IT_EOC);
}


void Updata (void)
{
        DHT11_Data_TypeDef   DHT11_Data;
        char DataStr [ 500 ]  ;
        
        
        Illumination=Read_Light();
        
        if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
        {
                Temp_int=DHT11_Data.temp_int;
                Temp_deci=DHT11_Data.temp_deci;
                Humi_int=DHT11_Data.humi_int;
                Humi_deci=DHT11_Data.humi_deci;
//                sprintf ( DataStr,"{\"Temperature\":\"%d.%d\",\"Humidity\":\"%d.%d\",\"Illumination\":\"%0.2f\"}",\
//                DHT11_Data.temp_int,DHT11_Data.temp_deci,DHT11_Data.humi_int,DHT11_Data.humi_deci,Illumination);
        }
        sprintf ( DataStr,"{\"Temperature\":\"%d.%d\",\"Humidity\":\"%d.%d\",\"Illumination\":\"%0.2f\"}",\
                Temp_int,Temp_deci,Humi_int,Humi_deci,Illumination);
        printf("%s\r\n",DataStr);
        Usart2_SendString(USART2,DataStr);
}


void Down_Control(void)
{
        
        char *p;
        char *name=NULL;
        char *status=NULL;
        
        printf("receave %s",USart8266_temp);
        p = strtok((char*)USart8266_temp, "_");
        if(p)
        {
                name = p;
        }
        p = strtok(NULL, "_");
        if(p)
        {
                status = p;
        }
        if(! (strcmp("setMotor1", name) || strcmp("true", status) ) ) 
        {
                //LED1_ON;   //�����Ҫ�Ĵ���
                //Exhaust_flag=1;
        }
        if(! (strcmp("setMotor1", name) || strcmp("false", status) ) ) 
        {
                //Water_OFF;  //�����Ҫ�Ĵ���
                //Exhaust_flag=0;
        }
        if(! (strcmp("setHydrovalve1", name) || strcmp("true", status) ) ) 
        {
                Water_ON;  //�����Ҫ�Ĵ���
                Irrigation_flag=1;
        }
        if(! (strcmp("setHydrovalve1", name) || strcmp("false", status) ) ) 
        {
                Water_OFF;   //�����Ҫ�Ĵ���
                Irrigation_flag=0;
        }
        
        /*
         if()   //�û��ر�����ϵͳ
        {
                Temp_flag = 0 ;
        }
        if()   //�û���������ϵͳ
        {
                Temp_flag = 1 ;
        }
        if()   //���������ƶ����õ���ֵ
        {
                temp_max =  ;
        }
        if()    //���������ƶ����õ���ֵ
        {
                temp_min =  ;
        }
        
        */

}







































/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
        printf("[%s][%d]", __func__, __LINE__);
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
        /* Go to infinite loop when Hard Fault exception occurs */
        printf("[%s][%d]", __func__, __LINE__);
        
        while (1)
        {
          
        }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
        /* Go to infinite loop when Memory Manage exception occurs */
        
        printf("[%s][%d]", __func__, __LINE__);
        
        while (1)
        {
                
        }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
        /* Go to infinite loop when Bus Fault exception occurs */
        
        printf("[%s][%d]", __func__, __LINE__);
        
        while (1)
        {
                
        }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
        /* Go to infinite loop when Usage Fault exception occurs */
        
        printf("[%s][%d]", __func__, __LINE__);
        
        while (1)
        {
                
        }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
        printf("[%s][%d]", __func__, __LINE__);
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
        printf("[%s][%d]", __func__, __LINE__);
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
        printf("[%s][%d]", __func__, __LINE__);
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
        
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
