
#include <ZigduinoRadio.h>

//Global definitions---------------------------------------------------------------------
#define BOARD_ID                  0      //if board's id is 0, then server gives it another id, which is > 0
#define NETWORK_ID                128
#define SERVER_BOARD_ID           255

unsigned char board_id;
unsigned char network_id;
//types of server messages
enum ServerMessageTypes{
  MAC_Message_Type = 0,
  NEW_ID_Message_Type,
  servMessTypesNum
};

//types of nodes messages
enum NodeMessageTypes{
  MEAS_Message_Type = 0,
  I_NEED_ID_Message_Type,
  I_GOT_ID_Message_Type,
  nodeMessTypeNum
};

//types of sensors we have in the system. Can be renamed to make sense
enum SensorsTypes{
  sens_type1 = 0,
  sens_type2,
  sens_type3,
  sensTypesNum
};

//ServiceServer definitions---------------------------------------------------------------
//----------------------------------------------------------------------------------------

//ServiceServer definitions end-----------------------------------------------------------
//----------------------------------------------------------------------------------------

//MACServer definitions-------------------------------------------------------------------
//----------------------------------------------------------------------------------------
struct MACStatus{
  bool isAccessGranted;
};

MACStatus macStatus;

//MACServer definitions end---------------------------------------------------------------
//----------------------------------------------------------------------------------------

//ZigBeeServer definitions---------------------------------------------------------------
//---------------------------------------------------------------------------------------
#define MEAS_DATA_BUF_SIZE	  2
#define MEAS_ARR_SIZE             8

#define START_FRAME               254
#define END_FRAME                 253

#define MEAS_MESS_SIZE            MEAS_DATA_BUF_SIZE+1+1+1+1+1+1
#define SERV_MESS_SIZE            5

struct ZBTXBuffStruct{
  
  #define I_NEED_ID_BIT              0
  #define I_GOT_ID_BIT               1
  #define MEAS_READY_TO_BE_SENT_BIT  2
  #define TX_BUFF_BUSY               7
  
  unsigned char statusReg;
  unsigned char txBuffer[ZR_FIFO_SIZE];
};

struct ZBRXBuffStruct{
  
  #define MAC_MESS_COME_BIT  0
  
  unsigned char statusReg;
  unsigned char rxBuffer[ZR_FIFO_SIZE];
};

ZBTXBuffStruct zbTxBuffStruct;
ZBRXBuffStruct zbRxBuffStruct;
//ZigBeeServer definitions end------------------------------------------------------------
//----------------------------------------------------------------------------------------

//MeasurementsServer definitions ---------------------------------------------------------------
//----------------------------------------------------------------------------------------

//a single measurement structure
struct MessStruct{
        unsigned char sensorId;
        unsigned char messType;
	unsigned char data[MEAS_DATA_BUF_SIZE];
        unsigned int  notUsed : 1;                //if notUsed is 0 then it has been used, if 1 then has NOT been used        
};

//all measurements are in this structure
struct MessArrStruct{
        unsigned char notUsedMeas;
        MessStruct    newMeas;
        MessStruct    measArr[MEAS_ARR_SIZE];
        unsigned char lastSentInd;
};

MessArrStruct messData;

#define NEW_MESS_AVALIBLE_BIT  0


unsigned char messStatus;
//MeasurementsServer definitions end------------------------------------------------------------
//----------------------------------------------------------------------------------------
#define butPin  0

void setup()
{
  //board id for, the sake of experiment, is set here manually, but ideally, it should be read from eeprom and if there is no id in eeprom, the request should be sent to a server
  board_id = BOARD_ID;
  network_id = NETWORK_ID;
  
  ZigduinoRadio.begin(0);
  Serial.begin(9600);
  
  ZigduinoRadio.attachError(errHandle);
  ZigduinoRadio.attachTxDone(onXmitDone);
  
  //print debug data
  Serial.print("Node id: ");
  Serial.println(board_id);
  
  Serial.print("Network id : ");
  Serial.println(network_id);
  
  //button pin as input
  pinMode(butPin, INPUT);
}

void loop()
{

  ServiceServer();
  MeasurementsServer();
  MACServer();
  ZigBeeServer();
  
  delay(500);
}

