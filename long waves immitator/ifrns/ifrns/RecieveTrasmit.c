/*
 * RecieveTrasmit.c
 *
 * Created: 05.12.2011 17:33:08
 *  Author: Admin
 */ 
#include "All.h"

unsigned char* frame = NULL;

void SendData()
{
	if(USARTD0.STATUS &  USART_DREIF_bm)
	{
			if((USARTD0.STATUS & USART_TXCIF_bm)!=0) 
			{
				USARTD0.STATUS |= USART_TXCIF_bm;
			}				
			unsigned char transBuf;
			transBuf = PopByteFromStack(transStack, &transPointer);	
			usart_putchar(&USARTD0, transBuf);					
	}			
}

ISR(USARTD0_RXC_vect)
{
	//примем уж то, что пришло
	unsigned char recbuf = USARTD0.DATA;
	PushByteToStack(recStack, &recPointer, recbuf);
	recTransStatus |= (1<<PARSE_IN_DATA_EN);
}

ISR(USARTD0_DRE_vect)
{
	int i;
	i++;
}

unsigned char* GetFrame()
{//выделим начало и конец. начало кадра - START_FRAME, конец - END_FRAME
 //вернем указатель на начало кадра
	volatile unsigned char* buf = NULL;
	//размер копируемой области памяти
	unsigned char memSize = 0;
	//начало индекс начала памяти
	unsigned char ucStart = 0;
	//end_frame найден
	boolean endFrameFound = false;
	//поиск
	for(volatile int i = recPointer; i>=0; i--)
	{
		if(recStack[i] == END_FRAME)
		{//нашли конец
			memSize = i;
			endFrameFound = true;
			continue;
		}
		if(endFrameFound == true){
			if(recStack[i] == START_FRAME)
			{//нашли начало
				memSize -= (i-1);
				ucStart = i;
				//выделим
				buf = (unsigned char*)malloc(memSize);
				//скопируем
				memcpy((void*)buf, &recStack[ucStart], (size_t)memSize);
				//exit
				break;
			}
		}		
	}
	return buf;
}



