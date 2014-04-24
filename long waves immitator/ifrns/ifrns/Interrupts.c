/*
 * Interrupts.c
 *
 * Created: 14.11.2011 15:40:27
 *  Author: Admin
 */ 
#include "All.h"

extern unsigned char form[arrLeng];
extern unsigned char formPi[arrLeng];
extern unsigned char form5[51];
extern unsigned char form5Pi[51];
unsigned char needLabel;
unsigned char dacValCou;
unsigned int dacVal;
volatile unsigned int counterRnsv = 0;

volatile unsigned long ph;
volatile unsigned char shiftRnsv;
//таймер F
ISR(TCF0_CCA_vect)
{
	//if(workStruct.formStatus == NMakingMask || workStruct.predFormStatus == labelMaster)
	//TCE0.CTRLA |= 3;
}

ISR(TCF0_OVF_vect)
{
	//PORTK.OUTCLR = 1;
	//формируем пачку
		switch (workStruct.formStatus)
			{
				case NMakingMask:
				{//формирование пачки
					workStruct.count++;
					
					if(workStruct.shift == false && workStruct.count < 3){
						if(workStruct.ticks != 0){
							if(workStruct.ticks > 2)  {
								workStruct.step = workStruct.ticks/2;
								if(workStruct.step + stageStruct.perPack > 65536)
									workStruct.step = (workStruct.step+stageStruct.perPack-65536)/2;
								if(workStruct.step >= TCF0.CCA)
									workStruct.step = TCF0.CCA/2;
								if(workStruct.step ==  0) workStruct.step = 1;						
							}					
							else workStruct.step = 1;				
							//workStruct.step = 1000;
							TCF0.CCA+=workStruct.step;
							TCF0.PER+=workStruct.step;
							workStruct.shift = true;	
						}						
					}else if(workStruct.count < 3){
						TCF0.CCA -= workStruct.step;
						TCF0.PER -= workStruct.step;
						workStruct.ticks -= workStruct.step;
						workStruct.shift = false;
					}				
					
					//проверим если канал В включен, то импульс уже был - выключим канал
					if(TCF0.CTRLB & TC0_CCBEN_bm)
					{//выключим канал
						TCF0.CTRLB &= ~TC0_CCBEN_bm;
						//PORTH.OUTCLR |= 0x02;
					}
					//проверим если канал C включен, то импульс уже был - выключим канал
					if(TCF0.CTRLB & TC0_CCCEN_bm)
					{//выключим канал
						TCF0.CTRLB &= ~TC0_CCCEN_bm;
					}
					
					//импульс окраса рнс
LabelTime:			if(workStruct.phaseMS == master && workStruct.count == 7 && workStruct.countSec == stageStruct.nSec)
					{	
						//следующий импульс совпадет с секундой
						workStruct.countSec = 0;
						//включим канал В и запишем в компаратор значение
						TCF0.CTRLB |= TC0_CCBEN_bm;
						TCF0.CCB = stageStruct.ccaPack;
						
						PORTH.OUTCLR |= 0x02;
						counterRnsv = 0;
						//поставим признак, что нужен импульс окраса
						
						if(system.sysType == Rns_e) needLabel = label;
					}				
					//импульсы окраса рнс_в
					if(system.sysType == Rns_v && workStruct.count == 7 && workStruct.phaseMS == master && workStruct.countSec > (stageStruct.nSec - periodsNumTillSecRnsv - 1)  && workStruct.countSec < stageStruct.nSec)
					{	//поставим метку (127 штук)			
						//моргнем ногой
						PORTH.OUTSET |= 0x02;
						//if((PORTH.OUTCLR & 0x02) != 0)
						//{
							//PORTH.OUTCLR |= 0x02;
						//}						
						counterRnsv++;
						 	
						needLabel = label;
					}													
					if(system.sysType == Rns_e  && workStruct.count == 7 && workStruct.diagMode == minMode && workStruct.phaseMS == master)
					{//рисуем минуту						
						//проверим нарисовали ли все
						if(workStruct.minMode != minModeDefault-1)
						{//нет
							workStruct.minMode++;
							//сбросим, если надо
							//if(workStruct.minMode == minModeDefault-1) workStruct.minMode = 0;
							//поставим признак, что нужен импульс окраса
							needLabel = label;
						}					
						else
						{
							workStruct.minMode = 0;
						}																	
					}
					else if(system.sysType == Rns_e && workStruct.count == 7 && workStruct.diagMode == fiveMinMode && workStruct.phaseMS == master)
					{//рисуем 5 минут
						//проверим нарисовали ли все
						if(workStruct.fiveMinMode != fiveMinModeDefault-1)
						{//нет
							workStruct.fiveMinMode++;
							
							//if(workStruct.fiveMinMode == fiveMinModeDefault-1) workStruct.fiveMinMode = 0;
							//поставим признак, что нужен импульс окраса
							needLabel = label;
						}	
						else
						{
							workStruct.fiveMinMode = 0;
						}													
					}																
PackDone:			//проверим, нарисовали ли пачку
					if(workStruct.count == diagramStruct.n)
					{//сформировали нужное кол-во импульсов
							workStruct.count = 0;
							PORTF.DIRSET |= 0x0001;
							//формируем ничего
							workStruct.formStatus = none;
							//выкл ДМА 
							DMA_CH1_OFF();
							//разрешим передачу/прием
							recTransStatus |= (1<<TRANS_ENABLE);
							//normal
							TCF0.CTRLB = (TCF0.CTRLB & 0xF0);
							TCF0.CCA = 0x00;
							//проверим, ведущая или ведомая (будет ли 9ый импульс)
							if(workStruct.phaseMS == master)
							{//сформировали ведущую - добавим импульс опознавания ведущей
							 //зададим режим работы
								workStruct.predFormStatus = labelMaster;	
							}			
							//Проверим, что формировать					
							//зададим время до след ведомой
							if(workStruct.predFormStatus == slave1)
							{
								TCF0.PER = stageStruct.perNoNMasterSlave2;
								workStruct.predFormStatus = slave2;
								
							}
							else if(workStruct.predFormStatus == slave2)
							{
								TCF0.PER = stageStruct.perNoNMasterSlave3;
								workStruct.predFormStatus = slave3;
								
							}
							else if(workStruct.predFormStatus == slave3)
							{
								TCF0.PER = stageStruct.perNoNMasterSlave4;
								workStruct.predFormStatus = slave4;
								
							}
							else if(workStruct.predFormStatus == slave4)
							{
								TCF0.PER = stageStruct.perNoN;
								workStruct.predFormStatus = 0;
								
							}
							else if(workStruct.predFormStatus == 0)
							{
								TCF0.PER = stageStruct.perNoNMasterSlave1;
								workStruct.predFormStatus = slave1;
								
							}
							else if(workStruct.predFormStatus == labelMaster)
							{//формируем маркер опознавания ведущей
								TCF0.PER = stageStruct.perNoNLabelMaster;
							}								
							else if(workStruct.predFormStatus == label)
							{
								TCF0.PER = stageStruct.perNoNLabel;
							}
							else if(workStruct.predFormStatus == labelToSlave1)
							{
								TCF0.PER = stageStruct.perNonLabelNext;
							}
							
							
PhaseCalc:					//сбросим счетчик фаз сдвигов
							workStruct.phaseCount = 0;
							//Рассчитаем режим для след пачки
							if(workStruct.phaseMS == master)
							{//Формировали мастера. Будем формировать слэйв, если они есть. Период тот же
								//if(workStruct.predFormStatus == labelMaster)
								//{
									PORTH.OUTSET = 0x20;
									workStruct.phaseMS = slave;
								//}									
																	
							}
							else
							{//формировали слэйв. проверим нужно ли мастера формировать
								if(workStruct.predFormStatus == 0)
								{//след пачка - пачка ведущей. Поменяем период
									workStruct.phaseMS = master;
									PORTH.OUTCLR = 0x20;
									switch (workStruct.phasePer)
									{
										case a:workStruct.phasePer = b;break;
										case b:workStruct.phasePer = a;break;
									}				
								}					
							}
								
					}					
					break;
				}
				case none:
				{
					workStruct.countNoneSteps++;
	
						
					
					if(workStruct.predFormStatus == slave1)
					{
						if(workStruct.countNoneSteps == stageStruct.nNoNMasterSlave1)
						{//начинаем формировать заново
							workStruct.countNoneSteps = 0;
							workStruct.formStatus = NMakingMask;
							//стоп передача
							recTransStatus &= ~(1<<TRANS_ENABLE);
							//USARTD0.CTRLA &= ~(USART_RXCINTLVL_HI_gc);
							//single slope					
							TCF0.CTRLB = (TCF0.CTRLB & 0xF0)|TC0_WGMODE0_bm|TC0_WGMODE1_bm;
							//зададим период
							TCF0.PER = stageStruct.perPack;
							//зададим компаратор
							TCF0.CCA = stageStruct.ccaPack;
							if(!(diagramStruct.slaveN & 0b00000001))
							{
								PORTF.DIR &= ~0x0001;
							}
							else
							{
								PORTF.DIRSET |= 0x0001;
								//вкл ДМА
								DMA_CH1_ON();
							}
							
						}
					}
					else if(workStruct.predFormStatus == slave2)
					{
						if(workStruct.countNoneSteps == stageStruct.nNoNMasterSlave2)
						{//начинаем формировать заново
							workStruct.countNoneSteps = 0;
							workStruct.formStatus = NMakingMask;
							//стоп передача
							recTransStatus &= ~(1<<TRANS_ENABLE);
							//USARTD0.CTRLA &= ~(USART_RXCINTLVL_HI_gc);
							//single slope					
							TCF0.CTRLB = (TCF0.CTRLB & 0xF0)|TC0_WGMODE0_bm|TC0_WGMODE1_bm;
							//зададим период
							TCF0.PER = stageStruct.perPack;
							//зададим компаратор
							TCF0.CCA = stageStruct.ccaPack;
							if(!(diagramStruct.slaveN & 0b00000010))
							{
								PORTF.DIR &= ~0x0001;
							}
							else
							{
								PORTF.DIRSET |= 0x0001;
								//вкл ДМА
								DMA_CH1_ON();
							}
							
						}	
					}
					else if(workStruct.predFormStatus == slave3)
					{
						if(workStruct.countNoneSteps == stageStruct.nNoNMasterSlave3)
						{//начинаем формировать заново
							workStruct.countNoneSteps = 0;
							workStruct.formStatus = NMakingMask;
							//стоп передача
							recTransStatus &= ~(1<<TRANS_ENABLE);
							//USARTD0.CTRLA &= ~(USART_RXCINTLVL_HI_gc);
							//single slope					
							TCF0.CTRLB = (TCF0.CTRLB & 0xF0)|TC0_WGMODE0_bm|TC0_WGMODE1_bm;
							//зададим период
							TCF0.PER = stageStruct.perPack;
							//зададим компаратор
							TCF0.CCA = stageStruct.ccaPack;
							if(!(diagramStruct.slaveN & 0b00000100))
							{
								PORTF.DIR &= ~0x0001;
							}
							else
							{
								PORTF.DIRSET |= 0x0001;
								//вкл ДМА
								DMA_CH1_ON();
							}
							
						}	
					}
					else if(workStruct.predFormStatus == slave4)
					{
						if(workStruct.countNoneSteps == stageStruct.nNoNMasterSlave4)
						{//начинаем формировать заново
							workStruct.countNoneSteps = 0;
							workStruct.formStatus = NMakingMask;
							//стоп передача
							recTransStatus &= ~(1<<TRANS_ENABLE);
							//USARTD0.CTRLA &= ~(USART_RXCINTLVL_HI_gc);
							//single slope					
							TCF0.CTRLB = (TCF0.CTRLB & 0xF0)|TC0_WGMODE0_bm|TC0_WGMODE1_bm;
							//зададим период
							TCF0.PER = stageStruct.perPack;
							//зададим компаратор
							TCF0.CCA = stageStruct.ccaPack;
							if(!(diagramStruct.slaveN & 0b00001000))
							{
								PORTF.DIR &= ~0x0001;
							}
							else
							{
								PORTF.DIRSET |= 0x0001;
								//вкл ДМА
								DMA_CH1_ON();
							}
							
						}	
					}
					else if(workStruct.predFormStatus == 0)
					{
						if(workStruct.countNoneSteps == stageStruct.nNoN)
						{//начинаем формировать заново
						 //следующая пачка будет пачкой ведущей станции
						 //увеличим счетчик секундный (кол-во периодов в сек)
							workStruct.countSec++;
							workStruct.countNoneSteps = 0;
							workStruct.formStatus = NMakingMask;
							//стоп передача
							recTransStatus &= ~(1<<TRANS_ENABLE);
							//USARTD0.CTRLA &= ~(USART_RXCINTLVL_HI_gc);
							//single slope					
							TCF0.CTRLB = (TCF0.CTRLB & 0xF0)|TC0_WGMODE0_bm|TC0_WGMODE1_bm;
							//зададим период
							TCF0.PER = stageStruct.perPack;
							//зададим компаратор
							TCF0.CCA = stageStruct.ccaPack;
							//для выдачи импульса пачки включим канал С и запишем значение в компаратор
							TCF0.CTRLB |= TC0_CCCEN_bm;
							TCF0.CCC = stageStruct.ccaPack;
							
							PORTF.DIRSET |= 0x0001;
							//вкл ДМА
						    DMA_CH1_ON();
						}	
					}
					else if(workStruct.predFormStatus == labelMaster)
					{
						if(workStruct.countNoneSteps == stageStruct.nNoNLabelMaster - 1)
						{
							workStruct.countNoneSteps = 0;
							//зададим компаратор
							TCF0.CCA = stageStruct.perNoNLabelMaster - (stageStruct.perPack - stageStruct.ccaPack);
							TCF0.CTRLB = (TCF0.CTRLB & 0xF0)|TC0_WGMODE0_bm|TC0_WGMODE1_bm;
							//стоп передача
							recTransStatus &= ~(1<<TRANS_ENABLE);
							//USARTD0.CTRLA &= ~(USART_RXCINTLVL_HI_gc);
							//зададим статус
							workStruct.count = 7;
							workStruct.formStatus = NMakingMask;
							workStruct.predFormStatus = 0;
							if (needLabel == label) workStruct.predFormStatus = label;
							//вкл ДМА
							DMA_CH1_ON();
						}							
					}						
					else if(workStruct.predFormStatus == label)
					{
						if(workStruct.countNoneSteps == stageStruct.nNoNLabel - 1)
						{
							workStruct.countNoneSteps = 0;
							//зададим компаратор
							TCF0.CCA = stageStruct.perNoNLabel - (stageStruct.perPack - stageStruct.ccaPack);
							TCF0.CTRLB = (TCF0.CTRLB & 0xF0)|TC0_WGMODE0_bm|TC0_WGMODE1_bm;
							//стоп передача
							recTransStatus &= ~(1<<TRANS_ENABLE);
							//USARTD0.CTRLA &= ~(USART_RXCINTLVL_HI_gc);
							//зададим статус
							workStruct.count = 7;
							workStruct.formStatus = NMakingMask;
							workStruct.predFormStatus = labelToSlave1;
							//сбросим признак временного окраса
							needLabel = 0;
							//вкл ДМА
							DMA_CH1_ON();
						}
					}
					else if(workStruct.predFormStatus == labelToSlave1)
					{
						if(workStruct.countNoneSteps == stageStruct.nNoNLabelNext )
						{
							workStruct.countNoneSteps = 0;
							workStruct.formStatus = NMakingMask;
							//стоп передача
							recTransStatus &= ~(1<<TRANS_ENABLE);
							//USARTD0.CTRLA &= ~(USART_RXCINTLVL_HI_gc);
							//single slope					
							TCF0.CTRLB = (TCF0.CTRLB & 0xF0)|TC0_WGMODE0_bm|TC0_WGMODE1_bm;
							//зададим период
							TCF0.PER = stageStruct.perPack;
							//зададим компаратор
							TCF0.CCA = stageStruct.ccaPack;
							//зададим статус
							workStruct.predFormStatus = slave1;
							if(!(diagramStruct.slaveN & 0b00000001))
							{
								PORTF.DIR &= ~0x0001;
							}
							else
							{
								PORTF.DIRSET |= 0x0001;
								//вкл ДМА
								DMA_CH1_ON();
							}						
						}
					}
					break;				
				}
			}
		
			//PORTK.OUTSET = 1;
}

