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
#define	UART0_BUF_LEN		128

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
void uart0_isrHandler (void)
{
	#if 0
	unsigned char Num;
	unsigned char rxd_head;
	unsigned char rxd_data;
	
	//OS_ENTER_CRITICAL();
	OSIntEnter();      
    while((U0IIR & 0x01) == 0)                          			//�ж��Ƿ����жϹ���
	{
        switch(U0IIR & 0x0E) 										//�ж��жϱ�־
		{                                         
            case 0x04 : for (Num = 0; Num < 8; Num++)				//���������ж�
						{
		                	rxd_data = U0RBR;
							rxd_head = (Uart0RxdHead + 1);
					        if( rxd_head >= UART0BUFFERLEN ) 
					           	rxd_head = 0;
					        if( rxd_head != Uart0RxdTail) 
					        {
					           	Uart0RevBuff[Uart0RxdHead] = rxd_data;
					           	Uart0RxdHead = rxd_head;
					        }
		                }
		                break;
            case 0x0C : while((U0LSR & 0x01) == 0x01)				//�ַ���ʱ�жϣ��ж������Ƿ�������
						{                         
		                	rxd_data = U0RBR;
							rxd_head = (Uart0RxdHead + 1);
					        if( rxd_head >= UART0BUFFERLEN ) 
					           	rxd_head = 0;
					        if( rxd_head != Uart0RxdTail) 
					        {
					           	Uart0RevBuff[Uart0RxdHead] = rxd_data;
					           	Uart0RxdHead = rxd_head;
					        }
		                }
		                break;
            default	: break;
        }
    }
    OSIntExit();
    //OS_EXIT_CRITICAL();
	#endif
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
