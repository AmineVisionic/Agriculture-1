#include "main.h"

const uint32_t Administrator_ID = 0x9276381B;
const uint32_t Initial=0x00000000;
u8 Temp_flag = 1 ;
u8 Show_flag = 0;
u8 Irrigation_flag=0;   //����־
u8 Exhaust_flag=0;      //�ŷ��־

float Illumination=0;
char temp[10];
char Humi[10];
char lighting[10];

u32 ID_Card[256];                       //���ڴ洢¼��Ŀ���

u8 Mode;
u8 Door_Flag=0;



int main(void)
{
        System_Init();                  //ϵͳ��ʼ��
        printf("[%s][%d]\r\n", __func__, __LINE__);
        
        /*************************�ֲ������Ķ���**************************/
        u8 Step=0;
        u8 Entering_Flag=1;
        
        uint32_t New_Card =0x22222222;      //�µĿ��ţ�������ʱ�洢��Ҫ¼��Ŀ���
        uint32_t New_Card1=0x88888888;      //�µĿ��ţ�������ʱ�洢��Ҫ¼��Ŀ���
        uint32_t New_Card2=0x66666666;      //�µĿ��ţ�������ʱ�洢��Ҫ¼��Ŀ���
        uint32_t *p_Pos= (uint32_t *)(Pos_Address);
        uint32_t  Card_Pos=*p_Pos;
        uint32_t *p_Card;
        u8 i;           //���ڼ���Կ�׿��ѭ�������Լ�������ѭ������
        
        
//        The_System_First_Run();                //��������
        
        //�� flash ����� ID �ż��ص��ڴ�����
        printf("\r\n��ǰ��һ���յ�ַΪ%d\r\n",*p_Pos);
        for(i=0;i<Card_Pos;i++)
        {
                p_Card = (uint32_t *)(Card_Address+i*4);
                ID_Card[i]=*p_Card;
                printf("IDΪ%8X\r\n",ID_Card[i]);
        }

       
        Mode = General_Mode;
        while(1)
        {
                
                delay_ms(500);
                
                New_Card=RFID_Number();
                
                if(  Administrator_ID == New_Card && Mode==General_Mode && Step==0 )
                {
                        Step=1;
                        Mode=Entering_Mode;
                        delay_ms(3000);
                }
                
                switch(Mode)
                {
                        case Abnormal_Mode:             //�쳣ģʽ
                                
                        break ;
                        
                        case Entering_Mode:             //¼��ģʽ
                                        printf("[%s][%d]\r\n", __func__, __LINE__);
                                        if(New_Card!=Administrator_ID && New_Card!=Initial && Step==1)
                                        {
                                                New_Card1=New_Card;
                                                Step=2;
                                                printf("New_Card1:%08X  ",New_Card1);
                                         }
                                        if(New_Card==Administrator_ID && Step==2)
                                        {
                                                Step=3;
                                                printf("New_Card2:%08X  ",New_Card2);
                                        }
                                        if(New_Card!=Administrator_ID && New_Card!=Initial && Step==3)
                                        {
                                                New_Card2=New_Card;
                                                Step=4;
                                                printf("New_Card2:%08X  ",New_Card2);
                                        }

                                        if(New_Card1==New_Card2 && Step==4)
                                        {
                                               
                                                for (i=0;i<Card_Pos;i++)
                                                {
                                                        if(New_Card2==ID_Card[i])
                                                        {
                                                                printf("�˿���¼��\r\n");
                                                                Entering_Flag=0;
                                                                break;
                                                        }
                                                }
                                                if(Entering_Flag!=0)
                                                {
                                                        ID_Card[Card_Pos]=New_Card2;
                                                        FLASH_Unlock();
                                                        FLASH_ProgramWord(Card_Address+Card_Pos*4,New_Card2);
                                                        FLASH_ErasePage(Pos_Address);
                                                        FLASH_ProgramWord(Pos_Address,Card_Pos+1);
                                                        FLASH_Lock();
                                                        Card_Pos=Card_Pos+1;
                                                        printf("¼��ɹ�\r\n");
                                                        printf("�µĿ���Ϊ��%08X\r\n",New_Card2);
                                                }
                                                Entering_Flag=1;
                                                
                                                Step=0;
                                                Mode= General_Mode;
                                        }
                                        if(New_Card1!=New_Card2 && Step==4)
                                        {
                                                printf("���ſ���һ��\r\n");
                                                Step=0;
                                                Mode= General_Mode;
                                        }
                        break ;
                        
                        case General_Mode:              //��ͨģʽ
                                if(Temp_flag!=0)
                                {
                                        Temperature_System();   //���ú���ϵͳִ�к���
                                }
                                for(i=0;i<Card_Pos;i++)
                                {
                                        if(New_Card==ID_Card[i])
                                        {
                                                Door_Flag=1;
                                                break;
                                        }
                                }
                                
                                if(Door_Flag!=0)
                                {
                                        TIM3->CCR3=25;    //180��
                                }
                                
                                else
                                {
                                        TIM3->CCR3=5;    //0��
                                }
                        break ;
                        default :
                                break;
                }
//                delay_ms(2000);
//                ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; 
//                printf("��ѹ=%f\n",ADC_ConvertedValueLocal);
//                delay_ms(500);
//                delay_ms(500);
//                
                
                /* �������ڽ��յ��ַ���  */
//                if( USART2_IT_Flag != 0 )
//                {
////                        Receave_System();
//                        USART2_IT_Flag=0;
//                }
        }
}




