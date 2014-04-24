/*
 * RecieveTrasmit.h
 *
 * Created: 05.12.2011 17:33:23
 *  Author: Admin
 */ 


#ifndef RECIEVETRASMIT_H_
#define RECIEVETRASMIT_H_

void SendData(void);
unsigned char* ParseData(unsigned char* data);
unsigned char* GetFrame(void);
void ExchangeServer(void);
boolean CheckForStack(unsigned char sp, unsigned char bytesNeeded);
void PushFrameToStack(unsigned char* stack, unsigned char* stPtr, unsigned char* toPushPtr, unsigned char bytesToSend);
void FlushStack(unsigned char* stack, unsigned char* stackPtr);

#endif /* RECIEVETRASMIT_H_ */