//таймер С

ISR(TCC0_CCA_vect)
{
	NOP();
}
ISR(TCC0_OVF_vect)
{
	//счетчик периодов до начала импульсов окраса 5 мин
	static unsigned char countFive = 0;
	//признак 5 мин
	static unsigned char five = 0;
	
	if ((TCC0.CTRLB & TC0_CCDEN_bm)!=0)
	{//выдавали сдвинутую секунду. запретим
		TCC0.CTRLB &= ~TC0_CCDEN_bm;
		PORTC.DIRCLR |= 0x08;
	}		
	//сдвинем шкалу
	if(workStruct.shiftSec == false && workStruct.countSecOnly < 7){
			if(workStruct.ticksSec != 0){
				if(workStruct.ticksSec > 2)  {
					workStruct.stepSec = workStruct.ticksSec/2;
					if(workStruct.stepSec + stageStruct.perSecOnly > 65536)
						workStruct.stepSec = (workStruct.stepSec+stageStruct.perSecOnly-65536)/2;
					if(workStruct.stepSec >= TCC0.CCA)
						workStruct.stepSec = TCC0.CCA/2;
					if(workStruct.stepSec ==  0) 
						workStruct.stepSec = 1;
				}
				else workStruct.stepSec = 1;
				TCC0.CCA+=workStruct.stepSec;
				TCC0.PER+=workStruct.stepSec;
				
				workStruct.shiftSec = true;
			}
		}else if(workStruct.countSecOnly < 7){
			TCC0.CCA -= workStruct.stepSec;
			TCC0.PER -= workStruct.stepSec;
			workStruct.ticksSec -= workStruct.stepSec;
			workStruct.shiftSec = false;
		}
		
	//формируем сек
	if(workStruct.countSecOnly == stageStruct.nSecOnly-1)
	{//следующий оверфлоу случится на заднем фронте секундной метки.
		PORTC.DIRSET |= 0x01;//выход сек
		workStruct.diagMode = secMode;
		//прибавим секунду
		AddSecond(&dateStruct);
		//синхронизируем с текущей датой
		if(workStruct.compensMissmatchInit == false)
			workStruct.compensMissmatchInit = true;
		//есть время для передачи
		recTransStatus |= (1<<TIME_READY_TO_TRANS);
		//посмотрим что за система
		if(system.sysType == Rns_e)
		{//рнс_е
				if(five!=0) countFive++;
				if(countFive == 12)
				{
					workStruct.diagMode = fiveMinMode;
					//уберем признак
					five = 0;
					//счетчик - 0
					countFive = 0;
				}			
				//проанализируем время
				if(dateStruct.sec == 0)
				{
					if(dateStruct.min%5 == 0)
					{//5 минут
						PORTC.DIRSET |= 0x02;//выход  мин	
						PORTC.DIRSET |= 0x04;//выход 5  мин
						workStruct.diagMode = minMode;
						five = 1;
					}	
					else
					{//минута
						PORTC.DIRSET |= 0x02;//выход  мин				
						workStruct.diagMode = minMode;
						//отправим опознавательный пакет на комп - имитатор на связи
						//recTransStatus |= (1<< VERSION_TO_TRANS);
					}							
				}
		}			
		else if(system.sysType == Rns_v)
		{//рнс_в
				
		}					
	}	
	else if(workStruct.countSecOnly == stageStruct.nSecOnly)
	{//задний фронт секунды
		workStruct.countSecOnly = 0;
		PORTC.DIRCLR |= 0x07;//вход
		
		//разрешим сравнение/захват для канала А, B, C
		TCC0.CTRLB |= TC0_CCDEN_bm;
		//выдадим сдвинутую секунду
		PORTC.DIRSET |= 0x08;
	}		
	workStruct.countSecOnly++;
}

