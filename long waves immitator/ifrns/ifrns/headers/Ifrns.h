/*
 * ifrns.h
 *
 * Created: 18.10.2011 18:23:47
 *  Author: Igor Tsarenko
 *	Organization: Navigation - Service
 */ 


#ifndef IFRNS_H_
#define IFRNS_H_
//��� ����������
//#define __AVR_ATxmega128A1__
#define ver_number			1
#define ver_modification	2
#define version			number"."modification
//������� ��������� �������. ������������ � sysclk.c
#define BOARD_XOSC_HZ	20000000UL
//�������**********************************************************
//����� ����������� �������
#define WDR() asm("WDR")
//nop
#define NOP() asm("NOP")
//test mode
#define TESTMODE
//DMA
#define DMA_CH1_OFF()	DMA.CH1.CTRLA &= ~0b10000000
#define DMA_CH1_ON()	DMA.CH1.CTRLA |= 0b10000000 
#define DMA_CH0_OFF()	DMA.CH0.CTRLA &= ~0b10000000
#define DMA_CH0_ON()	DMA.CH0.CTRLA |= 0b10000000
//*****************************************************************
//�����************************************************************
#define PORTAADDR     0x0600  /* Port A */
#define PORTBADDR     0x0620  /* Port B */
#define PORTCADDR     0x0640  /* Port C */
#define PORTDADDR     0x0660  /* Port D */
#define PORTEADDR     0x0680  /* Port E */
#define PORTFADDR     0x06A0  /* Port F */
#define PORTHADDR     0x06E0  /* Port H */
#define PORTJADDR     0x0700  /* Port J */
#define PORTKADDR     0x0720  /* Port K */
#define PORTQADDR     0x07C0  /* Port Q */
#define PORTRADDR     0x07E0  /* Port R */

//������������� ����
#define CALIBPORT			PORTK
#define CALIBPIN_SOURCE		0
#define CALIBPIN_DRAIN		1

#define DACPORT			  PORTA
#define DAC_CH0_On()	  DACPORT.DIR = 0xff
#define DAC_CH0_Off()	  DACPORT.DIR = 0x00
//*****************************************************************
//���� ������******************************************************



#define s32f float

#define s08 short


//*****************************************************************

#define arrLeng		255
#define formSize	58
unsigned char timFStart;	//�����
unsigned char timFStop;	//����

//u08 piFlag = 0;

//�����************************************************************
unsigned long freq; //� ��
unsigned long freqAltera;//� ��
//*****************************************************************
//��������/��������� ������-����***********************************
#define stackSize	128

unsigned char recStack[stackSize];
unsigned char recPointer;

unsigned char transStack[stackSize];
unsigned char transPointer;

//��������� ������:
//
//	(START_FRAME) (SEND_FRAME|REQUEST_FRAME|RECIEVE_FRAME) (WHAT_FRAME) (DATA) (END_FRAME)

#define START_FRAME			0x24//������� ������ ��������
#define END_FRAME			0x2a//������� ����� ��������

#define SEND_FRAME			0x2b//��������� ��������
#define REQUEST_FRAME		0x2c//��������� �������
#define RECIEVE_FRAME		0x2d//��������� ������. � ������� �� ����������

#define TIME_FRAME					0x21//��������� �������	
#define DIAGRAM_STRUCT_FRAME		0x22//��������� ��������� ���������
#define IMITATOR_ONLINE_FRAME		0x23//��������� �������� �� �����
#define SYSTEM_CHANGE_FRAME			0x25//��������� ����� �������
#define STOP_DIAGRAM_FRAME			0x26//��������� ��������� ������������ ���������
#define START_DIAGRAM_FRAME			0x27//��������� ������� ������������ ���������
#define VERSION_FRAME				0x28//��������� ������ ��������
#define SHIFT_DIAG_RIGHT_FRAME		0x29//��������� ��� ������ ��������� ������
#define SHIFT_DIAG_LEFT_FRAME		0x2b//��������� ��� ������ ��������� �����
#define SERVICE_STRUCT_FRAME		0x2c//��������� ��� ��������� ���������

