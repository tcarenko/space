/*
 * ifrns.h
 *
 * Created: 18.10.2011 18:23:47
 *  Author: Igor Tsarenko
 *	Organization: Navigation - Service
 */ 


#ifndef IFRNS_H_
#define IFRNS_H_
//тип процессора
//#define __AVR_ATxmega128A1__
#define ver_number			1
#define ver_modification	2
#define version			number"."modification
//главная системная частота. используется в sysclk.c
#define BOARD_XOSC_HZ	20000000UL
//функции**********************************************************
//сброс сторожевого таймера
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
//порты************************************************************
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

//калибровочный порт
#define CALIBPORT			PORTK
#define CALIBPIN_SOURCE		0
#define CALIBPIN_DRAIN		1

#define DACPORT			  PORTA
#define DAC_CH0_On()	  DACPORT.DIR = 0xff
#define DAC_CH0_Off()	  DACPORT.DIR = 0x00
//*****************************************************************
//типы данных******************************************************



#define s32f float

#define s08 short


//*****************************************************************

#define arrLeng		255
#define formSize	58
unsigned char timFStart;	//старт
unsigned char timFStop;	//стоп

//u08 piFlag = 0;

//опоры************************************************************
unsigned long freq; //в Гц
unsigned long freqAltera;//в Гц
//*****************************************************************
//приемный/отправной массив-стэк***********************************
#define stackSize	128

unsigned char recStack[stackSize];
unsigned char recPointer;

unsigned char transStack[stackSize];
unsigned char transPointer;

//структура пакета:
//
//	(START_FRAME) (SEND_FRAME|REQUEST_FRAME|RECIEVE_FRAME) (WHAT_FRAME) (DATA) (END_FRAME)

#define START_FRAME			0x24//признак начала передачи
#define END_FRAME			0x2a//признак конца передачи

#define SEND_FRAME			0x2b//заголовок передачи
#define REQUEST_FRAME		0x2c//заголовок запроса
#define RECIEVE_FRAME		0x2d//заголовок приема. в пакетах от компьютера

#define TIME_FRAME					0x21//заголовок времени	
#define DIAGRAM_STRUCT_FRAME		0x22//заголовок структуры диаграммы
#define IMITATOR_ONLINE_FRAME		0x23//заголовок имитатор на связи
#define SYSTEM_CHANGE_FRAME			0x25//заголовок смена системы
#define STOP_DIAGRAM_FRAME			0x26//заголовок остановки формирование диаграммы
#define START_DIAGRAM_FRAME			0x27//заголовок запуска формирования диаграммы
#define VERSION_FRAME				0x28//заголовок версии прошивки
#define SHIFT_DIAG_RIGHT_FRAME		0x29//заголовок для сдвига диаграммы вправо
#define SHIFT_DIAG_LEFT_FRAME		0x2b//заголовок для сдвига диаграммы влево
#define SERVICE_STRUCT_FRAME		0x2c//заголовок для служебной структуры

unsigned int recTransStatus;							//флаги
 
#define REC_ENABLE						0	//можно принимать
#define TRANS_ENABLE					1	//можно отправлять

#define PARSE_IN_DATA_EN				2	//парсим, что пришло
#define TIME_READY_TO_TRANS				3	//время готово для передачи
#define DIAGRAM_STRUCT_READY_TO_TRANS	4	//параметры диаграммы готовы для передачи
#define DIAGRAM_STRUCT_REQUEST			5	//запрос имитатором диаграммы с компа
#define IMITATOR_ONLINE					6	//послать ответ, что имитатор на связи
#define VERSION_TO_TRANS				7	//отослать номер версии
#define SERVICE_DIAG_TO_TRANS			8	//отослать служебную структуру
#define SERVICE_STRUCT_REQUEST			9	//запросить служебную структуру