void ServiceServer()
{
  //if our board doesn't have its id
  static unsigned char prevId;
  if(board_id == 0){
      Serial.print("\nSRV server        : board has no id");
      //if tx buffer is free
      if(!(zbTxBuffStruct.statusReg & (1 << TX_BUFF_BUSY))){
        //send request to a server
        putDataToBuff(zbTxBuffStruct.txBuffer, I_NEED_ID_Message_Type);
        //mark tx buffer as occupied
        zbTxBuffStruct.statusReg |= (1 << TX_BUFF_BUSY);
        zbTxBuffStruct.statusReg |= (1 << I_NEED_ID_BIT);
      }
  }
  if(prevId != board_id){
    //means that now a board has its id - need to send back a response to a server, that id was recieved
    if(!(zbTxBuffStruct.statusReg & (1 << TX_BUFF_BUSY))){
      Serial.print("\nSRV server        : !board id :");
      Serial.print(board_id);
      //put data
      putDataToBuff(zbTxBuffStruct.txBuffer, I_GOT_ID_Message_Type);
      zbTxBuffStruct.statusReg |= (1 << I_GOT_ID_BIT);
    }
  }
  
  prevId = board_id;
}

void MACServer()
{
  //check if the board has id
  if(board_id != 0){
    //if new MAC message has come
    if(zbRxBuffStruct.statusReg & (1 << MAC_MESS_COME_BIT)){
      if(macStatus.isAccessGranted) {
       Serial.print("\nMAC server        : MAC access granted");
      }else{
       Serial.print("\nMAC server        : MAC access denied");
      }
      zbRxBuffStruct.statusReg &= ~(1 << MAC_MESS_COME_BIT);
    }
  }
}

//ZigBeeServer--------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void ZigBeeServer()
{
    //check if there is smth to send
    if(zbTxBuffStruct.statusReg != 0){
        //there is smth to send. check what exactly
        if(zbTxBuffStruct.statusReg & (1 << MEAS_READY_TO_BE_SENT_BIT)){
          //send measurement if MAC access is granted
          if(macStatus.isAccessGranted){
            //MEASUREMENT server wants to send smth. lets trasmit it    
            txPacket(zbTxBuffStruct.txBuffer, MEAS_MESS_SIZE);
            Serial.print("\nZiB server        : TX buffer - measurement      : ");
            sendBuffSerial(zbTxBuffStruct.txBuffer, MEAS_MESS_SIZE);
            //reset a flag
            zbTxBuffStruct.statusReg &= (~(1 << MEAS_READY_TO_BE_SENT_BIT));
          }
        }else if(zbTxBuffStruct.statusReg & (1 << I_NEED_ID_BIT)){
          //since MAC is id based and the board doesnt have its id this message should be sent without MAC access
          txPacket(zbTxBuffStruct.txBuffer, SERV_MESS_SIZE);
          //show what we have in the buffer
          Serial.print("\nZiB server        : TX buffer - I_NEED_ID request: ");
          sendBuffSerial(zbTxBuffStruct.txBuffer, SERV_MESS_SIZE);
          //reset a flag
          zbTxBuffStruct.statusReg &= (~(1 << I_NEED_ID_BIT));
        }else if(zbTxBuffStruct.statusReg & (1 << I_GOT_ID_BIT)){
          txPacket(zbTxBuffStruct.txBuffer, SERV_MESS_SIZE+1);
          //show what we have in the buffer
          Serial.print("\nZiB server        : TX buffer - I_GOT_ID message : ");
          sendBuffSerial(zbTxBuffStruct.txBuffer, SERV_MESS_SIZE+1);
          //reset a flag
          zbTxBuffStruct.statusReg &= (~(1 << I_GOT_ID_BIT));
        }
        //release tx buffer
        zbTxBuffStruct.statusReg &= ~(1 << TX_BUFF_BUSY);
    }
  
  //lets read from the media
  unsigned char ind = 0;
  while(ZigduinoRadio.available()){
    zbRxBuffStruct.rxBuffer[ind] = ZigduinoRadio.read();
    ind++;
  }
  //Serial.print("\nZiB server        : RX buffer: ");
  //sendBuffSerial(zbRxBuffStruct.rxBuffer, ind);
  //Serial.println();
  //and parse what we recieved
  if(parseRxData(ind)){
    
  }
}

