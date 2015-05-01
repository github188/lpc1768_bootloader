##==========================================================================
##                   LPC-bootloader
##
##              Copyright(c) 2015-2016 Yoc Ltd. All rights reserved.
##
##--------------------------------------------------------------------------
## File Description
## ----------------
##      lpc1768 makeconfig definition.
##
##--------------------------------------------------------------------------
##
##  Created by   :     yoc
##        Date   :     2015.04.29
##==========================================================================


#-------------------------------------------------------------------------------
# ������·�� ��Ҫ���ú�·�� ��Ҫ����newlib ���п����
#-------------------------------------------------------------------------------
#linux�µ�·��
#GNU_DIR=/usr/local/CodeSourcery/Sourcery_G++_Lite/arm-none-eabi

#Windows�µ�·��  ʹ�� cygwin make����
GNU_DIR=D:/CodeSourcery/Sourcery\ G++\ Lite/arm-none-eabi


#CROSS_COMPILE :=arm-uclinuxeabi-

CROSS_COMPILE =arm-none-eabi-

#
# Include the make variables (CC, etc...)
#
AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
CPP	= $(CC) -E
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
LDR	= $(CROSS_COMPILE)ldr
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)RANLIB