//таймер D
ISR(TCD0_CCA_vect)
{
	NOP();
}
ISR(TCD0_OVF_vect)
{
	DACA.CH0DATA = dacVal;		
	if(workStruct.piFlag == 0)
	{
		dacVal = form5Pi[dacValCou];
	}
	else
	{
		dacVal = form5[dacValCou];
	}			
	dacVal<<=8;
	dacValCou++;
	
}
//таймер E
ISR(TCE0_CCA_vect)
{
	TCD0.CTRLA |= 0x03;
	NOP();
}	


ISR(TCE0_OVF_vect)
{
	//выключим таймер
	TCD0.CTRLA = 0;
	TCE0.CTRLA = 0;
	
	TCE0.CNT = 0x01;
	TCD0.CNT = 0x01;
	
	//разрешим прием/передачу
	recTransStatus |= (1<<TRANS_ENABLE);
	
	dacValCou=0;
	
	//Разберем фазовые коды. Рассчитаем на следующее включение ДМА
	volatile unsigned char ucShift;
	//запишем нужный фазовый код
	unsigned char ucS;
	switch (workStruct.phaseMS)
	{
		case master:
		{
			PORTH.OUTSET = 0x08;
			switch (workStruct.phasePer)
			{
				case a: 
				{
					ucS = 0;
					PORTH.OUTSET = 0x10;
					break;
				}					
				case b: 
				{
					ucS = 2;
					PORTH.OUTCLR = 0x10;
					break;
				}					
			}	
			break;			
		}			
		case slave:
		{
			PORTH.OUTCLR = 0x08;
			switch (workStruct.phasePer)
			{
				case a:
				{
					ucS = 1;
					PORTH.OUTSET = 0x10;
					break;
				}					
				case b:
				{
					ucS = 3;
					PORTH.OUTCLR = 0x10;
					break;
				}					
			}	
			break;
		}			
	}			
	//посмотрим, формируем ли импульс окраса
	if(workStruct.predFormStatus == label && system.sysType == Rns_e && system.advForm == AdvancedFOn)
	{//проверим, какой импульс окраса сек, мин или 5 мин для модернизированного формата
		if(workStruct.diagMode == minMode)
		{//минута
			ucShift = dateStruct.min<<workStruct.phaseMinCount;
			if(workStruct.phaseMinCount<9)
			{
				workStruct.phaseMinCount++;
			}	
			else
			{
				workStruct.phaseMinCount = 0;
			}						
		}
		else if(workStruct.diagMode == fiveMinMode)
		{//пять минут
			ucShift = (dateStruct.hour)<<workStruct.phase5MinCount;
			if(workStruct.phase5MinCount<10)
			{
				workStruct.phase5MinCount++;
			}	
			else
			{
				workStruct.phase5MinCount = 0;
			}					
		}		
		else 
		{//секунда
			volatile unsigned long ulShift = diagramStruct.phaseCodeSecLabel;
			//сдвинем
			ulShift = ulShift << workStruct.phaseSecCount;
			ucShift = (0xFF000000 & ulShift)>>24;
			if(workStruct.phaseSecCount<29) 
			{
				workStruct.phaseSecCount++; 
			}				
			else
			{
				workStruct.phaseSecCount = 0;
			}				
		}		
	}
	else if(system.sysType == Rns_v && workStruct.predFormStatus == label)
	{//окрас рнсв
		unsigned char codeNum = (unsigned char)floor((workStruct.countSec - (stageStruct.nSec - periodsNumTillSecRnsv))/ 32 );
		//выберем переменную
		ph = *(workStruct.phaseCodesRnsvPtr + codeNum);
		//сдвинем нужное кол-во раз
		shiftRnsv = workStruct.countSec - (stageStruct.nSec - periodsNumTillSecRnsv) - codeNum * 32;
		ph <<= shiftRnsv;
		//запихнем в переменную для дальнейшей обработки
		ucShift = (unsigned char)((ph & 0xFF000000)>>24);
	}	
	
	if(workStruct.predFormStatus!=label && workStruct.predFormStatus != labelMaster)// && workStruct.predFormStatus != labelMaster)
	{//окрас пачек		
		//workStruct.predFormStatus содержит инфу о будущем формировании	
		//PORTH.OUTSET = 0x40;
		//workStruct.phaseBuf = workStruct.phaseCodesPtr[ucS];			
		//workStruct.phaseBuf = workStruct.phaseBuf << workStruct.phaseCount;
		//workStruct.phaseCount++;	
		
		//workStruct.predFormStatus содержит инфу о будущем формировании
		workStruct.phaseBuf = *(workStruct.phaseCodesPtr + ucS);	
		//сдвинем нужное кол-во раз
		ucShift = workStruct.phaseBuf;
		ucShift = ucShift << workStruct.phaseCount;
		workStruct.phaseCount++;
	}else if(system.advForm == AdvancedFOff)
	{
		//ucShift = 0;		
	}
	if(((unsigned char)(ucShift & 0x80)) == 0) 
	{
		PORTH.OUTCLR = 0X01;
		workStruct.piFlag = 0;
	}		
	else
	{ 
		PORTH.OUTSET = 0X01;
		workStruct.piFlag = 1;
	}		
	PORTH.OUTCLR = 0x40;
	//Установим начальный адрес для ДМА (форма Пи или 0)
	//выключим ДМА
	//DMA_CH0_OFF();
	//DMA.CH0.TRFCNT = arrLeng;	
	//switch ((workStruct.phaseBuf & 0x80)>>7)
	//{
		//case 0:
		//{
			////DMA.CH0.SRCADDR0 = (int)(((long)(&(form))) & 0x000000FF);
			////DMA.CH0.SRCADDR1 = (int)(((long)(&(form))) & 0x0000FF00) >> 8;
			////DMA.CH0.SRCADDR2 = (int)(((long)(&(form))) & 0x00FF0000) >> 16;
			//PORTH.OUTCLR = 0X01;
			//workStruct.piFlag = 0;
		//}		
		//break;	
		//case 1:
		//{
			////DMA.CH0.SRCADDR0 = (int)(((long)(&(formPi))) & 0x000000FF);
			////DMA.CH0.SRCADDR1 = (int)(((long)(&(formPi))) & 0x0000FF00) >> 8;
			////DMA.CH0.SRCADDR2 = (int)(((long)(&(formPi))) & 0x00FF0000) >> 16;
			//PORTH.OUTSET = 0X01;
			//workStruct.piFlag = 1;
		//}		
		//break;	
	//}		
	//включим ДМА		
	//DMA_CH0_ON();
}