void sendBuffSerial(unsigned char* buff, unsigned char num)
{
 unsigned char cou = 0;
   while(cou < num){
      Serial.print(buff[cou]);
      cou++;
  }
}

bool parseRxData(unsigned char num)
{
  //any packet starts from the START_FRAME ->
  for(int i = 0; i < num; i++){
     if(zbRxBuffStruct.rxBuffer[i] == START_FRAME){
       //START_FRAME is found -> next, network id
       //Serial.print("\nStart frame is found");
       if(zbRxBuffStruct.rxBuffer[i+1] == network_id){
         //network id is correct -> next, board id is coming
         //Serial.print("\nNetwork id is correct");
         if(zbRxBuffStruct.rxBuffer[i+2] == (unsigned char)SERVER_BOARD_ID){
           //message from a server. next byte is the server message type
           //Serial.print("\nServer id is found");
           if(zbRxBuffStruct.rxBuffer[i+3] < servMessTypesNum){
             switch(zbRxBuffStruct.rxBuffer[i+3])
             {
               case MAC_Message_Type:
               {
                 bool isAccessGranted;
                 //we have a MAC message from a server. next byte is the board id to which access is granted
                 if(board_id == zbRxBuffStruct.rxBuffer[i+4]) isAccessGranted = true; 
                 else isAccessGranted = false;
                 if(zbRxBuffStruct.rxBuffer[i+5] == END_FRAME){
                   zbRxBuffStruct.statusReg |= (1 << MAC_MESS_COME_BIT);
                   //Serial.print("\nZiB server        : MAC message from a server");
                   macStatus.isAccessGranted = isAccessGranted;
                 }
                 return true;
               }
               case NEW_ID_Message_Type:
               {
                 //this is a message from a server, which contains requested id
                 if(board_id == 0) {
                   if(zbRxBuffStruct.rxBuffer[i+5] == END_FRAME){
                     //packet is complete - we can use the content
                     board_id = zbRxBuffStruct.rxBuffer[i+4];
                     //now we need to send to a server response that this is is used now!
                   }
                 }
                 break;
               }
               //possibly there are another types of messages from a server -> just add more cases
             }
           } 
         }
       }
     }
  }
  return false;
}

void txPacket(unsigned char* buff, unsigned char num)
{
  //transmission
  ZigduinoRadio.beginTransmission();
  ZigduinoRadio.write(buff, num);
  ZigduinoRadio.endTransmission();
}

void errHandle(radio_error_t err)
{
  //Serial.println();
  //Serial.print("ZigBeeServer: error");
  //Serial.println();
}

void onXmitDone(radio_tx_done_t x)
{
  //Serial.println();
  //Serial.print("ZigBeeServer: txDone");
  //Serial.println();
}
//ZigBeeServer end----------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//MeasurementsServer--------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void MeasurementsServer()
{
  if(board_id == 0) return;
  if(!(messStatus & (1 << NEW_MESS_AVALIBLE_BIT))) getNewDataMEAS();
  putNewMeasInArrMEAS();
  getLastUnsentMessagesMEAS();
}

void getNewDataMEAS()
{
  static char i = 0;
  //create new measurement. HERE I manually put some rnd values to a measData.newMeas structure
  messData.newMeas.sensorId = sens_type1;
  messData.newMeas.messType = MEAS_Message_Type;
  //here we get voltage level on the butPin
  int val = digitalRead(butPin);
  if(val == HIGH) {
    Serial.print("\nMSR server        : button released");
    messData.newMeas.data[0]  = 0;
  }
  else if(val == LOW) {
    Serial.print("\nMSR server        : button pressed");
    messData.newMeas.data[0]  = 1;
  }
  messData.newMeas.data[1]  = 0;
  messData.newMeas.notUsed  = 1;
  //i++;
  
  messStatus |= (1 << NEW_MESS_AVALIBLE_BIT);
}

