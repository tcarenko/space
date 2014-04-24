/*
 * Initialization.c
 *
 * Created: 18.10.2011 19:09:39
 *  Author: Admin
 */ 
#include "All.h"
//����� ��������*********************************************************

unsigned char form5[formSize] = 
{
	128,
124,
139,
108,
158,
88,
176,
72,
190,
61,
199,
55,
202,
53,
202,
55,
199,
59,
194,
65,
188,
72,
181,
78,
174,
85,
168,
91,
162,
97,
157,
102,
152,
106,
148,
110,
144,
113,
141,
116,
139,
118,
137,
120,
135,
122,
134,
123,
133,
128,
128,
128,
128,
128,
128,
128,
128,
128,
128
}	;

unsigned char form5Pi[formSize] = 
{
	128,
132,
117,
148,
98,
168,
80,
184,
66,
195,
57,
201,
54,
203,
54,
201,
57,
197,
62,
191,
68,
184,
75,
178,
82,
171,
88,
165,
94,
159,
99,
154,
104,
150,
108,
146,
112,
143,
115,
140,
117,
138,
119,
136,
121,
134,
122,
133,
123,
128,
128,
128,
128,
128,
128,
128,
128,
128,
128
};	
unsigned char form[arrLeng] = {128,
129,
130,
130,
128,
124,
120,
117,
120,
128,
139,
149,
152,
145,
128,
108,
92,
89,
102,
128,
158,
180,
183,
164,
128,
88,
61,
58,
83,
128,
176,
209,
211,
181,
128,
72,
35,
33,
68,
128,
190,
230,
232,
193,
128,
61,
18,
17,
58,
128,
199,
243,
244,
200,
128,
55,
9,
9,
54,
128,
202,
249,
249,
203,
128,
53,
7,
7,
54,
128,
202,
248,
247,
201,
128,
55,
11,
11,
56,
128,
199,
242,
241,
198,
128,
59,
18,
19,
61,
128,
194,
234,
233,
192,
128,
65,
27,
28,
67,
128,
188,
223,
222,
186,
128,
72,
38,
39,
74,
128,
181,
213,
211,
179,
128,
78,
49,
50,
80,
128,
174,
202,
201,
172,
128,
85,
59,
60,
87,
128,
168,
191,
190,
166,
128,
91,
69,
70,
93,
128,
162,
182,
181,
160,
128,
97,
78,
79,
98,
128,
157,
173,
173,
155,
128,
102,
86,
87,
103,
128,
152,
166,
165,
151,
128,
106,
93,
94,
107,
128,
148,
159,
159,
147,
128,
110,
99,
100,
111,
128,
144,
154,
153,
143,
128,
113,
105,
105,
114,
128,
141,
149,
149,
141,
128,
116,
109,
109,
117,
128,
139,
145,
145,
138,
128,
118,
112,
113,
119,
128,
137,
142,
142,
136,
128,
120,
115,
116,
121,
128,
135,
139,
139,
135,
128,
122,
118,
118,
122,
128,
134,
137,
137,
133,
128,
123,
120,
120,
123,
128,
133,
135,
135,
132,
128,
124,
122,
122,
124,
128,
132,
134,
134,
131,
128,
125,
123

};
unsigned char formPi[arrLeng] = {128,
127,
126,
126,
128,
132,
136,
139,
136,
128,
117,
107,
104,
111,
128,
148,
164,
167,
154,
128,
98,
76,
73,
92,
128,
168,
195,
198,
173,
128,
80,
47,
45,
75,
128,
184,
221,
223,
188,
128,
66,
26,
24,
63,
128,
195,
238,
239,
198,
128,
57,
13,
12,
56,
128,
201,
247,
247,
202,
128,
54,
7,
7,
53,
128,
203,
249,
249,
202,
128,
54,
8,
9,
55,
128,
201,
245,
245,
200,
128,
57,
14,
15,
58,
128,
197,
238,
237,
195,
128,
62,
22,
23,
64,
128,
191,
229,
228,
189,
128,
68,
33,
34,
70,
128,
184,
218,
217,
182,
128,
75,
43,
45,
77,
128,
178,
207,
206,
176,
128,
82,
54,
55,
84,
128,
171,
197,
196,
169,
128,
88,
65,
66,
90,
128,
165,
187,
186,
163,
128,
94,
74,
75,
96,
128,
159,
178,
177,
158,
128,
99,
83,
83,
101,
128,
154,
170,
169,
153,
128,
104,
90,
91,
105,
128,
150,
163,
162,
149,
128,
108,
97,
97,
109,
128,
146,
157,
156,
145,
128,
112,
102,
103,
113,
128,
143,
151,
151,
142,
128,
115,
107,
107,
115,
128,
140,
147,
147,
139,
128,
117,
111,
111,
118,
128,
138,
144,
143,
137,
128,
119,
114,
114,
120,
128,
136,
141,
140,
135,
128,
121,
117,
117,
121,
128,
134,
138,
138,
134,
128,
122,
119,
119,
123,
128,
133,
136,
136,
133,
128,
123,
121,
121,
124,
128,
132,
134,
134,
132,
128,
124,
122,
122,
125,
128,
131,
133

};



