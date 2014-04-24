/*
 * Stack.c
 *
 * Created: 05.12.2011 18:12:28
 *  Author: Admin
 */ 
#include "All.h"
#include <stdlib.h>


//добавление элемента в очередь
 boolean PushByteToStack(unsigned char* stack, unsigned char* sp, unsigned char msg)
 {
	 //увеличим указатель стека
	 if(((*sp)) < stackSize-1)
	 {
		 //записали по указателю
		 *(stack + *sp) = msg;
		 (*sp)++;
		 return true;
	 }
	 else
	 {
		return false;	 		 
	 }
	 
 }

//добавление элемента в начало очереди (на дно)
boolean PushToQueueBottom(unsigned char* arr, unsigned char* p, unsigned char msg)
{
	
}

//вытащить из стека (всегда с головы)
unsigned char PopByteFromStack(unsigned char* stack, volatile unsigned char* sp)
{
	unsigned char pT;
	if(*sp != 0)
	{
		(*sp)--;
		pT = *(stack + *sp);
		*(stack + *sp) = 0;
		return pT;
	}
	else
	{
		return 0;
	}
}