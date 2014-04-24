/*
 * ifrns.c
 *
 * Created: 18.10.2011 18:22:16
 *  Author: Admin
 */ 
#include "All.h"

//‘азовые коды***********************************************************
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
							  
	InitializeHardwareSystem(CLK_SCLKSEL_PLL_gc,	//источник опоры
							 OSC_XOSCEN_bm,			//источник осцил
							 OSC_PLLSRC_XOSC_gc,	//источник фапч
							 2,						//коэфф умнож
							 64,					//диапазон частот внеш. источника 
							 0,						//OSC_XOSCSEL0_bm|OSC_XOSCSEL1_bm,		//диапазон частот дл€ внешнего источника
							 PORTJADDR,				//порт0 данные
							 PORTKADDR,				//порт1 данные
							 PORTQADDR,				//порт2 данные
							 PORTRADDR,				//строб порт
							 PORTHADDR,				//порт внешнего прерывани€
							 2						//пин внешнего прерывани€
							);

    for(;;)
    {		
		//пришло - не пришло?
		ExchangeServer();
		ShiftServer();
		//усыпим €дро, чтобы не мешать DMA
		if(SLEEP.CTRL == 0)
		{
			SLEEP.CTRL = 0x01;
			asm("SLEEP");
		}	   
    }
}