void InitializeHardwareSystem(unsigned char clockSource, 
					 unsigned char oscSource, 
					 unsigned char pllSource,  
					 unsigned char multiplicationFactor, 
					 unsigned char freqRange, 
					 unsigned char xoscSel,
					 unsigned int port0,
					 unsigned int port1,
					 unsigned int port2,
					 unsigned int portAddress, 
					 unsigned long portInterr,
					 unsigned char pinInterr)
{
	//InitializeWatchDogTimer();
	InitializeClock(clockSource, oscSource, pllSource, multiplicationFactor, freqRange, xoscSel);
	InitializePorts(portAddress,port0,port1,port2,portInterr, pinInterr);
	InitializeDAC();
	ScanCalibrPins(0x08ed);
	
	InitializeUART();
	InitializeDiagramParameters();
	//InitializeEventSystem();
	
	InitializeTimers();
	InitializeDMA();
	
	InitSynchTimers();
	InitializeInterrupts();
	
}
void InitializeSoftwareSystem(SystemType sysT,					//��� ������� - ����,����
							  AdvancedFormat adv,				//����������������� ������?
							  SignalType sigT,					//��� ������� - �����, �����
							  unsigned char phCodeMA,			//������� ��� ����� �
							  unsigned char phCodeMB,			//������� ��� ����� �
							  unsigned char phCodeSA,			//������� ��� ����� �
							  unsigned char phCodeSB,			//������� ��� ����� �
							  unsigned long phCodeSecLabel,		//��� ������� ������ � ����������������� ������� ����
							  unsigned long rns_v_Code0,			//��� ����
							  unsigned long rns_v_Code1,			//��� ����
							  unsigned long rns_v_Code2,			//��� ����
							  unsigned long rns_v_Code3,			//��� ����
							  unsigned long sysFreq
							  )
{
	//������� �������
	//system.sysType = sysT;
	//system.advForm = adv;
	//system.sigType = sigT;
	
	if(system.sysType == Rns_v) system.advForm = AdvancedFOff;
	
	switch (system.sigType)
	{
		case Chaika:		workStruct.count = 0; break;
		case Loran:	workStruct.count = -7; break;
	}	
	//������� ����
	diagramStruct.phaseCodes[0] = phCodeMA;
	diagramStruct.phaseCodes[1] = phCodeSA;
	diagramStruct.phaseCodes[2] = phCodeMB;
	diagramStruct.phaseCodes[3] = phCodeSB;
	
	diagramStruct.phaseCodeSecLabel		= phCodeSecLabel;
	//������� ���� ��� � 5 ��� �������� ����������� �� ����� ���������� ��������� � ����������� �� �������� �������
	diagramStruct.phaseCodeMinLabel		= 0;
	diagramStruct.phaseCode5MinLabel	= 0;
	//������� ��� ����
	diagramStruct.phaseCodesRnsv[0] = rns_v_Code0;
	diagramStruct.phaseCodesRnsv[1] = rns_v_Code1;
	diagramStruct.phaseCodesRnsv[2] = rns_v_Code2;
	diagramStruct.phaseCodesRnsv[3] = rns_v_Code3;
	
	diagramStruct.sysType = sysT;
	diagramStruct.modernFormat = adv;
	diagramStruct.sigType = sigT;
	
	//������� ��� ����� ������� ��� ����� ����
	workStruct.phaseCodesRnsvPtr = diagramStruct.phaseCodesRnsv;
	//������� ��� ����� ������� ��� �����
	workStruct.phaseCodesPtr = diagramStruct.phaseCodes;
	workStruct.phaseCount = 0;
	workStruct.count = 0;
	//��������� ��������� �������
	workStruct.phasePer	= a; 
	workStruct.phaseMS	= master;
	//����� false
	workStruct.shift = false;
	workStruct.compensMissmatchInit = false;
	workStruct.shiftDone = false;
	//�����/���� �������� �� ���
	timFStart = 3;
	timFStop  = 0;
	//������� �� �������
	freq = sysFreq;
	//
	serviceStruct.dacVal = 0x08ED;
	
}


