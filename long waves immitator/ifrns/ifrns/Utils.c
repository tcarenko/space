/*
 * utils.c
 *��������������� �������
 * Created: 22.11.2011 17:03:53
 *  Author: Admin
 */ 
#include "All.h"

#define secInLepYear	31622400
#define secInYear		31536000	


unsigned char daysInMonths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
unsigned long long secInMonths[12]	 = {0, 2678400,
								   2678400+2419200, 
								   2678400+2419200+2678400, 
								   2678400+2419200+2678400+2592000,
								   2678400+2419200+2678400+2592000+2678400, 
								   2678400+2419200+2678400+2592000+2678400+2592000,
								   2678400+2419200+2678400+2592000+2678400+2592000+2678400, 
								   2678400+2419200+2678400+2592000+2678400+2592000+2678400+2678400,
								   2678400+2419200+2678400+2592000+2678400+2592000+2678400+2678400+2592000,
								   2678400+2419200+2678400+2592000+2678400+2592000+2678400+2678400+2592000+2678400,
								   2678400+2419200+2678400+2592000+2678400+2592000+2678400+2678400+2592000+2678400+2592000};

//�������� ���������
unsigned int GetCRC(void* trgt, unsigned char size)
{//�������� CRC16
	unsigned char* in = (unsigned char*)trgt;
	unsigned int crc = 0;
	for(unsigned char i = 0; i < size; i++)
	{
		crc =_crc16_update(crc,in[i]);
	}
	return crc;
}

//���������� �������� ��������� PER � CCA �������
boolean CalculatePerAndCal()
{
	if(freq == 0)
	{
		freq = 10;
		//InitializeHardwareSystem(CLK_SCLKSEL_RC32M_gc, OSC_RC32MEN_bm, 0, 1, 0, 0,PORTJADDR,PORTKADDR,PORTQADDR,PORTRADDR,PORTHADDR,1);
	}
	else if(1)
	{
		/*double div = (pow(2,TCF0.CTRLA-1));*/
		stageStruct.perPack = (freq / 1/*pow(2,TCF0.CTRLA-1)*/)*(diagramStruct.T1n /*/ 1000000*/)-1; //������ � ����� �� n ���������
		stageStruct.ccaPack = (freq / 1/*pow(2,TCF0.CTRLA-1)*/)* (diagramStruct.T1n/* / 1000000*/)  - (freq/ 1/*pow(2,TCF0.CTRLA-1)*/)* (diagramStruct.tau/* / 1000000*/); //������������ ��������
				
		
//������� �1. ���������� ���-�� ����� �� ������� �� ����� �������� ����������� �������**************************************
		
			unsigned int *ms1 = FindLengthAndNumberOfSteps(diagramStruct.masterSlave1 - diagramStruct.T1n*diagramStruct.n  - diagramStruct.Tmas , 1);
			stageStruct.nNoNMasterSlave1 = *(ms1+1);
			stageStruct.perNoNMasterSlave1 = (*ms1) - 1;
				
//������� �2. ���������� ���-�� ����� �� ������� �� ����� �������� n********************************************************
		
			unsigned int *ms2 = FindLengthAndNumberOfSteps(diagramStruct.masterSlave2 - diagramStruct.masterSlave1 - diagramStruct.T1n*diagramStruct.n, 1);
			stageStruct.nNoNMasterSlave2 = *(ms2+1);
			stageStruct.perNoNMasterSlave2 = (*ms2) - 1;
		
//������� �3. ���������� ���-�� ����� �� ������� �� ����� �������� n*******************************************************
		
			unsigned int *ms3 = FindLengthAndNumberOfSteps(diagramStruct.masterSlave3 - diagramStruct.masterSlave2 - diagramStruct.T1n*diagramStruct.n, 1);
			stageStruct.nNoNMasterSlave3 = *(ms3+1);
			stageStruct.perNoNMasterSlave3 = (*ms3) - 1;
		
//������� �4. ���������� ���-�� ����� �� ������� �� ����� �������� n*******************************************************
		
			unsigned int *ms4 = FindLengthAndNumberOfSteps(diagramStruct.masterSlave4 - diagramStruct.masterSlave3 - diagramStruct.T1n*diagramStruct.n, 1);
			stageStruct.nNoNMasterSlave4 = *(ms4+1);
			stageStruct.perNoNMasterSlave4 = (*ms4) - 1;
			
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!			
//������� ������� ����������� � ����������!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			
//�� ��������� ������� �� ������ �������***************************************************************************

		unsigned int *q = FindLengthAndNumberOfSteps(diagramStruct.Tpack - diagramStruct.masterSlave4 - diagramStruct.T1n*diagramStruct.n, 1);
		
		stageStruct.nNoN = *(q+1);
		stageStruct.perNoN = (*q)-1;
		
		
//����� ���-�� ����� � ����� ���� �� �������� ����������� ������� �� �������� ������*************************************************
		
		unsigned int *p = FindLengthAndNumberOfSteps(diagramStruct.Tn - diagramStruct.Tmas, 0);
		
		stageStruct.nNoNLabel = *(p+1);
		stageStruct.perNoNLabel = (*p)-1;

		
//����� ���-�� ����� � ����� ���� �� �������� ������ �� ���� �����*************************************************

		unsigned int* v = FindLengthAndNumberOfSteps(diagramStruct.masterSlave1 - diagramStruct.n * diagramStruct.T1n - diagramStruct.Tn, 1);
		
		stageStruct.nNoNLabelNext = *(v+1);
		stageStruct.perNonLabelNext = (*v)-1;
		
//����� ���-�� ����� � ����� ���� �� ���������� �������� ������� ����� �� �������� ����������� �������

		unsigned int* c = FindLengthAndNumberOfSteps(diagramStruct.Tmas, 0);
		
		stageStruct.nNoNLabelMaster = *(c+1);
		stageStruct.perNoNLabelMaster = (*c)-1;
		
		
//����� ���-�� ����� � ����� ���� ��� �������********************************************************		

		unsigned int *w = FindLengthAndNumberOfSteps(1000000, 0);
		
		stageStruct.nSecOnly = *(w+1);
		stageStruct.perSecOnly = (*w)-1;
			
//��������� ���-�� ����� � �������
		stageStruct.nSec = (unsigned int) FindNOK((double)1000000, (double)diagramStruct.Tpack);
	}				
	
	
	
}