//void Network_System(void)
//{
//        /*װ�� JSON ��ʽ�ַ���*/
//        
//        if(Show_flag==0)
//        {
//                sprintf(temp,"%d.%d",DHT11_Data.temp_int,DHT11_Data.temp_deci);
//                sprintf(Humi,"%d.%d",DHT11_Data.humi_int,DHT11_Data.humi_deci);
//                sprintf(lighting,"%0.2f",Illumination);
//                OLED_Fill(0x00);
//                OLED_ShowCN(32,0,4,title);      //��ʾ����----�ǻ�ũҵ
//                OLED_ShowCN(0,2,3,temp_code);   //��ʾ�¶�
//                OLED_ShowCN(0,4,3,Humi_code);   //��ʾʪ��
//                OLED_ShowCN(0,6,3,light);       //��ʾ��ǿ
//                OLED_ShowStr(49,2,temp,2);
//                OLED_ShowStr(49,4,Humi,2);
//                OLED_ShowStr(49,6,lighting,2);
//                Show_flag=~Show_flag;
//        }
//        else
//        {
////                sprintf();
//                OLED_Fill(0x00);
//                OLED_ShowCN(32,0,4,title);      //��ʾ����----�ǻ�ũҵ
//                OLED_ShowCN(0,2,2,Air_Temp);
//                OLED_ShowCN(0,4,2,Irrigation);
//                OLED_ShowCN(0,6,2,Exhaust);
//                OLED_ShowCN(33,2,3,System);
//                OLED_ShowCN(33,4,3,System);
//                OLED_ShowCN(33,6,3,System);
//                if(Temp_flag!=0)
//                {
//                        OLED_ShowCN(97,2,2,Open);
//                }
//                else
//                {
//                        OLED_ShowCN(97,2,2,Close);
//                }
//                if(Irrigation_flag!=0)
//                {
//                        OLED_ShowCN(97,4,2,Open);
//                }
//                else
//                {
//                        OLED_ShowCN(97,4,2,Close);
//                }
//                if(Exhaust_flag!=0)
//                {
//                        OLED_ShowCN(97,6,2,Open);
//                }
//                else
//                {
//                        OLED_ShowCN(97,6,2,Close);
//                }
//                Show_flag=~Show_flag;
//        }
//        
//}

void  Temperature_System (void)
{
        if( Temperature > temp_max )
        {
                         //�ر�̫���Ʋ�����ˮ�����н���
                TemDown_ON;
        }
        else if ( Temperature < ((temp_max+temp_min)/2) && Temperature > temp_min )
        {
                TemDown_OFF;
        }
        else if ( Temperature > ((temp_max+temp_min)/2) && Temperature < temp_max )
        {
                LED_OFF ;
        }
        else if ( Temperature < temp_min )
        {
                      //�ر�ˮ��������̫���ƽ�������
                LED_ON;
        }
}
void System_Init(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();     //usart1
        DHT11_Init();
        USART_Config();    //usart2
        LED_GPIO_Config();
        BASIC_TIM_Init();
        GENERAL_TIM_Init();
        I2C_init();
        OLED_Init();
        TSL2561_Init();
        spi_Init();
        ADC1_Init();
        RC522_Init();
}

uint32_t RFID_Number ( void )
{
        u8 Array_ID [ 4 ];                  //�Ⱥ���IC�������ͺ�UID(IC�����к�)
        u8 Status,i;
        u32 Card_Number=Initial;
        if ( ( Status = PcdRequest ( PICC_REQALL, Array_ID ) ) != MI_OK )       //Ѱ��
        {
                Status = PcdRequest ( PICC_REQALL, Array_ID );
        }
        if(Status==MI_OK)
        {
                if ( PcdAnticoll ( Array_ID ) == MI_OK )
                {
                        for(i=0;i<4;i++)
                        Card_Number=(Card_Number<<8)|Array_ID[i];
                        printf("card:%08X\r\n",Card_Number);
                }
        }
        return Card_Number;
}

void The_System_First_Run( void )  //оƬ�״�����ʱ��Ҫ���������еĴ�����ִ�д˺�������flash ����д����Ӧ������
{
        uint32_t * P;
        FLASH_Unlock();                 //�˺���һ������ԭ��¼���Կ�׺Ž��ᱻɾ��   ����
        FLASH_ErasePage(Card_Address);
        FLASH_ErasePage(Pos_Address);
        FLASH_ProgramWord(Card_Address,Administrator_ID);
        FLASH_ProgramWord(Pos_Address,0x00000001);
        FLASH_Lock();
        P = (uint32_t *)(Card_Address);
        printf("\r\n����Ա����Ϊ:%8X",*P);
        P = (uint32_t *)(Pos_Address);
        printf("\r\n��ǰ��һ���յ�ַΪ:%d",*P);
}









/*  json ��ʽ��װ��ʽ  */
//{"Temperature":"23.00","Humidity":"59.00","CO2":"21.6","PH":"4.6","Illumination":"6.5"}
//Message: {"method":"setMotor1","params":false}
//Message: {"method":"setCarPos","params":"73"}
//Message: {"method":"setHydrovalve1","params":false}
//                printf("[%s][%d]", __func__, __LINE__);
//setMotor1_true
//setHydrovalve1_false