//�������������� ��������� ���������
boolean InitializeDiagramParameters()
{//��������� �� EEPROM ����������� ���������
	unsigned char* ptr = (unsigned char*)&diagramStruct;
	unsigned int crcEE = 0;
	volatile unsigned int crc = 0;
	eeprom_busy_wait();
	eeprom_read_block(ptr, (void*)DIAGRAM_STRUCT_EEPROM_ADDRESS, sizeof(DiagramStruct));
	ptr = (unsigned char*)&crcEE;
	eeprom_busy_wait();
	eeprom_read_block(ptr, (void*)DIAGRAM_STRUCT_EEPROM_CRC, sizeof(unsigned int));
	crc = GetCRC(&diagramStruct, sizeof(DiagramStruct));
	if(crc != crcEE)
		//�������� �� ���� ������ ��������
		recTransStatus |= 1 << DIAGRAM_STRUCT_REQUEST;
		
	//��������� ��������� ���������
	ptr = (unsigned char*)&serviceStruct;
	eeprom_busy_wait();
	eeprom_read_block(ptr, (void*)SERVICE_STRUCT_EEPROM_ADDRESS, sizeof(ServiceStruct));
	ptr = (unsigned char*)&crcEE;
	eeprom_busy_wait();
	eeprom_read_block(ptr, (void*)SERVICE_STRUCT_EEPROM_CRC, sizeof(unsigned int));
	crc = GetCRC(&serviceStruct, sizeof(ServiceStruct));
	if(crc != crcEE)
		//�������� �� ���� ������ ��������
		recTransStatus |= 1 << SERVICE_STRUCT_REQUEST;
	
	while((recTransStatus & (1 << DIAGRAM_STRUCT_REQUEST)) || (recTransStatus & (1 << SERVICE_STRUCT_REQUEST)))
	{
		sei();
		PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
		recTransStatus |= (1 << TRANS_ENABLE);
		ExchangeServer();
	}
	recTransStatus &= ~(1 << TRANS_ENABLE);
	cli();
	
	DateStruct ds;
	ds.year		= 2012;
	ds.month	= 5;
	ds.day		= 8;
	ds.hour		= 12;
	ds.min		= 23;
	ds.sec		= 14;
	
	dateStruct.year		= 2012;
	dateStruct.leapYear = true;
	dateStruct.month	= may;
	dateStruct.day		= 8;
	dateStruct.hour		= 00;
	dateStruct.min		= 00;
	dateStruct.sec		= 00;
	
	//diagramStruct.slaveN = 0x0f;
	system.advForm = diagramStruct.modernFormat;
	system.sigType = diagramStruct.sigType;
	system.sysType = diagramStruct.sysType;
	unsigned long mismatch = 0;
	
	//workStruct.totalTicks = GetFnavHzMismatch(ds);
}