unsigned int recTransStatus;							//�����
 
#define REC_ENABLE						0	//����� ���������
#define TRANS_ENABLE					1	//����� ����������

#define PARSE_IN_DATA_EN				2	//������, ��� ������
#define TIME_READY_TO_TRANS				3	//����� ������ ��� ��������
#define DIAGRAM_STRUCT_READY_TO_TRANS	4	//��������� ��������� ������ ��� ��������
#define DIAGRAM_STRUCT_REQUEST			5	//������ ���������� ��������� � �����
#define IMITATOR_ONLINE					6	//������� �����, ��� �������� �� �����
#define VERSION_TO_TRANS				7	//�������� ����� ������
#define SERVICE_DIAG_TO_TRANS			8	//�������� ��������� ���������
#define SERVICE_STRUCT_REQUEST			9	//��������� ��������� ���������

//*****************************************************************
//���� ������ ����� �� ����� ����**********************************
unsigned int portL;//low
unsigned int portM;//med
unsigned int portH;//high
//*****************************************************************
//���� ������******************************************************
unsigned int portAddr;
//*****************************************************************
//������***********************************************************
#define SEV_SET_ADDR					0x01
#define HTO_SET_ADDR					0x02
#define PHASE_PLUS_MINUS_SET_ADDR		0x04
#define SEV_MEAS_GET_ADDR				0x08
#define HTO_MEAS_GET_ADDR				0x10
//*****************************************************************
//������ ����� ������� ���
unsigned long arrayFrontsCodesSEV[64];
unsigned long* arrFrCodesPntrSEV;
//������ ����� ������� ���
unsigned long arrayFrontsCodesHTO[64];
unsigned long* arrFrCodesPntrHTO;
//������ ����� ���+/-
unsigned char arrayPhaseManipul[64];
unsigned char arrPhManPntr;
//*****************************************************************
typedef enum
{
	false,
	true
}boolean;
//������ �������
unsigned int secTimeICP;
unsigned long ulSecBuf;
//��������� ���������
#define diagramStructDefaultTpack					80000
#define diagramStructDefaulttau						10
#define diagramStructDefaultT1n						1000
#define diagramStructDefaultTn						3600
#define diagramStructDefaultTmas					1200
#define diagramStructDefaultn						8
#define diagramStructDefaultslaveN					0b00000011
#define diagramStructDefaultmasterSlave1			21000
#define diagramStructDefaultmasterSlave2			36000
#define diagramStructDefaultmasterSlave3			48000
#define diagramStructDefaultmasterSlave4			65000

//��������� ������� ���� 
#define phaseCodeMasterADefault	0b11001010
#define phaseCodeMasterBDefault	0b10011111
#define phaseCodeSlaveADefault	0b11111001
#define phaseCodeSlaveBDefault	0b10101100

#define rns_vCode0							0b01000001100001010001111001000101
#define rns_vCode1							0b10011101010011111010000111000100
#define rns_vCode2							0b10011011010110111101100011010010
#define rns_vCode3							0b11101110011001010101111111000000
					
					
					
#define refYear							2005
#define refMonth						1
#define refDay							1
#define refHour							0
#define refMin							0
#define refSec							0
#define secsFrom01011958Till01012005	1483228800L
#define periodsNumTillSecRnsv			128



#define phaseCodesSecLabel0Default		0xff5a6270
#define phaseCodesSecLabel1Default		0xff5c5a40
#define phaseCodesMinLabelDefault		0x0
#define phaseCodeFiveMinLabelDefault	0x0



//��� �������
typedef enum systemType
{
	Rns_e,
	Rns_v
}SystemType;
//��� �������
typedef enum signalType
{
	Loran,
	Chaika
}SignalType;
//����������������� ������
typedef enum advancedFormat
{
	AdvancedFOff,
	AdvancedFOn
}AdvancedFormat;