//*****************************************************************
//шина данных порты на вывод кода**********************************
unsigned int portL;//low
unsigned int portM;//med
unsigned int portH;//high
//*****************************************************************
//шина адреса******************************************************
unsigned int portAddr;
//*****************************************************************
//Адреса***********************************************************
#define SEV_SET_ADDR					0x01
#define HTO_SET_ADDR					0x02
#define PHASE_PLUS_MINUS_SET_ADDR		0x04
#define SEV_MEAS_GET_ADDR				0x08
#define HTO_MEAS_GET_ADDR				0x10
//*****************************************************************
//массив кодов фронтов СЕВ
unsigned long arrayFrontsCodesSEV[64];
unsigned long* arrFrCodesPntrSEV;
//массив кодов фронтов ХТО
unsigned long arrayFrontsCodesHTO[64];
unsigned long* arrFrCodesPntrHTO;
//массив кодов фаз+/-
unsigned char arrayPhaseManipul[64];
unsigned char arrPhManPntr;
//*****************************************************************
typedef enum
{
	false,
	true
}boolean;
//приход секунды
unsigned int secTimeICP;
unsigned long ulSecBuf;
//дефолтная структура
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

//дефолтные фазовые коды 
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



//тип системы
typedef enum systemType
{
	Rns_e,
	Rns_v
}SystemType;
//тип сигнала
typedef enum signalType
{
	Loran,
	Chaika
}SignalType;
//модернизированный формат
typedef enum advancedFormat
{
	AdvancedFOff,
	AdvancedFOn
}AdvancedFormat;

//структура, со всеми параметрами диаграммы
typedef struct  
{//все в микросекундах
	AdvancedFormat	modernFormat;
	SystemType sysType;
	SignalType sigType;
	unsigned long Tpack;			//период пачек
	unsigned char tau;			//длительность импульсов в пачке
	unsigned int T1n;		    //период импульсов в пачке
	unsigned int Tmas;			//время от n-го импульса до импульса опознования ведущей
	unsigned int Tn;			    //время от n-ого импульса до импульса окраса
	unsigned char n;				//кол-во импульсов в пачке	
	unsigned char slaveN;			//кол-во ведомых. 0х0000ssss - s - какая по счету ведомая станция присутствует в цепочке
	unsigned long masterSlave1;	//расстояние до 1 ведомой
	unsigned long masterSlave2;	//расстояние до 2 ведомой
	unsigned long masterSlave3;	//расстояние до 3 ведомой
	unsigned long masterSlave4;	//расстояние до 4 ведомой
	//массив фазовых кодов*********************************************
	unsigned char phaseCodes[4];
	//30-ти элементный фазовый код для сек маркеров для модернизированного формата
	unsigned long phaseCodeSecLabel;
	//фазовый код для 10 маркеров минуты
	unsigned int  phaseCodeMinLabel;
	//фазовый код для 11 маркеров 5 мин
	unsigned int  phaseCode5MinLabel;
	//фазовый код для импульсов окраса Рнсв
	unsigned long phaseCodesRnsv[4];
}DiagramStruct;

typedef struct 
{
	unsigned int dacVal;	
}ServiceStruct;

//структура значений периода и регистра сравнения
typedef struct 
{
	unsigned int perPack;
	unsigned int ccaPack;

	unsigned int ccaNPlusOne;						//задаем длину импульса
	
	unsigned int perNoNMasterSlave1;					//длины и кол-ва шагов от начала ведущей пачки до начал ведомых пачек
	unsigned int nNoNMasterSlave1;
	unsigned int perNoNMasterSlave2;
	unsigned int nNoNMasterSlave2;
	unsigned int perNoNMasterSlave3;
	unsigned int nNoNMasterSlave3;
	unsigned int perNoNMasterSlave4;
	unsigned int nNoNMasterSlave4;
	
	unsigned int perNoN;						
	unsigned int ccaNo;
	unsigned int nNoN;								//кол-во по perNo от n-ого до начала след пачки
	
	unsigned int perNoNLabelMaster;					//длина шага  до конца импульса опознования ведущей
	unsigned int nNoNLabelMaster;					//кол-во шагов до конца импульса опознования ведущей
	
	unsigned int perNoNLabel;						//длина шагов для получения импульса окраса
	unsigned int nNoNLabel;							//кол-во шагов до конца импульса окраса
	
	unsigned int perNonLabelNext;					//кол-во шагов от окраса до след пачки
	unsigned int nNoNLabelNext;						//длина шага от окраса до след пачки
	
	unsigned int perSecOnly;							//периоды до секунды				
	unsigned int nSecOnly;							//кол-во периодов perSecOnly в секунде	
			
	unsigned int nSec;								//кол-во периодов пачек до секунды
	
}StageStruct;