//�������������� ���������� ������
void InitializeWatchDogTimer()
{
	//��������� ����� ������� ������ - 8�. ��������
	while(WDT.CTRL != 0x2A)
	{//�������� ���������� ���������� ����� ����
		//������ �� ���������
		CPU_CCP=CCP_IOREG_gc; 
		//������ � �������
		WDT.CTRL = WDT_CEN_bm|WDT_PER3_bm | WDT_PER1_bm|WDT_ENABLE_bm;	
	}
}

//����� �������� � ��������� �����
void InitializeClock(unsigned char clockSource, 
					 unsigned char oscSource, 
					 unsigned char pllSource,  
					 unsigned char multiplicationFactor, 
					 unsigned char freqRange, 
					 unsigned char xoscSel)
{
	//�������� �������� �������� �������	
	switch (clockSource)
	{
		//2 ��� ����������
		case CLK_SCLKSEL_RC2M_gc:	{InitOscSource(oscSource); 
									 
									break;}
		//32 ��� ����������							
		case CLK_SCLKSEL_RC32M_gc:	{InitOscSource(oscSource);
									 
									break;}
		//������� �����							
		case CLK_SCLKSEL_XOSC_gc:	InitXOSCReg(freqRange,xoscSel);
									InitOscSource(oscSource);
									
									break;
		//����							
		case CLK_SCLKSEL_PLL_gc:	{
									 InitOscSource(pllSource);
								     InitPll(pllSource, multiplicationFactor);
								     break;
									}
	}
	
	//�������� �������� �����
	while((CLK.CTRL != clockSource))
	{//�������� ���������� ���������� ����� ����
		//������ �� ���������
		ccp_write_io((uint8_t *)&CLK.CTRL, clockSource);
	}
}
//��������� ��������� ��� �����������
void InitOscSource(unsigned char oscSource)
{
	DFLLRC32M.CTRL = 1;
	//��������� �������� ��� �����������
	OSC.CTRL |= oscSource;
	OSC.CTRL |= 8;
	//�������� ���� �������� ���������������. ��������� ���������� ������!
	while(!OSC.STATUS & oscSource){};
}
//��������� ���������� ��� ������� �������
void InitXOSCReg(unsigned char frqRange, unsigned char xoscSel)
{	
	//��������� �������� ��� ��������� ������� � ����������
	OSC.XOSCCTRL = frqRange | xoscSel;

}
//��������� ��������� ���� � ������
void InitPll(unsigned char pllSource, unsigned char multFact)
{
	//������� �������� ��� ���� � ����������� ���������
	OSC.PLLCTRL = pllSource | multFact;
	//�������� ����
	OSC.CTRL |= OSC_PLLEN_bm;
	//�������� ���� ���� ���������������. ��������� ���������� ������!
	while(!OSC.STATUS & OSC_PLLEN_bm){};
}



void InitializePorts(unsigned int portAddress, unsigned int port0, unsigned int port1, unsigned int port2, unsigned long portInterr,unsigned char pinInterr)
{//�������� ��� ���� �� ����� � �������� ������
	//1 - output, 0 - input
	PORTA.DIRSET = 0xFF;
	PORTB.DIRSET = 0xFF;
	PORTC.DIRSET = 0x00;//�����
	PORTC.PIN0CTRL = 0xC0;//����������� �������� ����, SLEW-RATE
	PORTC.PIN1CTRL = 0xC0;//����������� �������� ����, SLEW-RATE
	PORTC.PIN2CTRL = 0xC0;//����������� �������� ����, SLEW-RATE
	PORTC.PIN3CTRL = 0xC0;//����������� �������� ����, SLEW-RATE
	PORTD.DIRSET = 0xFF;
	PORTE.DIRSET = 0xFF;
	PORTF.DIRSET = 0xFF;//�����
	PORTF.PIN0CTRL = 0x40;//����������� �������� ����, 
	//������� ����
	PORTH.DIRSET = 0xFF;
	PORTJ.DIRSET = 0xFF;
	//PORTK.DIRSET = 0xFF;
	PORTQ.DIRSET = 0xFF;
	PORTR.DIRSET = 0xFF;
	
	PORTA.OUTSET = 0xFF;
	PORTB.OUTSET = 0xFF;
	PORTC.OUTSET = 0x00;
	
	PORTE.OUTSET = 0xFF;
	PORTF.OUTSET = 0xFF;
	PORTH.OUTSET = 0xFF;
	PORTJ.OUTSET = 0xFF;
	//PORTK.OUTSET = 0xFF;
	PORTQ.OUTSET = 0xFF;
	PORTR.OUTSET = 0xFF;
	
	CALIBPORT.DIRCLR |= ( 1 << CALIBPIN_DRAIN ); 
	CALIBPORT.DIRSET |= ( 1 << CALIBPIN_SOURCE );
	
	
}
void InitializeEventSystem()
{
	//����� 1: �������� TCD0 overflow
	//EVSYS.CH0MUX = 	0b11010000;
}