//���������, �� ����� ����������� ���������
typedef struct  
{//��� � �������������
	AdvancedFormat	modernFormat;
	SystemType sysType;
	SignalType sigType;
	unsigned long Tpack;			//������ �����
	unsigned char tau;			//������������ ��������� � �����
	unsigned int T1n;		    //������ ��������� � �����
	unsigned int Tmas;			//����� �� n-�� �������� �� �������� ����������� �������
	unsigned int Tn;			    //����� �� n-��� �������� �� �������� ������
	unsigned char n;				//���-�� ��������� � �����	
	unsigned char slaveN;			//���-�� �������. 0�0000ssss - s - ����� �� ����� ������� ������� ������������ � �������
	unsigned long masterSlave1;	//���������� �� 1 �������
	unsigned long masterSlave2;	//���������� �� 2 �������
	unsigned long masterSlave3;	//���������� �� 3 �������
	unsigned long masterSlave4;	//���������� �� 4 �������
	//������ ������� �����*********************************************
	unsigned char phaseCodes[4];
	//30-�� ���������� ������� ��� ��� ��� �������� ��� ������������������ �������
	unsigned long phaseCodeSecLabel;
	//������� ��� ��� 10 �������� ������
	unsigned int  phaseCodeMinLabel;
	//������� ��� ��� 11 �������� 5 ���
	unsigned int  phaseCode5MinLabel;
	//������� ��� ��� ��������� ������ ����
	unsigned long phaseCodesRnsv[4];
}DiagramStruct;

typedef struct 
{
	unsigned int dacVal;	
}ServiceStruct;

//��������� �������� ������� � �������� ���������
typedef struct 
{
	unsigned int perPack;
	unsigned int ccaPack;

	unsigned int ccaNPlusOne;						//������ ����� ��������
	
	unsigned int perNoNMasterSlave1;					//����� � ���-�� ����� �� ������ ������� ����� �� ����� ������� �����
	unsigned int nNoNMasterSlave1;
	unsigned int perNoNMasterSlave2;
	unsigned int nNoNMasterSlave2;
	unsigned int perNoNMasterSlave3;
	unsigned int nNoNMasterSlave3;
	unsigned int perNoNMasterSlave4;
	unsigned int nNoNMasterSlave4;
	
	unsigned int perNoN;						
	unsigned int ccaNo;
	unsigned int nNoN;								//���-�� �� perNo �� n-��� �� ������ ���� �����
	
	unsigned int perNoNLabelMaster;					//����� ����  �� ����� �������� ����������� �������
	unsigned int nNoNLabelMaster;					//���-�� ����� �� ����� �������� ����������� �������
	
	unsigned int perNoNLabel;						//����� ����� ��� ��������� �������� ������
	unsigned int nNoNLabel;							//���-�� ����� �� ����� �������� ������
	
	unsigned int perNonLabelNext;					//���-�� ����� �� ������ �� ���� �����
	unsigned int nNoNLabelNext;						//����� ���� �� ������ �� ���� �����
	
	unsigned int perSecOnly;							//������� �� �������				
	unsigned int nSecOnly;							//���-�� �������� perSecOnly � �������	
			
	unsigned int nSec;								//���-�� �������� ����� �� �������
	
}StageStruct;

unsigned int diagramArray[128];
unsigned char diagramArrayIndex;	
//������� ������
typedef enum phasePer				
	{
		a,
		b
	}PhasePer;	
PhasePer phPer;
//������� ������
typedef enum phaseMS				
	{
		master,
		slave
	}PhaseMS;	
PhaseMS phMS;

//��� ��������� ���, ��� ��� 5 ���
typedef enum diagMode
{
	secMode = 0,
	minMode,
	fiveMinMode
}DiagMode;
	

typedef enum month
{
	jan = 0,
	feb,
	mar,
	apr,
	may,
	jun,
	jul,
	aug,
	sep,
	oct,
	nov,
	dec
}Month;

//�������
typedef struct
{
	SystemType		sysType;
	AdvancedFormat	advForm;
	SignalType		sigType;
}System;	

System system;
//��������� � ������� ���������� ������������ ���������
#define NMakingMask			   0x01
#define none				   0x04
#define labelMaster			   0x16	
#define label				   0x02
#define labelToSlave1		   0x06
#define slave1				   0x08
#define slave2				   0x10
#define slave3				   0x12
#define slave4				   0x14

