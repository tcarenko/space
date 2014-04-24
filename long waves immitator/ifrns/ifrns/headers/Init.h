/*
 * init.h
 *
 * Created: 18.10.2011 19:54:12
 *  Author: Admin
 */ 


#ifndef INIT_H_
#define INIT_H_

//������������� ������
void InitializeHardwareSystem(unsigned char clockSource,		//�������� �����
					 unsigned char oscSource,					//�������� �����
					 unsigned char pllSource,					//�������� ����
					 unsigned char multiplicationFactor,		//����� �����
					 unsigned char freqRange,					//�������� ������ ��� �������� ���������
					 unsigned char xoscSel,					//�������� ��������� ������
					 unsigned int port0,						//��� �� ����
					 unsigned int port1,						//��� �� ����
					 unsigned int port2,						//��� �� ����
					 unsigned int portAddress,				//���� �����
					 
					 unsigned long portInterr,				//���� �������� ����������
					 unsigned char pinInterr					//��� �������� ����������	
					 );	
//������������� �����
void InitializeSoftwareSystem(SystemType sysT, 	
							  AdvancedFormat adv,						 	
							  SignalType sigT,
							  unsigned char phCodeMA,
							  unsigned char phCodeSA,
							  unsigned char phCodeMB,
							  unsigned char phCodeSB,
							  unsigned long phCodeSecLabel,
							  unsigned long rns_v_Code0,			//��� ����
							  unsigned long rns_v_Code1,			//��� ����
							  unsigned long rns_v_Code2,			//��� ����
							  unsigned long rns_v_Code3,				//��� ����
							  unsigned long sysFreq
							  );	
//������������� �����
void InitializeClock(unsigned char clockSource,				//�������� �����
					 unsigned char oscSource,					//�������� �����
					 unsigned char pllSource,					//�������� ����
					 unsigned char multiplicationFactor,		//����� �����
					 unsigned char freqRange,					//�������� ������ ��� �������� ���������
					 unsigned char xoscSel);					//�������� ��������� ������
//������������� ����� ��� �����������
void InitOscSource(unsigned char oscSource);
//��������� ��������� ���� � ������
void InitPll(unsigned char pllSource, unsigned char multFact);
//��������� ���������� ��� ������� �������
void InitXOSCReg(unsigned char frqRange, unsigned char xoscSel);
//������������� WDT
void InitializeWatchDogTimer(void);
//������������� ������
void InitializePorts(unsigned int portAddress, unsigned int port0, unsigned int port1, unsigned int port2, unsigned long portInterr,unsigned char pinInterr);
//������������� ������� �������
void InitializeEventSystem();
//������������� ��������
void InitializeTimers(void);
void InitTCC0(void);
void InitTCC1(void);
void InitTCD0(void);
void InitTCD1(void);
void InitTCE0(void);
void InitTCE1(void);
void InitTCF0(void);
void InitTCF1(void);
//�������������
void InitSynchTimers(void);
//������������� ���
void InitializeDAC(void);
//������������� DMA
void InitializeDMA(void);
//�������������� ������������ ����������
void InitializeHiRezExtension(void);
//������������� ���������� ������������ ��������
void InitializeAWEX(void);
//������������� ����������� ����������
void InitializeInterrupts(void);
//�������������� ��������� ���������
boolean InitializeDiagramParameters();
//�������� ���������
unsigned int CheckStruct(void* trgt, unsigned char size);
//������������� ����� ��������/������ ������
void InitializeUART(void);
//������������� ����� �������
void InitializeCodesParams(void);
//������������� ���+/-
void InitializationPhaseParams(void);


#endif /* INIT_H_ */