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
//���������� ����� ����� � �� �����
unsigned int* FindLengthAndNumberOfSteps(double totalLength, unsigned char chet);
//���������� ���
double FindNOK(volatile double num1, volatile double num2);
//�������� �������
void AddSeconds(DateStruct* tStr);
//������ ���-�� ������ �� 1.1.2005 �
unsigned long GetFnavHzMismatch(DateStruct currDate);
//������������ ������������� ����
void ScanCalibrPins(unsigned int);
//��� ������ ���������
void ShiftDiagram(unsigned long);
//
void ShiftServer();
#endif /* UTILS_H_ */