//���������� ����� ����� � �� ����� (�������� - ����� � ���)
unsigned int* FindLengthAndNumberOfSteps(volatile double totalLength, unsigned char chet)
{//chet - ������ ��� �������� ���-�� ����� �����
	volatile unsigned int arr[] = {0,0};
	totalLength = round(totalLength);
	volatile double totalLengthBuf = totalLength; //����� ��������� � �������� 
	
	volatile unsigned int i = 0;
	volatile double lengthOfStep = 0.0, int_part = 0.0, fract_part = 0.0, lengthOfStepForCheck = 0.0, fract_part_i = 0.0;
	
	if(chet)
	{
		do 
		{
			i++;
			lengthOfStep = totalLengthBuf * freq /*8*/ / (/*1000000 * */i);	//������� ���
			fract_part = modf(lengthOfStep, &int_part);
			fract_part_i = modf(((double)i)/2,0);
		} 
		while(!(fract_part_i==0.0 && fract_part==0.0 && lengthOfStep<65535.0));//(fract_part_i!=0.0 && (fract_part!=0.0 || lengthOfStep>65535.0));
	}
	else
	{
		do 
		{
			i++;
			lengthOfStep = totalLengthBuf * freq /*8*/ / (/*1000000 * */i);	//������� ���
			fract_part = modf(lengthOfStep, &int_part);
			fract_part_i = modf(((double)i)/2,0);
		} 
		while(!(fract_part_i!=0.0 && fract_part==0.0 && lengthOfStep<65535.0 && i>1));//(fract_part_i!=0.0 && (fract_part!=0.0 || lengthOfStep>65535.0));
	}
	arr[0] = (unsigned int)(lengthOfStep);   //���-�� �����
	arr[1] = i;						//����� ����
	return arr;
}

//���������� ����������� ������ ��������
double FindNOK(volatile double num1, volatile double num2)
{
	volatile double numMax = 0.0;
	double* ptr;
	do
	{			
		numMax+=num1;
	} 
	while (modf((numMax/num2), ptr)!=0.0);
	
	return numMax/num2;
}


