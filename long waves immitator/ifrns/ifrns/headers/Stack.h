/*
 * Stack.h
 *
 * Created: 05.12.2011 18:12:42
 *  Author: Admin
 */ 


#ifndef STACK_H_
#define STACK_H_

//��������� ���������
typedef struct message 
{
	struct Message			*headPrevPointer;
	unsigned char						data;
}message;

//��������� �����
typedef struct stack
{
	struct Message			*headPointer;
}stack;


//���������� �������� � �������
boolean PushByteToStack(unsigned char* stack, unsigned char* sp, unsigned char msg);
 
//���������� �������� � ������ ������� (�� ���)
boolean PushToQueueBottom(unsigned char*, unsigned char*, unsigned char);

//�������� �� ������� (������ � ������)
unsigned char PopByteFromStack(unsigned char* stack, volatile unsigned char* sp);

#endif /* STACK_H_ */