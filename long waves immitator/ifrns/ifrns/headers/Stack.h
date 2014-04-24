/*
 * Stack.h
 *
 * Created: 05.12.2011 18:12:42
 *  Author: Admin
 */ 


#ifndef STACK_H_
#define STACK_H_

//структура сообщения
typedef struct message 
{
	struct Message			*headPrevPointer;
	unsigned char						data;
}message;

//структура стека
typedef struct stack
{
	struct Message			*headPointer;
}stack;


//добавление элемента в очередь
boolean PushByteToStack(unsigned char* stack, unsigned char* sp, unsigned char msg);
 
//добавление элемента в начало очереди (на дно)
boolean PushToQueueBottom(unsigned char*, unsigned char*, unsigned char);

//вытащить из очереди (всегда с головы)
unsigned char PopByteFromStack(unsigned char* stack, volatile unsigned char* sp);

#endif /* STACK_H_ */