void InitializeTimers()
{//�������������� �������
	
	CalculatePerAndCal();
	
	InitTCCO();
	InitTCC1();
	
	InitTCDO();
	InitTCD1();
	
	InitTCEO();
	InitTCE1();
	
	InitTCFO();
	InitTCF1();	

}

void InitSynchTimers()
{
	//������� ��������� ������������. ������
	TCC0.CTRLA = 3;
	//������� ��������� ������������. ������
	TCF0.CTRLA = 3;
	//
	TCF1.CTRLA = 3;
}

void InitTCCO() 
{
	
	//�������� ���������/������ ��� ������ �, B, C
	TCC0.CTRLB =TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm/* | TC0_CCDEN_bm*/ |0x03;
	//�������� ����� � ������ � B C D
	TCC0.CTRLC = TC0_CMPA_bm | TC0_CMPB_bm | TC0_CMPC_bm | TC0_CMPD_bm;
	//input capture
	//TCF0.CTRLD = 0b00101000;//0 �����
	//���������� ���������� �� ������������ �������� ������� ���������
	TCC0.INTCTRLA = /*TC0_OVFINTLVL0_bm|*/TC0_OVFINTLVL1_bm;
	TCC0.INTCTRLB = /*TC0_CCAINTLVL0_bm|*/TC0_CCAINTLVL1_bm;
	//�������� ��������
	TCC0.PER = stageStruct.perSecOnly;
	TCC0.CCA = stageStruct.perSecOnly-80;
	TCC0.CCB = stageStruct.perSecOnly-80;
	TCC0.CCC = stageStruct.perSecOnly-80;
	//������� �������� � ���������� D, ��� ������������ ��������� �������
	TCC0.CCD = 1000 + 212; //����� + 30���
	TCF0.INTFLAGS = 0x00;
	//��� ������������� ������� � ������� ���-��
	//TCC0.CNT = 7998;
	TCC0.CNT = 4999;
}
void InitTCC1() 
{
	
}
void InitTCDO() 
{
	//�������� ���������/������ ��� ������ �, normal mode
	TCD0.CTRLB = TC0_CCAEN_bm|0x03;
	//�������� ����� � ������ �
	TCD0.CTRLC = TC0_CMPA_bm;
	//���������� ���������� �� ������������ �������� ������� ���������
	TCD0.INTCTRLA = TC0_OVFINTLVL1_bm|TC0_OVFINTLVL0_bm;
	//���������� ���������� �� ��������� ������ � ������� ���������
	//TCD0.INTCTRLB = TC0_CCAINTLVL1_bm|TC0_CCAINTLVL0_bm;
	////������ �������� ��� �������� � ������ 
	//CalculatePerAndCal();
	//�������� ��������
	unsigned char n;
	n=5;
	TCD0.PER = n*freq-1; //
	TCD0.CCA = n*freq/2;
	//������� �����
	TCD0.INTFLAGS = 0x00;
	
	TCD0.INTFLAGS = 0x00;
}
void InitTCD1() 
{
	
}
void InitTCEO() 
{
	//�������� ���������/������ ��� ������ �
	TCE0.CTRLB =TC0_CCAEN_bm /*| TC0_CCBEN_bm*/|0x03;
	//�������� ����� � ������ �
	TCE0.CTRLC = TC0_CMPA_bm/* | TC0_CMPB_bm*/;
	//input capture
	//TCF0.CTRLD = 0b00101000;//0 �����
	//���������� ���������� �� ������������ �������� ������� ���������
	TCE0.INTCTRLA =TC0_OVFINTLVL1_bm| TC0_OVFINTLVL0_bm;
	TCE0.INTCTRLB = TC0_CCAINTLVL0_bm|TC0_CCAINTLVL1_bm;
	//�������� �������� ��� ������������ �����
	TCE0.CCA = 1000;
	TCE0.PER = TCE0.CCA + 1275 +16;
	////������� �����
	TCE0.INTFLAGS = 0x00;
}
void InitTCE1() 
{
	
}	
void InitTCFO() 
{	
	//�������� ���������/������ ��� ������ �
	TCF0.CTRLB =TC0_CCAEN_bm /*| TC0_CCBEN_bm*/|0x03;
	//�������� ����� � ������ �
	TCF0.CTRLC = TC0_CMPA_bm/* | TC0_CMPB_bm*/;
	//input capture
	//TCF0.CTRLD = 0b00101000;//0 �����
	//���������� ���������� �� ������������ �������� ������� ���������
	TCF0.INTCTRLA = TC0_OVFINTLVL0_bm|TC0_OVFINTLVL1_bm;
	TCF0.INTCTRLB = TC0_CCAINTLVL0_bm|TC0_CCAINTLVL1_bm;
	//�������� �������� ��� ������������ �����
	TCF0.CCA = stageStruct.ccaPack;
	TCF0.PER = stageStruct.perPack;
	////������� �����
	workStruct.formStatus = NMakingMask;
	TCF0.INTFLAGS = 0x00;
}
void InitTCF1() 
{
	//�������� ���������/������ ��� ������ �
	TCF1.CTRLB =TC1_CCAEN_bm /*| TC0_CCBEN_bm*/|0x03;
	//�������� ����� � ������ �
	TCF1.CTRLC = TC1_CMPA_bm/* | TC0_CMPB_bm*/;
	//input capture
	//TCF0.CTRLD = 0b00101000;//0 �����
	//���������� ���������� �� ������������ �������� ������� ���������
	//TCF1.INTCTRLA = TC1_OVFINTLVL0_bm|TC1_OVFINTLVL1_bm;
	//TCF1.INTCTRLB = TC1_CCAINTLVL0_bm|TC1_CCAINTLVL1_bm;
	//�������� �������� ��� ������������ �����
	TCF1.CCA = 49999-100;
	TCF1.PER = 49999;
}


