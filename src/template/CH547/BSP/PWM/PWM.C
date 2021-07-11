/********************************** (C) COPYRIGHT *******************************
* File Name          : PWM.C
* Author             : WCH
* Version            : V1.0
* Date               : 2018/08/29
* Description        : CH549 PWM�������ѡ���жϴ�����PWM ѭ�����ڽ����жϣ�
*                      ����           ����
*                      P25            PWM0
*                      P24            PWM1
*                      P23            PWM2
*                      P22            PWM3
*                      P21            PWM4
*                      P20            PWM5
*                      P26            PWM6
*                      P27            PWM7
*******************************************************************************/
#include ".\PWM\PWM.H"
#pragma  NOAREGS
/*******************************************************************************
* Function Name  : PWM_CFG_CHANNEL()
* Description    : PWMͨ�����ʹ��,����IO�˿�����
* Input          : Channel��ͨ���ţ�λ���ʾ
*                  NewState��0:�ر�ͨ��  1:����ͨ��
* Output         : None
* Return         : None
*******************************************************************************/
void PWM_SEL_CHANNEL(UINT8 Channel,UINT8 NewState)
{
    UINT8 i;
    /* ͨ������ */
    if(NewState == Enable)                    //�������
    {
        PWM_CTRL &= ~bPWM_CLR_ALL;
        if(Channel&CH0)
        {
            PWM_CTRL |= bPWM0_OUT_EN;
        }
        if(Channel&CH1)
        {
            PWM_CTRL |= bPWM1_OUT_EN;
        }
        PWM_CTRL2 = (Channel>>2);
        /* ��Ӧ��GPIO�����ó�����ģʽ */
        for(i=0; i!=6; i++)
        {
            if(Channel & (1<<i))
            {
                P2_MOD_OC &= ~(1<<(5-i));
                P2_DIR_PU |= (1<<(5-i));
            }
        }
        if(Channel&CH6)
        {
            P2_MOD_OC &= ~CH6;
            P2_DIR_PU |= CH6;
        }
        if(Channel&CH7)
        {
            P2_MOD_OC &= ~CH7;
            P2_DIR_PU |= CH7;
        }
    }
    else                                      //����ر�
    {
        if(Channel&CH0)
        {
            PWM_CTRL &= ~bPWM0_OUT_EN;
        }
        if(Channel&CH1)
        {
            PWM_CTRL &= ~bPWM1_OUT_EN;
        }
        PWM_CTRL2 &= ~(Channel>>2);
    }
}
#ifdef PWM_INTERRUPT
/*******************************************************************************
* Function Name  : PWMInterruptEnable()
* Description    : PWM�ж�ʹ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PWMInterruptEnable()
{
    PWM_CTRL |= bPWM_IF_END;                                                  //���PWM�ж�
    IE_PWMX = 1;
    EA = 1;
}
/*******************************************************************************
* Function Name  : PWMInterrupt(void)
* Description    : PWM�жϷ������ FREQ_SYS/n/64��FREQ_SYS/n/256 ���ж�
*******************************************************************************/
void PWMInterrupt( void ) interrupt INT_NO_PWMX using 1                       //PWM0~8�жϷ������,ʹ�üĴ�����1
{
    static UINT8 duty = 0;
    PWM_CTRL |= bPWM_IF_END;                                                  //���PWM�ж�

    /* ��ѡ��,��������ռ�ձ� */
    SetPWM0Dat(duty);
    SetPWM1Dat(duty);
    SetPWM2Dat(duty);
    duty++;

}
#endif