unsigned int diagramArray[128];
unsigned char diagramArrayIndex;	
//фазовый период
typedef enum phasePer				
	{
		a,
		b
	}PhasePer;	
PhasePer phPer;
//фазовый период
typedef enum phaseMS				
	{
		master,
		slave
	}PhaseMS;	
PhaseMS phMS;

//что формируем сек, мин или 5 мин
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

//система
typedef struct
{
	SystemType		sysType;
	AdvancedFormat	advForm;
	SignalType		sigType;
}System;	

System system;
//структура с текущим состоянием формирования диаграммы
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
	unsigned char countNoneSteps;			//счетчик периодов
	s08 count;								//счетчик импульсов
	unsigned char countLabel;				//счетчик для импульса окраса
	unsigned int countSec;					//счетчик для формирования импульса, совпадающего с сек
	unsigned int countSecOnly;				//счетчик для формирования секунды
	unsigned int predFormStatus;			//предыдущее состояние формирования
	unsigned char formStatus;				//состояние формирования
	unsigned char synchronized:1;			//синхронизация таймеров
	unsigned char	countSlave1NoneSteps;	//счетчик периодов до ведомой1
	unsigned char	countSlave2NoneSteps;	//счетчик периодов до ведомой2
	unsigned char	countSlave3NoneSteps;	//счетчик периодов до ведомой3
	unsigned char	countSlave4NoneSteps;	//счетчик периодов до ведомой4
	unsigned char countSlave1;				//формируем ведомую1 счетчик импульсов
	unsigned char countSlave2;				//формируем ведомую1 счетчик импульсов
	unsigned char countSlave3;				//формируем ведомую3 счетчик импульсов
	unsigned char countSlave4;				//формируем ведомую4 счетчик импульсов
	PhasePer phasePer;						//фазовый период
	PhaseMS phaseMS;						//фазовый код	
	unsigned char* phaseCodesPtr;			//указатель на фазовый код
	unsigned char phaseBuf;					//буфер с текущим фазовым кодом	
	unsigned char phaseCount;				//счетчик сдвига фаз кода
	unsigned long* phaseCodesRnsvPtr;		//указатель на фазовый код рнсв
	unsigned long phaseRnsvBuf;				//буфер с текущим фазовым кодом рнсв
	unsigned char phaseRnsvCount;			//счетчик сдвига фаз кода рнсв
	unsigned char phaseSecCount;			//счетчик сдвига 30-ти элементного кода
	unsigned char phaseMinCount;			//счетчик сдвига фаз кода для мин маркера
	unsigned char phase5MinCount;			//счетчик сдвига фаз кода для 5 мин маркера
	DiagMode diagMode;						//
	unsigned char minMode;					//счетчик кол-ва периодов минут
	#define minModeDefault 10
	unsigned char fiveMinMode;				//счетчик кол-ва периодов 5 минут
	#define fiveMinModeDefault 11
	unsigned long totalTicks;
	unsigned long ticks;					//сдвинуть на msToShiftMs
	volatile boolean shift;
	unsigned int step;
	
	unsigned long ticksSec;					//сдвинуть на msToShiftMs
	volatile boolean shiftSec;
	unsigned int stepSec;
	
	boolean compensMissmatchInit;
	boolean shiftDone;
	
	unsigned char piFlag:1;
}WorkStruct;
//время
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



/*Диаграмма пачек
|<------------------------------Tpack---------------------------------->|
|             |<---Tn------>|											|
|<T1n>|		  |			    |							     			|
|	  |		  |			    |					         			    |
|     |       |             |											|
 tau  |       |             |											|
|  |  |       |             |											|
 1	   2	   n            окрас
 __    __      __      __    __                                          __
|  |  |  |	  |  |	  |  |  |  |										|
|  |__|  |_...|  |____|  |__|  |___...__________________________________|
						|
						|
						опознавание ведущей

*/


#endif /* IFRNS_H_ */