void InitializeInterrupts(void)
{
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
	sei();	
}

void InitializeUART( void )
{		

	PORTD.DIRSET = PIN3_bm;	
	PORTD.DIRCLR = PIN2_bm;
	//���������� ���������� �� ����� ��������������� � ������� ���������� TCF0
	USARTD0.BAUDCTRLA = 0x13; 
	USARTD0.BAUDCTRLB = 0xC8;
	USARTD0.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;
	//�������� ���������� �� �����
	USARTD0.CTRLA |= USART_RXCINTLVL_MED_gc;
	//�����/��������
	USARTD0.CTRLB |= USART_RXEN_bm|USART_TXEN_bm;
	
}

void InitializeDAC(void)
{
	//���A
	DACA.CTRLA = 0b00000101;//������� �����. ���
	DACA.CTRLB = 0b00000000;//������� ����� ���� ������� ���� - �� �����. �.�. ����� ����� �� ������ �������� � �������� ������
	DACA.CTRLC = 1;//left adjust
	DACA.EVCTRL = 0;//������� �����
	DACA.TIMCTRL = 0;
	
	//���A
	DACB.CTRLA = 0b00000101;//������� �����. ���
	DACB.CTRLB = 0b00000000;//������� ����� ���� ������� ���� - �� �����. �.�. ����� ����� �� ������ �������� � �������� ������
	DACB.CTRLC = 0b00001000;//���� adjust
	DACB.EVCTRL = 0;//������� �����
	DACB.TIMCTRL = 0;
}

