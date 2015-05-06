/*==========================================================================
##                   LPC-bootloader
##
##              Copyright(c) 2015-2016 Yoc Ltd. All rights reserved.
##
##--------------------------------------------------------------------------
## File Description
## ----------------
##      uart0.h lpc1768ͨ��ͷ�ļ� ����ֲ���
##
##--------------------------------------------------------------------------
##
##  Created by   :     yoc
##        Date   :     2015.04.29
##==========================================================================*/

#include "config.h"
#include "board.h"
#include <string.h>

#define	_DEBUG_TRACE
//#define	UART0_BPS			9600
#define	UART0_BUF_LEN		1048

volatile uint8 uart0_buf[UART0_BUF_LEN];
volatile uint8 uart0_rd;
volatile uint8 uart0_wr;

/*********************************************************************************************************
** Function name:     	InitUart0
** Descriptions:	    ���ڳ�ʼ��������Ϊ8λ����λ��1λֹͣλ������żУ�飬������Ϊ9600
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uart0_init (uint32 baud)
{
    uint32 ulFdiv; 
	LPC_SC->PCONP  |= (1 << 3);              	/* Enable power to Uart0 block  */
	LPC_PINCON->PINSEL0 &= ~(0x0F << 4);   
	LPC_PINCON->PINSEL0 |= (0x01 << 6) | (0x01 << 4); //���ùܽ�Ϊ���ڹ���
	LPC_PINCON->PINMODE0 &= ~(0x0F << 4); //���� 
    LPC_UART0->LCR = 0x83;                                                  //�������ò�����
    ulFdiv = (FPCLK / 16) / baud;                              //���ò�����
    LPC_UART0->DLM  = ulFdiv / 256;
    LPC_UART0->DLL  = ulFdiv % 256; 
    LPC_UART0->LCR  = 0x03;                                                  //����������
    LPC_UART0->FCR  = 0x87;                                                  //ʹ��FIFO������8���ֽڴ�����
    LPC_UART0->IER  = 0x01;                                                  //ʹ�ܽ����ж�

	NVIC_EnableIRQ(UART0_IRQn);

	uart0_rd  = 0;
	uart0_wr  = 0;
	memset((void *)uart0_buf,0x00,UART0_BUF_LEN);				//��ʼ��������

}
/*********************************************************************************************************
* Function Name:        uart0_isrHandler
* Description:          UART0 �жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void UART0_IRQHandler (void)
{

	uint8 i,temp;
	//__disable_irq();  
	
    while((LPC_UART0->IIR & 0x01) == 0)                          			//�ж��Ƿ����жϹ���
	{
        switch(LPC_UART0->IIR & 0x0E) 										//�ж��жϱ�־
		{                                         
            case 0x04 : for (i = 0; i < 8; i++)				//���������ж�
						{
		                	temp = LPC_UART0->RBR;
							uart0_buf[uart0_wr++] = temp;
							uart0_wr = uart0_wr % UART0_BUF_LEN;
		                }
		                break;
            case 0x0C : while((LPC_UART0->LSR & 0x01) == 0x01)				//�ַ���ʱ�жϣ��ж������Ƿ�������
						{                         
		                	temp = LPC_UART0->RBR;
							uart0_buf[uart0_wr++] = temp;
					       	uart0_wr = uart0_wr % UART0_BUF_LEN;
		                }
		                break;
            default	: break;
        }
    }
   // __enable_irq();
  

}
/*********************************************************************************************************
** Function name:	    uart0_putChar
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    ch:Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uart0_putChar(uint8 ch)
{
    LPC_UART0->THR = ch;                                                      //д������
    while((LPC_UART0->LSR & 0x20) == 0);                                      //�ȴ����ݷ������
}

/*********************************************************************************************************
** Function name:	    Uart0PutStr
** Descriptions:	    �򴮿ڷ����ַ���
** input parameters:    Str:  Ҫ���͵��ַ���ָ��
**                      Len:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uart0_putStr(uint8 *str, uint32 len)
{
    uint32 i;  
    for (i=0;i<len;i++) {
        uart0_putChar(*str++);
    }
}
/*********************************************************************************************************
** Function name:	    uart0_IsEmpty
** Descriptions:	    �жϻ������Ƿ�ǿ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      1���ǿգ�0����
*********************************************************************************************************/
uint8 uart0_IsEmpty(void)
{
	return (uart0_rd == uart0_wr);
}


/*********************************************************************************************************
** Function name:     	uart0_clear
** Descriptions:	    �������0���ջ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uart0_clear(void) 
{
	uart0_rd = 0;
	uart0_wr = 0;
}	



/*********************************************************************************************************
** Function name:	    uart0_getChar
** Descriptions:	    �Ӵ��ڻ�������ȡһ���ֽڣ���֮ǰ��Ҫ�жϻ������ǿ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��ȡ��������
*********************************************************************************************************/
uint8 uart0_getChar(void)
{
    uint8 ch;
    ch = uart0_buf[uart0_rd++];
	uart0_rd = uart0_rd % UART0_BUF_LEN;
	return ch;
}


/**************************************End Of File*******************************************************/
