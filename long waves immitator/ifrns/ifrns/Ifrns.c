/*
 * ifrns.c
 *
 * Created: 18.10.2011 18:22:16
 *  Author: Admin
 */ 
#include "All.h"

//������� ����***********************************************************
unsigned char phaseCodesMasterA	= phaseCodeMasterADefault;
unsigned char phaseCodesMasterB	= phaseCodeMasterBDefault;
unsigned char phaseCodesSlaveA	= phaseCodeSlaveADefault;
unsigned char phaseCodesSlaveB	= phaseCodeSlaveBDefault;
unsigned long phaseCodesSecLabel= phaseCodesSecLabel0Default;

int main(void)
{	
	InitializeSoftwareSystem(
							  Rns_e, 
							  AdvancedFOff,
							  Loran,
							  phaseCodesMasterA,
							  phaseCodesMasterB,
							  phaseCodesSlaveA,
							  phaseCodesSlaveB,
							  phaseCodesSecLabel,
							  rns_vCode0,
							  rns_vCode1,
							  rns_vCode2,
							  rns_vCode3,
							  5
							  );
							  
	InitializeHardwareSystem(CLK_SCLKSEL_PLL_gc,	//�������� �����
							 OSC_XOSCEN_bm,			//�������� �����
							 OSC_PLLSRC_XOSC_gc,	//�������� ����
							 2,						//����� �����
							 64,					//�������� ������ ����. ��������� 
							 0,						//OSC_XOSCSEL0_bm|OSC_XOSCSEL1_bm,		//�������� ������ ��� �������� ���������
							 PORTJADDR,				//����0 ������
							 PORTKADDR,				//����1 ������
							 PORTQADDR,				//����2 ������
							 PORTRADDR,				//����� ����
							 PORTHADDR,				//���� �������� ����������
							 2						//��� �������� ����������
							);

    for(;;)
    {		
		//������ - �� ������?
		ExchangeServer();
		ShiftServer();
		//������ ����, ����� �� ������ DMA
		if(SLEEP.CTRL == 0)
		{
			SLEEP.CTRL = 0x01;
			asm("SLEEP");
		}	   
    }
}

