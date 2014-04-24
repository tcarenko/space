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
	//������ �� ��, ��� ������
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
{//������� ������ � �����. ������ ����� - START_FRAME, ����� - END_FRAME
 //������ ��������� �� ������ �����
	volatile unsigned char* buf = NULL;
	//������ ���������� ������� ������
	unsigned char memSize = 0;
	//������ ������ ������ ������
	unsigned char ucStart = 0;
	//end_frame ������
	boolean endFrameFound = false;
	//�����
	for(volatile int i = recPointer; i>=0; i--)
	{
		if(recStack[i] == END_FRAME)
		{//����� �����
			memSize = i;
			endFrameFound = true;
			continue;
		}
		if(endFrameFound == true){
			if(recStack[i] == START_FRAME)
			{//����� ������
				memSize -= (i-1);
				ucStart = i;
				//�������
				buf = (unsigned char*)malloc(memSize);
				//���������
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
			//������ ��, ��� ������
			//unsigned char* frame = NULL;
			if(recPointer != 0) 
			{	//������� �����
				//���� ���� ����� - �������
				//if(!(recTransStatus & (1<<TRANS_ENABLE))) return;
				if(recPointer == stackSize-1) FlushStack(recStack, &recPointer);
				//if(!(recTransStatus & (1<<TRANS_ENABLE))) return;
				frame = GetFrame();
				//��������
				if(*frame == (unsigned char)START_FRAME) {
					if(*(frame + 1) == RECIEVE_FRAME)
					{	//��������
						switch(*(frame + 2))
						{
							case (unsigned char)DIAGRAM_STRUCT_FRAME:
							{
								//PMIC.CTRL &= ~PMIC_HILVLEN_bm;
								//������ ���������. ������� �� � ������ ������� ������
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
								
								//�������� � eeprom
								unsigned char* ptr = (unsigned char*)&diagramStruct;
								unsigned int crc;
								eeprom_write_block(ptr, (void*)DIAGRAM_STRUCT_EEPROM_ADDRESS, sizeof(DiagramStruct));	
								eeprom_busy_wait();
								crc = GetCRC(&diagramStruct, sizeof(DiagramStruct));
								ptr = (unsigned char*)&crc;
								eeprom_write_block(ptr, (void*)DIAGRAM_STRUCT_EEPROM_CRC, sizeof(unsigned int));
								eeprom_busy_wait();
								recTransStatus &= ~(1 << DIAGRAM_STRUCT_REQUEST);
								//����������
							
									reset_do_soft_reset();			
								
								
								break;
							}	
							case (unsigned char)SYSTEM_CHANGE_FRAME:
							{
								//�������� ����������� �� ���������
								if((SREG & CPU_I_bm) == 0)
								{//������ ��� �������
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
								//�������� � �����
								eeprom_write_block(ptr, (void*)SERVICE_STRUCT_EEPROM_ADDRESS, sizeof(ServiceStruct));
								eeprom_busy_wait();
								unsigned int crc = GetCRC(&serviceStruct, sizeof(ServiceStruct));
								ptr = (unsigned char*)&crc;
								eeprom_write_block(ptr, (void*)SERVICE_STRUCT_EEPROM_CRC, sizeof(unsigned int)); 
								eeprom_busy_wait();
								recTransStatus &= ~(1 << SERVICE_STRUCT_REQUEST);
								//����������
								if(recTransStatus & (1 << DIAGRAM_STRUCT_REQUEST) == 0)
								{
									//reset_do_soft_reset();		
								}
								
							}			
							case (unsigned char)TIME_FRAME:
							{
								memcpy(&dateStruct.year, frame+3, sizeof(DateStruct));
								//����������
								reset_do_soft_reset();
							}																					
						 }																						
						}
						else if(*(frame + 1) == (unsigned char)REQUEST_FRAME)
						{//������� � �����
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
			//������ �������� ��������� ������
			recTransStatus &= ~(1<<PARSE_IN_DATA_EN);
		}
	else 
		{//��������
			if(recTransStatus & (1<<TRANS_ENABLE))
			{
						//����� �����
						PushByteToStack(transStack, &transPointer, START_FRAME);
						//����� ����������. ��������, ��� ����������
						if(recTransStatus & (1<<TIME_READY_TO_TRANS))
						{//�������� �����. �������� � ���� ��� ��������
						 //��������, �����-�� � ����� ���� ������?
						 if(CheckForStack(transPointer, sizeof(dateStruct)) == true)
						 {//����� ����. ����������
							 unsigned char *pSend = (unsigned char*)&dateStruct;
							 PushByteToStack(transStack, &transPointer, SEND_FRAME);	
							 PushByteToStack(transStack, &transPointer, TIME_FRAME);	
							 //������
							 PushFrameToStack(transStack, &transPointer, pSend, sizeof(dateStruct))	;		 				 
							 //������� ����
							 recTransStatus &= ~(1<<TIME_READY_TO_TRANS);
						 }
						}
						else if(recTransStatus & (1<<DIAGRAM_STRUCT_READY_TO_TRANS))
						{//��������� ��������� ������������
							if(CheckForStack(transPointer, sizeof(DiagramStruct)) == true)
							{//����� ����.����������
								unsigned char* pSend = (unsigned char*)&diagramStruct;
								PushByteToStack(transStack, &transPointer, SEND_FRAME);								
								PushByteToStack(transStack, &transPointer, DIAGRAM_STRUCT_FRAME);
								//������
								PushFrameToStack(transStack, &transPointer, pSend, sizeof(DiagramStruct));
								//������� ����
								recTransStatus &= ~(1<<DIAGRAM_STRUCT_READY_TO_TRANS);
							}
						}
						else if(recTransStatus & (1<<DIAGRAM_STRUCT_REQUEST))
						{//������ ������ ���������
							if(CheckForStack(transPointer,sizeof(unsigned char)) == true)
							{
								PushByteToStack(transStack, &transPointer, REQUEST_FRAME);
								PushByteToStack(transStack, &transPointer, DIAGRAM_STRUCT_FRAME);
								//recTransStatus &= ~(1<<DIAGRAM_STRUCT_REQUEST);
							}
						}
						else if(recTransStatus & (1<<IMITATOR_ONLINE))
						{//�������� �� �����
							if(CheckForStack(transPointer,sizeof(unsigned char)) == true)
							{
								PushByteToStack(transStack, &transPointer, SEND_FRAME);
								PushByteToStack(transStack, &transPointer, IMITATOR_ONLINE_FRAME);
								recTransStatus &= ~(1<<IMITATOR_ONLINE);
							}
						}
						else if(recTransStatus & (1<<VERSION_TO_TRANS))
						{//�������� ������
							PushByteToStack(transStack, &transPointer, SEND_FRAME);	
							PushByteToStack(transStack, &transPointer, VERSION_FRAME);
							PushByteToStack(transStack, &transPointer, ver_number);
							PushByteToStack(transStack, &transPointer, ver_modification);
							recTransStatus &= ~(1<<VERSION_TO_TRANS);
						}
						else if(recTransStatus & (1 <<SERVICE_DIAG_TO_TRANS))
						{//�������� ��������� ���������
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
						
						//���� ������ ����� ����� �� ���������� ������
						if(transPointer != 1){	
							//����� �����
							PushByteToStack(transStack, &transPointer, END_FRAME);
							//��������
							do{
								 if(recTransStatus & (1<<TRANS_ENABLE)){
									SendData();
								 }
							}while(transPointer != 0);
						}	
						//�������� ���� ��������
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
{//�������� ���� �� � ����� ����� ��� bytesNeeded ����
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