void InitializeDMA()
{
	////����� 0
	DMA.CTRL = 0b10000000;//dma on
	//DMA.CH0.REPCNT = 0;//repeat forever
	//DMA.CH0.CTRLA = 0b10100100;//channel on, repeat, single-shot, 1 byte
	//DMA.CH0.ADDRCTRL =  0b01010000;//block, inc-source
	//DMA.CH0.TRFCNT = arrLeng;
	//DMA.CH0.TRIGSRC =0x60+0x00;// 0x60 + 0x00; //tcd0, overflow
	//
	//
	//DMA.CH0.SRCADDR0 = (int)(((long)(&(form))) & 0x000000FF);
	//DMA.CH0.SRCADDR1 = (int)(((long)(&(form))) & 0x0000FF00) >> 8;
	//DMA.CH0.SRCADDR2 = (int)(((long)(&(form))) & 0x00FF0000) >> 16;
	//
	//DMA.CH0.DESTADDR0 = (0x0319) & 0x00FF;
	//DMA.CH0.DESTADDR1 = ((0x0319) & 0xFF00) >> 8;
	//DMA.CH0.DESTADDR2 = 0;
	//
//***************************************************************************
	//����� 1 ��������� ������� ���������
	DMA.CH1.CTRLA = 0b10100100;
	DMA.CH1.REPCNT = 0;
	DMA.CH1.ADDRCTRL = 0b10001000;
	DMA.CH1.TRFCNT = 0;
	DMA.CH1.TRIGSRC = 0xA0 + 0x00; //tcf0, cca - ��������
	
	DMA.CH1.SRCADDR0 = (int)(((long)(&(timFStart))) & 0x000000FF);
	DMA.CH1.SRCADDR1 = (int)(((long)(&(timFStart))) & 0x0000FF00) >> 8;
	DMA.CH1.SRCADDR2 = (int)(((long)(&(timFStart))) & 0x00FF0000) >> 16;
	
	DMA.CH1.DESTADDR0 = (0x0A00) & 0x00FF;
	DMA.CH1.DESTADDR1 = ((0x0A00) & 0xFF00) >> 8;
	DMA.CH1.DESTADDR2 = 0;
//***************************************************************************	
	////����� 2 ��������� 1��� ������� ��� ����
	//DMA.CH2.CTRLA = 0b10100100;
	//DMA.CH2.REPCNT = 0;
	//DMA.CH2.ADDRCTRL = 0b01000100;
	//DMA.CH2.TRFCNT = 0;
	//DMA.CH2.TRIGSRC = 0x80 + 0x02; //tce0, cca
	//
	//DMA.CH2.SRCADDR0 = (int)(((long)(&(timFStart))) & 0x000000FF);
	//DMA.CH2.SRCADDR1 = (int)(((long)(&(timFStart))) & 0x0000FF00) >> 8;
	//DMA.CH2.SRCADDR2 = (int)(((long)(&(timFStart))) & 0x00FF0000) >> 16;
	//
	//DMA.CH2.DESTADDR0 = (0x0900) & 0x00FF;
	//DMA.CH2.DESTADDR1 = ((0x0900) & 0xFF00) >> 8;
	//DMA.CH2.DESTADDR2 = 0;
////////***************************************************************************	
	//////����� 3 ���������� 1��� ������� ��� ����
	//DMA.CH3.CTRLA = 0b10100100;
	//DMA.CH3.REPCNT = 0;
	//DMA.CH3.ADDRCTRL = 0b01000100;
	//DMA.CH3.TRFCNT = 0;
	//DMA.CH3.TRIGSRC = 0x80 + 0x00; //tce0, overflow
	//
	//DMA.CH3.SRCADDR0 = (int)(((long)(&(timFStop))) & 0x000000FF);
	//DMA.CH3.SRCADDR1 = (int)(((long)(&(timFStop))) & 0x0000FF00) >> 8;
	//DMA.CH3.SRCADDR2 = (int)(((long)(&(timFStop))) & 0x00FF0000) >> 16;
	//
	//DMA.CH3.DESTADDR0 = (0x0900) & 0x00FF;
	//DMA.CH3.DESTADDR1 = ((0x0900) & 0xFF00) >> 8;
	//DMA.CH3.DESTADDR2 = 0;
}
