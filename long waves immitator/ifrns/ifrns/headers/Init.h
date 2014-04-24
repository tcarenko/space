/*
 * init.h
 *
 * Created: 18.10.2011 19:54:12
 *  Author: Admin
 */ 


#ifndef INIT_H_
#define INIT_H_

//инициализация железа
void InitializeHardwareSystem(unsigned char clockSource,		//источник опоры
					 unsigned char oscSource,					//источник осцил
					 unsigned char pllSource,					//источник фапч
					 unsigned char multiplicationFactor,		//коэфф умнож
					 unsigned char freqRange,					//диапазон частот для внешнего источника
					 unsigned char xoscSel,					//диапазон номиналов кварца
					 unsigned int port0,						//код на порт
					 unsigned int port1,						//код на порт
					 unsigned int port2,						//код на порт
					 unsigned int portAddress,				//порт строб
					 
					 unsigned long portInterr,				//порт внешнего прерывания
					 unsigned char pinInterr					//пин внешнего прерывания	
					 );	
//инициализация софта
void InitializeSoftwareSystem(SystemType sysT, 	
							  AdvancedFormat adv,						 	
							  SignalType sigT,
							  unsigned char phCodeMA,
							  unsigned char phCodeSA,
							  unsigned char phCodeMB,
							  unsigned char phCodeSB,
							  unsigned long phCodeSecLabel,
							  unsigned long rns_v_Code0,			//код рнсв
							  unsigned long rns_v_Code1,			//код рнсв
							  unsigned long rns_v_Code2,			//код рнсв
							  unsigned long rns_v_Code3,				//код рнсв
							  unsigned long sysFreq
							  );	
//инициализация опоры
void InitializeClock(unsigned char clockSource,				//источник опоры
					 unsigned char oscSource,					//источник осцил
					 unsigned char pllSource,					//источник фапч
					 unsigned char multiplicationFactor,		//коэфф умнож
					 unsigned char freqRange,					//диапазон частот для внешнего источника
					 unsigned char xoscSel);					//диапазон номиналов кварца
//инициализация опоры для осциллятора
void InitOscSource(unsigned char oscSource);
//установка источника ФАПЧ и запуск
void InitPll(unsigned char pllSource, unsigned char multFact);
//установка диапазонов для внешней частоты
void InitXOSCReg(unsigned char frqRange, unsigned char xoscSel);
//инициализация WDT
void InitializeWatchDogTimer(void);
//инициализация портов
void InitializePorts(unsigned int portAddress, unsigned int port0, unsigned int port1, unsigned int port2, unsigned long portInterr,unsigned char pinInterr);
//инициализация системы событий
void InitializeEventSystem();
//инициализация таймеров
void InitializeTimers(void);
void InitTCC0(void);
void InitTCC1(void);
void InitTCD0(void);
void InitTCD1(void);
void InitTCE0(void);
void InitTCE1(void);
void InitTCF0(void);
void InitTCF1(void);
//синхронизация
void InitSynchTimers(void);
//инициализация ЦАП
void InitializeDAC(void);
//инициализация DMA
void InitializeDMA(void);
//инициализируем высокоточное расширение
void InitializeHiRezExtension(void);
//инициализация расширения формирования диаграмм
void InitializeAWEX(void);
//инициализация контроллера прерываний
void InitializeInterrupts(void);
//инициализируем параметры диаграммы
boolean InitializeDiagramParameters();
//проверка структуры
unsigned int CheckStruct(void* trgt, unsigned char size);
//инициализация блока передачи/приема данных
void InitializeUART(void);
//инициализация кодов фронтов
void InitializeCodesParams(void);
//инициализация фаз+/-
void InitializationPhaseParams(void);


#endif /* INIT_H_ */