enum
{
	eNMakingMask=0,	   
	eNone			,	   
	eLabelMaster	,	   	
	eLabel			,	   
	eLabelToSlave1	,	   
	eSlave1			,	   
	eSlave2			,	   
	eSlave3			,	   
	eSlave4			,	   
}	eStatus;

typedef struct 
{
	unsigned char countNoneSteps;			//������� ��������
	s08 count;								//������� ���������
	unsigned char countLabel;				//������� ��� �������� ������
	unsigned int countSec;					//������� ��� ������������ ��������, ������������ � ���
	unsigned int countSecOnly;				//������� ��� ������������ �������
	unsigned int predFormStatus;			//���������� ��������� ������������
	unsigned char formStatus;				//��������� ������������
	unsigned char synchronized:1;			//������������� ��������
	unsigned char	countSlave1NoneSteps;	//������� �������� �� �������1
	unsigned char	countSlave2NoneSteps;	//������� �������� �� �������2
	unsigned char	countSlave3NoneSteps;	//������� �������� �� �������3
	unsigned char	countSlave4NoneSteps;	//������� �������� �� �������4
	unsigned char countSlave1;				//��������� �������1 ������� ���������
	unsigned char countSlave2;				//��������� �������1 ������� ���������
	unsigned char countSlave3;				//��������� �������3 ������� ���������
	unsigned char countSlave4;				//��������� �������4 ������� ���������
	PhasePer phasePer;						//������� ������
	PhaseMS phaseMS;						//������� ���	
	unsigned char* phaseCodesPtr;			//��������� �� ������� ���
	unsigned char phaseBuf;					//����� � ������� ������� �����	
	unsigned char phaseCount;				//������� ������ ��� ����
	unsigned long* phaseCodesRnsvPtr;		//��������� �� ������� ��� ����
	unsigned long phaseRnsvBuf;				//����� � ������� ������� ����� ����
	unsigned char phaseRnsvCount;			//������� ������ ��� ���� ����
	unsigned char phaseSecCount;			//������� ������ 30-�� ����������� ����
	unsigned char phaseMinCount;			//������� ������ ��� ���� ��� ��� �������
	unsigned char phase5MinCount;			//������� ������ ��� ���� ��� 5 ��� �������
	DiagMode diagMode;						//
	unsigned char minMode;					//������� ���-�� �������� �����
	#define minModeDefault 10
	unsigned char fiveMinMode;				//������� ���-�� �������� 5 �����
	#define fiveMinModeDefault 11
	unsigned long totalTicks;
	unsigned long ticks;					//�������� �� msToShiftMs
	volatile boolean shift;
	unsigned int step;
	
	unsigned long ticksSec;					//�������� �� msToShiftMs
	volatile boolean shiftSec;
	unsigned int stepSec;
	
	boolean compensMissmatchInit;
	boolean shiftDone;
	
	unsigned char piFlag:1;
}WorkStruct;
//�����
typedef struct
{
	unsigned long ulHours;
	unsigned char ucMins;
	unsigned char ucSecs;
}TimeStruct;

typedef struct  
{
	boolean leapYear;
	unsigned int year;
	Month month;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
}DateStruct;

StageStruct stageStruct;
WorkStruct workStruct;
DiagramStruct diagramStruct;
TimeStruct timeStruct;
DateStruct dateStruct;
WorkStruct workStructAdd;
ServiceStruct serviceStruct;



/*��������� �����
|<------------------------------Tpack---------------------------------->|
|             |<---Tn------>|											|
|<T1n>|		  |			    |							     			|
|	  |		  |			    |					         			    |
|     |       |             |											|
 tau  |       |             |											|
|  |  |       |             |											|
 1	   2	   n            �����
 __    __      __      __    __                                          __
|  |  |  |	  |  |	  |  |  |  |										|
|  |__|  |_...|  |____|  |__|  |___...__________________________________|
						|
						|
						����������� �������

*/


#endif /* IFRNS_H_ */