void ExchangeServer()
{
	if(!(recTransStatus & (1<<TRANS_ENABLE))) return;
	if(recTransStatus & (1<<PARSE_IN_DATA_EN))
		{
			//парсим то, что пришло
			//unsigned char* frame = NULL;
			if(recPointer != 0) 
			{	//выделим пакет
				//если стэк полон - очистим
				//if(!(recTransStatus & (1<<TRANS_ENABLE))) return;
				if(recPointer == stackSize-1) FlushStack(recStack, &recPointer);
				//if(!(recTransStatus & (1<<TRANS_ENABLE))) return;
				frame = GetFrame();
				//проверим
				if(*frame == (unsigned char)START_FRAME) {
					if(*(frame + 1) == RECIEVE_FRAME)
					{	//действия
						switch(*(frame + 2))
						{
							case (unsigned char)DIAGRAM_STRUCT_FRAME:
							{
								//PMIC.CTRL &= ~PMIC_HILVLEN_bm;
								//пришла диаграмма. положим ее в нужную область памяти
								//memcpy(&diagramStruct, (frame + 3), sizeof(DiagramStruct));
								memcpy(&diagramStruct.modernFormat, (frame+3), 2);
								memcpy(&diagramStruct.sysType, (frame+5), 2);
								memcpy(&diagramStruct.sigType, (frame+7), 2);
								memcpy(&diagramStruct.Tpack, (frame+9), sizeof(unsigned long));
								memcpy(&diagramStruct.Tn, (frame+13), sizeof(unsigned long));
								memcpy(&diagramStruct.slaveN, (frame+17), sizeof(unsigned char));
								memcpy(&diagramStruct.masterSlave1, (frame+19), sizeof(unsigned long)*4);
								memcpy(&diagramStruct.phaseCodeSecLabel, (frame+35), sizeof(unsigned long)+ 2*sizeof(unsigned int));
								
								//diagramStruct.sigType = Loran;
								
								diagramStruct.n = diagramStructDefaultn;
								diagramStruct.T1n = diagramStructDefaultT1n;
								diagramStruct.Tmas = diagramStructDefaultTmas;
								diagramStruct.tau = diagramStructDefaulttau;
								
								diagramStruct.phaseCodesRnsv[0] = rns_vCode0;
								diagramStruct.phaseCodesRnsv[1] = rns_vCode1;
								diagramStruct.phaseCodesRnsv[2] = rns_vCode2;
								diagramStruct.phaseCodesRnsv[3] = rns_vCode3;
								
								diagramStruct.phaseCodes[0] = phaseCodeMasterADefault;
								diagramStruct.phaseCodes[1] = phaseCodeSlaveADefault;
								diagramStruct.phaseCodes[2] = phaseCodeMasterBDefault;
								diagramStruct.phaseCodes[3] = phaseCodeSlaveBDefault;
								
								if(diagramStruct.masterSlave1 == 0) diagramStruct.masterSlave1 = 12000;
								if(diagramStruct.masterSlave2 == 0) diagramStruct.masterSlave2 = 24000;
								if(diagramStruct.masterSlave3 == 0) diagramStruct.masterSlave3 = 36000;
								if(diagramStruct.masterSlave4 == 0) diagramStruct.masterSlave4 = 48000;
								
								//сохраним в eeprom
								unsigned char* ptr = (unsigned char*)&diagramStruct;
								unsigned int crc;
								eeprom_write_block(ptr, (void*)DIAGRAM_STRUCT_EEPROM_ADDRESS, sizeof(DiagramStruct));	
								eeprom_busy_wait();
								crc = GetCRC(&diagramStruct, sizeof(DiagramStruct));
								ptr = (unsigned char*)&crc;
								eeprom_write_block(ptr, (void*)DIAGRAM_STRUCT_EEPROM_CRC, sizeof(unsigned int));
								eeprom_busy_wait();
								recTransStatus &= ~(1 << DIAGRAM_STRUCT_REQUEST);
								//перезапуск
							
									reset_do_soft_reset();			
								
								
								break;
							}	
							case (unsigned char)SYSTEM_CHANGE_FRAME:
							{
								//проверим формируется ли диаграмма
								if((SREG & CPU_I_bm) == 0)
								{//сменим тип системы
									if(*(frame + 3) == Rns_e || *(frame + 3) == Rns_v)
									{
										system.sysType = *(frame + 3);
									}
									}						
									break;						 
							}
							case (unsigned char)STOP_DIAGRAM_FRAME:
							{
								//PMIC.CTRL &= ~PMIC_HILVLEN_bm;
								break;
							}	
							case (unsigned char)START_DIAGRAM_FRAME:
							{
								//PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
								break;
							}
							case (unsigned char)SHIFT_DIAG_RIGHT_FRAME:
							{
								//unsigned int msToShift = *(frame+3);
								//msToShift |= (*(frame+4) << 8);
								//workStruct.msToShift = msToShift;
								ShiftDiagram(50000);
								break;
							}	
							case (unsigned char)SHIFT_DIAG_LEFT_FRAME:
							{
								//unsigned int msToShift = *(frame+3);
								//msToShift |= (*(frame+4) << 8);
								//workStruct.msToShift = msToShift;
								ShiftDiagram(-1);
								break;
							}	
							case (unsigned char)SERVICE_STRUCT_FRAME:
							{
								memcpy(&serviceStruct, (frame + 3), sizeof(ServiceStruct));
								ScanCalibrPins(serviceStruct.dacVal);
								unsigned char* ptr = (unsigned char*)&serviceStruct;
								//сохраним в епром
								eeprom_write_block(ptr, (void*)SERVICE_STRUCT_EEPROM_ADDRESS, sizeof(ServiceStruct));
								eeprom_busy_wait();
								unsigned int crc = GetCRC(&serviceStruct, sizeof(ServiceStruct));
								ptr = (unsigned char*)&crc;
								eeprom_write_block(ptr, (void*)SERVICE_STRUCT_EEPROM_CRC, sizeof(unsigned int)); 
								eeprom_busy_wait();
								recTransStatus &= ~(1 << SERVICE_STRUCT_REQUEST);
								//перезапуск
								if(recTransStatus & (1 << DIAGRAM_STRUCT_REQUEST) == 0)
								{
									//reset_do_soft_reset();		
								}
								
							}			
							case (unsigned char)TIME_FRAME:
							{
								memcpy(&dateStruct.year, frame+3, sizeof(DateStruct));
								//перезапуск
								reset_do_soft_reset();
							}																					
						 }																						
						}
						else if(*(frame + 1) == (unsigned char)REQUEST_FRAME)
						{//запросы с компа
							if(*(frame + 2) == (unsigned char)VERSION_FRAME){
								recTransStatus |= (1 << VERSION_TO_TRANS);
							}				
							else if(*(frame + 2) == (unsigned char)DIAGRAM_STRUCT_FRAME){
								recTransStatus |= (1 <<DIAGRAM_STRUCT_READY_TO_TRANS);
							}
							else if(*(frame + 2) == (unsigned char)SERVICE_STRUCT_FRAME){
								recTransStatus |= (1 <<SERVICE_DIAG_TO_TRANS);
							}
						}
						FlushStack(recStack, &recPointer);
				}						
			}			
			//запрет проверки приемного буфера
			recTransStatus &= ~(1<<PARSE_IN_DATA_EN);
		}
	else 
		{//отправка
			if(recTransStatus & (1<<TRANS_ENABLE))
			{
						//старт фрейм
						PushByteToStack(transStack, &transPointer, START_FRAME);
						//можно передавать. проверим, что передавать
						if(recTransStatus & (1<<TIME_READY_TO_TRANS))
						{//передаем время. запихнем в стек для передачи
						 //проверим, место-то в стэке есть вообще?
						 if(CheckForStack(transPointer, sizeof(dateStruct)) == true)
						 {//место есть. запихиваем
							 unsigned char *pSend = (unsigned char*)&dateStruct;
							 PushByteToStack(transStack, &transPointer, SEND_FRAME);	
							 PushByteToStack(transStack, &transPointer, TIME_FRAME);	
							 //данные
							 PushFrameToStack(transStack, &transPointer, pSend, sizeof(dateStruct))	;		 				 
							 //опустим флаг
							 recTransStatus &= ~(1<<TIME_READY_TO_TRANS);
						 }
						}
						else if(recTransStatus & (1<<DIAGRAM_STRUCT_READY_TO_TRANS))
						{//передадим диаграмму формирования
							if(CheckForStack(transPointer, sizeof(DiagramStruct)) == true)
							{//место есть.запихиваем
								unsigned char* pSend = (unsigned char*)&diagramStruct;
								PushByteToStack(transStack, &transPointer, SEND_FRAME);								
								PushByteToStack(transStack, &transPointer, DIAGRAM_STRUCT_FRAME);
								//данные
								PushFrameToStack(transStack, &transPointer, pSend, sizeof(DiagramStruct));
								//опустим флах
								recTransStatus &= ~(1<<DIAGRAM_STRUCT_READY_TO_TRANS);
							}
						}
						else if(recTransStatus & (1<<DIAGRAM_STRUCT_REQUEST))
						{//запрос данных диаграммы
							if(CheckForStack(transPointer,sizeof(unsigned char)) == true)
							{
								PushByteToStack(transStack, &transPointer, REQUEST_FRAME);
								PushByteToStack(transStack, &transPointer, DIAGRAM_STRUCT_FRAME);
								//recTransStatus &= ~(1<<DIAGRAM_STRUCT_REQUEST);
							}
						}
						else if(recTransStatus & (1<<IMITATOR_ONLINE))
						{//имитатор на связи
							if(CheckForStack(transPointer,sizeof(unsigned char)) == true)
							{
								PushByteToStack(transStack, &transPointer, SEND_FRAME);
								PushByteToStack(transStack, &transPointer, IMITATOR_ONLINE_FRAME);
								recTransStatus &= ~(1<<IMITATOR_ONLINE);
							}
						}
						else if(recTransStatus & (1<<VERSION_TO_TRANS))
						{//отправим версию
							PushByteToStack(transStack, &transPointer, SEND_FRAME);	
							PushByteToStack(transStack, &transPointer, VERSION_FRAME);
							PushByteToStack(transStack, &transPointer, ver_number);
							PushByteToStack(transStack, &transPointer, ver_modification);
							recTransStatus &= ~(1<<VERSION_TO_TRANS);
						}
						else if(recTransStatus & (1 <<SERVICE_DIAG_TO_TRANS))
						{//отправим служебную структуру
							unsigned char *pSend = (unsigned char*)&serviceStruct;
							PushByteToStack(transStack, &transPointer, SEND_FRAME);	
							PushByteToStack(transStack, &transPointer, SERVICE_STRUCT_FRAME);
							PushFrameToStack(transStack, &transPointer, pSend, sizeof(serviceStruct));
							recTransStatus &= ~(1 << SERVICE_DIAG_TO_TRANS);
						}	
						else if(recTransStatus & (1 << SERVICE_STRUCT_REQUEST))
						{
							PushByteToStack(transStack, &transPointer, REQUEST_FRAME);
							PushByteToStack(transStack, &transPointer, SERVICE_STRUCT_FRAME);
							//recTransStatus &= ~(1<<SERVICE_STRUCT_REQUEST);
						}
						
						//если только старт фрейм то передавать нечего
						if(transPointer != 1){	
							//конец фрейм
							PushByteToStack(transStack, &transPointer, END_FRAME);
							//отправим
							do{
								 if(recTransStatus & (1<<TRANS_ENABLE)){
									SendData();
								 }
							}while(transPointer != 0);
						}	
						//очистить стек передачи
						FlushStack(transStack, &transPointer);					
					}
			}		
		
}

void FlushStack(unsigned char* stack, unsigned char* stackPtr){
	unsigned char bytesNum = *stackPtr;
	for(unsigned char i = 0; i < bytesNum; i++){
		stack[i] = 0;
	}
	*stackPtr = 0;
}

boolean CheckForStack(unsigned char sp, unsigned char bytesNeeded)
{//проверим есть ли в стэке место под bytesNeeded байт
	if(((stackSize - sp)*sizeof(unsigned char) - 5) > bytesNeeded)
	{
		return true;
	}
	return false;
}

void PushFrameToStack(unsigned char* stack, unsigned char* stPtr, unsigned char* toPushPtr, unsigned char bytesToSend)
{
	for(unsigned char i = 0; i<bytesToSend; i++)
	{
		PushByteToStack(stack, stPtr, *(toPushPtr + i));
	}
}