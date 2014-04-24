/*
 * utils.h
 *
 * Created: 22.11.2011 17:04:06
 *  Author: Admin
 */ 


#ifndef UTILS_H_
#define UTILS_H_

unsigned int GetCRC(void* trgt, unsigned char size);
boolean CalculatePerAndCal(void);
//нахождения числа шагов и их длины
unsigned int* FindLengthAndNumberOfSteps(double totalLength, unsigned char chet);
//нахождение НОК
double FindNOK(volatile double num1, volatile double num2);
//прибавим секунды
void AddSeconds(DateStruct* tStr);
//расчет кол-ва секунд от 1.1.2005 г
unsigned long GetFnavHzMismatch(DateStruct currDate);
//просканируем калибровочные ноги
void ScanCalibrPins(unsigned int);
//для сдвига диаграммы
void ShiftDiagram(unsigned long);
//
void ShiftServer();
#endif /* UTILS_H_ */