void AddSecond(DateStruct* tStr)
{
	tStr->sec++;
	if(tStr->sec >= 60){
		tStr->sec = 0;
		tStr->min++; 
	}		
	if(tStr->min >= 60){
		tStr->min = 0;
		tStr->hour++;  
	}		
	if(tStr->hour > 23){
		tStr->hour = 0;
		tStr->day++;
	}
	if(tStr->day > daysInMonths[tStr->month]){
		if(tStr->leapYear == true && tStr->month == feb){
		}	else{
			tStr->day = 1;
			tStr->month++;
		}	
	}
	if(tStr->month > 12){
		tStr->month = 0;
		tStr->year++;
	}			
}	

void SecsToTime(unsigned long secs, TimeStruct *time)
{
	volatile unsigned long ulHours;
	volatile unsigned char ucMins;
	volatile unsigned char ucSecs;
	
	ulHours = floor(secs/3600);
	ucMins = floor((secs -(ulHours*3600))/60);
	ucSecs = (secs - (ulHours*3600 + ucMins*60));
	
	time->ulHours = ulHours;
	time->ucMins = ucMins;
	time->ucSecs = ucSecs; 
}	


void ScanCalibrPins(unsigned int val)
{//��������� ������ - ����������� ������
	//0x08ed
	if(DACB.CH0DATA != val)
	{
		DACB.CH0DATA = val;
		serviceStruct.dacVal = val;
	}		
}	

unsigned long GetFnavHzMismatch(DateStruct currDate){
	//���������� ���-�� ������ �� 01.01.2005 �
	volatile unsigned long long secsFrom01012005TillNow = 0;
	//�������
	volatile DateStruct diffDate;
	diffDate.year	= currDate.year		- refYear;
	diffDate.month	= currDate.month	- refMonth;
	diffDate.day	= currDate.day		- refDay;
	diffDate.hour	= currDate.hour		- refHour;
	diffDate.min	= currDate.min		- refMin;
	diffDate.sec	= currDate.sec		- refSec;
	//������� ������
	
	secsFrom01012005TillNow += secInMonths[diffDate.month];
	
	secsFrom01012005TillNow += diffDate.sec; 
	secsFrom01012005TillNow += diffDate.min*60;
	secsFrom01012005TillNow += (unsigned long long)diffDate.hour*3600;
	secsFrom01012005TillNow += (unsigned long long)diffDate.day*86400;
	
	secsFrom01012005TillNow += (unsigned long long)diffDate.year*secInYear;
	
	//�������� �� ������������
	unsigned char leapYears = 0;

	//���� ���������� ��� �� 2005 - 2008
	unsigned int startYear = 2008;
	while(currDate.year >= startYear)
	{
		if(currDate.year == startYear){
			if(currDate.month > 2) leapYears++;		
		}		
		else leapYears++;			
		startYear+=4;
	}
		
	secsFrom01012005TillNow += leapYears*86400;
	//���� ������ � 1958 ����
	secsFrom01012005TillNow += secsFrom01011958Till01012005;
	//���� ���������� ������
	secsFrom01012005TillNow += 24;
	//
	secsFrom01012005TillNow *= 1000000;
	unsigned long remainder = secsFrom01012005TillNow % diagramStruct.Tpack;
	//������� ������� � ����� �������. � �������� �� 200 ��
	remainder *= 5;
	
	return remainder;
	
}

void ShiftDiagram(unsigned long ticks){
	workStruct.totalTicks = ticks;
	workStruct.shiftDone = false;
}

void ShiftServer(){
	if(workStruct.totalTicks != 0 && workStruct.ticks == 0 && workStruct.ticksSec == 0){
		//���������� ���-�� ����� �� ������� �������� �����
		workStruct.shift = false;
		workStruct.shiftSec = false;
		workStruct.ticks = workStruct.totalTicks;
		workStruct.ticksSec = workStruct.totalTicks;
		workStruct.totalTicks = 0;
	}else if(workStruct.ticks == 0 && workStruct.ticksSec == 0){
		workStruct.shiftDone = true;
	}
}