void putNewMeasInArrMEAS()
{
  //search for a place for a new message - start searching from the tail of the array
  //check if there is a new measurement
  if(!(messStatus & (1 << NEW_MESS_AVALIBLE_BIT))) return;
  //save it
  for(int i = 0; i < MEAS_ARR_SIZE; i++){
    if(messData.measArr[i].notUsed == 0) {
      //place at index i either empty or contains used measurement - can be used
      messData.measArr[i] = messData.newMeas; 
      //reset the bit - means that the measurement was stored
      messStatus &= ~(1 << NEW_MESS_AVALIBLE_BIT);
      //printAllMEAS();
      return;
    }
  }
  Serial.print("\nMSR server        : no space for a new measurement");
}

void printAllMEAS()
{
  Serial.println();
  for(int i = 0; i < MEAS_ARR_SIZE; i++){
    Serial.print(messData.measArr[i].data[0]);
    Serial.print("-");
    Serial.print(messData.measArr[i].notUsed);
    Serial.print("; ");
  }
}

void getLastUnsentMessagesMEAS()
{
  //wait till everythis is sent
  if(zbTxBuffStruct.statusReg & (1 << MEAS_READY_TO_BE_SENT_BIT)) return; 
  for(int i = messData.lastSentInd; i < MEAS_ARR_SIZE; i++){
    if(messData.measArr[i].notUsed == 1) {
      //not used measurement is found - put it to a buffer
      messData.notUsedMeas = i;
      //check if tx buffer is free
      if(!(zbTxBuffStruct.statusReg & (1 << TX_BUFF_BUSY))){
        //put measurement to a buffer
        putDataToBuff(zbTxBuffStruct.txBuffer, MEAS_Message_Type);
        //mark buffer as occupied
        zbTxBuffStruct.statusReg |= (1 << TX_BUFF_BUSY);
        //set a flag that the measurement is used
        messData.measArr[i].notUsed = 0;
        //set a flag that we ready to send the measurement
        zbTxBuffStruct.statusReg |= (1 << MEAS_READY_TO_BE_SENT_BIT);
      }
      break;
    }
  }
  if(++messData.lastSentInd >= MEAS_ARR_SIZE) messData.lastSentInd = 0;
}

void putMeasToBuffMEAS(unsigned int ind)
{
  zbTxBuffStruct.txBuffer[0] = (unsigned char)START_FRAME;                                              //start frame
  zbTxBuffStruct.txBuffer[1] = network_id;                                                              //network if frame
  zbTxBuffStruct.txBuffer[2] = board_id;                                                                //board id frame
  zbTxBuffStruct.txBuffer[3] = messData.measArr[ind].messType;                                          //message type
  zbTxBuffStruct.txBuffer[4] = messData.measArr[ind].sensorId;                                          //sensor id frame
  memcpy(&zbTxBuffStruct.txBuffer[5], messData.measArr[ind].data, MEAS_DATA_BUF_SIZE);                  //copy data to a buffer
  zbTxBuffStruct.txBuffer[5+MEAS_DATA_BUF_SIZE] = (unsigned char)END_FRAME; 
}

void putDataToBuff(unsigned char* buff, unsigned char mType)
{
  buff[0] = (unsigned char)START_FRAME;                                              //start frame
  buff[1] = network_id;                                                              //network if frame
  buff[2] = board_id;                                                                //board id frame
  buff[3] = mType;  
  switch(mType)
  {
    case MEAS_Message_Type: {
      buff[4] = messData.measArr[messData.notUsedMeas].sensorId;                                      //sensor id frame
      memcpy(&buff[5], messData.measArr[messData.notUsedMeas].data, MEAS_DATA_BUF_SIZE);
      zbTxBuffStruct.txBuffer[5+MEAS_DATA_BUF_SIZE] = (unsigned char)END_FRAME; 
      break;
    }
    case I_NEED_ID_Message_Type: {
      buff[4] = (unsigned char)END_FRAME;
      break;
    }
    case I_GOT_ID_Message_Type: {
      buff[2] = 0;
      buff[4] = board_id;
      buff[5] = (unsigned char)END_FRAME;
      break;
    }
  }
  zbTxBuffStruct.statusReg |= (1 << TX_BUFF_BUSY);
}

//MeasurementsServer end----------------------------------------------------------------
//--------------------------------------------------------------------------------------
