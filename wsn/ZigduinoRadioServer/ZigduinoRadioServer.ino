
#include <ZigduinoRadio.h>
/*Sketch for a server side.
Server's tasks:
  -  control MAC (give permissions for sending data for nodes)
  -  get data from nodes
  -  send data to PC
  -  send data over bluetooth*  
*/

//Global definitions---------------------------------------------------------------------
#define BOARD_ID                  255    //255 is a server board id
#define NETWORK_ID                128

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
  type1 = 0,
  type2,
  type3,
  sensTypesNum
};

//ServiceServer definitions---------------------------------------------------------------
//----------------------------------------------------------------------------------------
struct ServiceStatus{
  #define GIVE_NEW_ID_BIT  0
  #define GOT_NEW_ID_BIT   1
  
  unsigned char statusReg;
  unsigned char idToGive;
};

ServiceStatus servStatus;

//ServiceServer definitions end-----------------------------------------------------------
//----------------------------------------------------------------------------------------


//ZigBeeServer definitions---------------------------------------------------------------
//---------------------------------------------------------------------------------------
#define MEAS_DATA_BUF_SIZE	  2
#define MEAS_ARR_SIZE             16

#define START_FRAME               254
#define END_FRAME                 253

#define MAC_MESS_SIZE             6
#define NEW_ID_MESS_SIZE          6
#define MEAS_MESS_SIZE            MEAS_DATA_BUF_SIZE+1+1+1+1+1+1
#define SERV_MESS_SIZE            5

#define RX_BUFF_SIZE              MEAS_DATA_BUF_SIZE+1+1+1+1+1+1

//this structure contains different buffers and status register showing from which buffer data should be sent over ZigBee
struct ZBTXBuffStruct{
  
  #define MAC_STATUS_BIT  0
  #define SEND_NEW_ID_BIT 1
  #define TX_BUFF_BUSY    7
  
  
  unsigned char statusReg;
  unsigned char txBuffer[ZR_FIFO_SIZE];
};

struct ZBRXBuffStruct{
   #define NEW_DATA_BIT  0
   
   unsigned char statusReg;
   unsigned char rxBuffer[ZR_FIFO_SIZE];
};

ZBTXBuffStruct zbTxBuffStruct;
ZBRXBuffStruct zbRxBuffStruct;

//ZigBeeServer definitions end------------------------------------------------------------
//----------------------------------------------------------------------------------------

//MACServer definitions-------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#define MAX_NUM_OF_NODES  4
#define TIME_SLOT_PERIOD  300

struct MACStatus{
  unsigned char boardIdWithPermission;
  unsigned long ticks;
};

struct Node{
  unsigned char nodeId;
};

struct Nodes{
  Node nodesArr[MAX_NUM_OF_NODES];
  unsigned char numActiveNodes;    //number of active nodes
  unsigned char lastNode;          //last index of the node from the nodeArr, which owned the media
};

Nodes nodes;
MACStatus macStatus;

//MACServer definitions end---------------------------------------------------------------
//----------------------------------------------------------------------------------------

//Measurements definitions ---------------------------------------------------------
//----------------------------------------------------------------------------------------

//a single measurement structure
struct MeasStruct{
        unsigned char boardId;
        unsigned char sensorId;
	unsigned char data[MEAS_DATA_BUF_SIZE];
        unsigned int  used : 1;
};

//all measurements are in this structure
struct MeasArrStruct{
        MeasStruct    measArr[MEAS_ARR_SIZE];
        unsigned char numOfMeas;
};

MeasArrStruct measData;
//Measurements definitions end------------------------------------------------------------
//----------------------------------------------------------------------------------------
#define greenLedPin  0
#define redLedPin    1

void setup()
{
  board_id = BOARD_ID;
  network_id = NETWORK_ID;
  //for the experiment lets fill in the boards id manually
//  Node nodeA;
//  nodeA.nodeId = 1;
//  
//  Node nodeB;
//  nodeB.nodeId = 2;
//  
//  nodes.nodesArr[0] = nodeA;     //in the system there is a node with an id = 1;
//  nodes.nodesArr[1] = nodeB;
//  
//  nodes.numActiveNodes = 2;  //we have only 2 boards in the system
//  nodes.lastNode = 0;        //this board has an access to the media
  
  ZigduinoRadio.begin(0);
  Serial.begin(9600);
  
  ZigduinoRadio.attachError(errHandle);
  ZigduinoRadio.attachTxDone(onXmitDone);
  
  //print debug data
  Serial.print("Server id  : ");
  Serial.println(board_id);
  
  Serial.print("Network id : ");
  Serial.println(network_id);
  
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
}

void loop()
{
  ServiceServer();
  MACServer();
  ZigBeeServer();
  delay(10);
}

//ServiceServer------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ServiceServer()
{
  //check the status reg
  if(servStatus.statusReg != 0){
    //check what exactly should be done
    if(servStatus.statusReg & (1 << GIVE_NEW_ID_BIT)){
      //need to give an id to a board. we will send id, but the Node will be added to a Node Array after server gets the confirmation from a board, that is now is taken
      Serial.print("\nSRV server        : request for a new id has come");
      if(servStatus.idToGive == 0) servStatus.idToGive++;
      //put the data to txBuffer
      putDataToBuff(zbTxBuffStruct.txBuffer, NEW_ID_Message_Type);
      //save the board id to which the permission will be granted
      zbTxBuffStruct.statusReg |= (1 << SEND_NEW_ID_BIT);
      //the request has been processed - reset the bit
      servStatus.statusReg &= ~(1 << GIVE_NEW_ID_BIT);
    }
    if(servStatus.statusReg & (1 << GOT_NEW_ID_BIT)){
      //got response from a node that is got its id
      //create a node
      Node node;
      //assign new id to it
      node.nodeId = servStatus.idToGive;
      //save it in an array
      nodes.nodesArr[nodes.numActiveNodes] = node;
      //increment the number of nodes we have
      if(nodes.numActiveNodes < MAX_NUM_OF_NODES){
        nodes.numActiveNodes++;
        Serial.print("\nSRV server        : !new node in the network : id : ");
        Serial.print(node.nodeId);
        nodes.lastNode = 0;        //this board has an access to the media
      }else{
        Serial.print("\nSRV server        : no space for a new node");
      }
      //next id to give is ->
      servStatus.idToGive++;
      //reset the bit
      servStatus.statusReg &= ~(1 << GOT_NEW_ID_BIT);
    }
  }
}
//ServiceServer end--------------------------------------------------------------------
//-------------------------------------------------------------------------------------


//MACServer----------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void MACServer()
{
  //give permissions
  //check the last permission given
  macStatus.ticks++;
  //check if we have nodes in the network
  if(nodes.numActiveNodes != 0){
    if(macStatus.ticks > TIME_SLOT_PERIOD){
      //it is time to give a permission to the next node
      macStatus.ticks = 0;
      if(nodes.lastNode < nodes.numActiveNodes-1) nodes.lastNode++;
      else nodes.lastNode = 0;
      //get the id of the node, permission to which should be given
      macStatus.boardIdWithPermission = nodes.nodesArr[nodes.lastNode].nodeId;
      //form the packet with the foumd node id;
      putDataToBuff(zbTxBuffStruct.txBuffer, MAC_Message_Type);
      //set a flag, that MAC server has some data to be transmitted
      zbTxBuffStruct.statusReg |= (unsigned char)(1 << MAC_STATUS_BIT);
      //print debug data
      Serial.print("\nMAC server        : MAC permission to a board id: ");
      Serial.print(macStatus.boardIdWithPermission);
    }
  }else{
    Serial.print("\nMAC server        : No nodes in the network");
  }
}

void putDataToBuff(unsigned char* buff, unsigned char mType)
{
  buff[0] = (unsigned char)START_FRAME;                                              //start frame
  buff[1] = network_id;                                                              //network if frame
  buff[2] = board_id;                                                                //board id frame
  buff[3] = mType;  
  switch(mType)
  {
    case MAC_Message_Type: {
      buff[4] = macStatus.boardIdWithPermission;                                      //sensor id frame
      buff[5] = END_FRAME;
      break;
    }
    case NEW_ID_Message_Type: {
      buff[4] = servStatus.idToGive;
      buff[5] = (unsigned char)END_FRAME;
      break;
    }
  }
  zbTxBuffStruct.statusReg |= (1 << TX_BUFF_BUSY);
}
//MACServer end------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//ZigBeeServer-------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ZigBeeServer()
{
  if(zbTxBuffStruct.statusReg != 0){
    //there is something to send
    if(zbTxBuffStruct.statusReg & (1 << MAC_STATUS_BIT)){
      //MAC server wants to send smth
      txPacket(zbTxBuffStruct.txBuffer, MAC_MESS_SIZE);
      //sendBuffSerial(zbTxBuffStruct.txBufferMAC, TX_BUFF_MAC_SIZE);
      //Serial.println();
      //reset a flag
      zbTxBuffStruct.statusReg &= (~(1 << MAC_STATUS_BIT));
    }else if(zbTxBuffStruct.statusReg & (1 << SEND_NEW_ID_BIT)){
      //Service server wants to send smth
      txPacket(zbTxBuffStruct.txBuffer, NEW_ID_MESS_SIZE);
      //reset the flag
      zbTxBuffStruct.statusReg &= ~(1 << SEND_NEW_ID_BIT);
    }
  }
  //lets read from the media.
  unsigned char ind = 0;
  while(ZigduinoRadio.available()){
    zbRxBuffStruct.rxBuffer[ind] = ZigduinoRadio.read();
    if(!(zbRxBuffStruct.statusReg & (1 << NEW_DATA_BIT))) zbRxBuffStruct.statusReg |= (1 << NEW_DATA_BIT);
    ind++;
  }
  if(zbRxBuffStruct.statusReg & (1 << NEW_DATA_BIT)){
    Serial.print("\nZiB server        : RX buffer: ");
    sendBuffSerial(zbRxBuffStruct.rxBuffer, ind);
    //and parse what we recieved
    if(parseRxData(ind)){
      //we have a new measurement! check the recieved data
    }
  }
}

bool parseRxData(unsigned char num)
{
  zbRxBuffStruct.statusReg &= ~(1 << NEW_DATA_BIT);
  MeasStruct meas;
  bool measFound = false;
  //any packet starts from the START_FRAME ->
  for(int i = 0; i < num; i++){
     if(zbRxBuffStruct.rxBuffer[i] == START_FRAME){
       //START FRAME is found -> next network id
       if(zbRxBuffStruct.rxBuffer[i+1] == network_id){
        //check if board's id is 0
         if(zbRxBuffStruct.rxBuffer[i+2] == 0){
           //request without id
           if(zbRxBuffStruct.rxBuffer[i+3] == I_NEED_ID_Message_Type){
             //it is "give me id" request
             //check if the packet is complete
             if(zbRxBuffStruct.rxBuffer[i+4] == END_FRAME){
               servStatus.statusReg |= (1 << GIVE_NEW_ID_BIT);
             }
           }else if(zbRxBuffStruct.rxBuffer[i+3] == I_GOT_ID_Message_Type){
             //probably confirmation from a board that now it got its id
             if(zbRxBuffStruct.rxBuffer[i+5] == END_FRAME){
               //we have a complete packet
               servStatus.statusReg |= (1 << GOT_NEW_ID_BIT);
             }
           }
         }else{
          //network id is correct -> next, board id, from the array we have, is coming
           for(int j = 0; j < nodes.numActiveNodes; j++){
             if(zbRxBuffStruct.rxBuffer[i+2] == nodes.nodesArr[j].nodeId){
               meas.boardId = nodes.nodesArr[j].nodeId;
               //node id from the array has been found -> next, message type is coming
               if(zbRxBuffStruct.rxBuffer[i+3] < nodeMessTypeNum){
                 switch(zbRxBuffStruct.rxBuffer[i+3]){
                   case MEAS_Message_Type: {
                       //measurement has come
                       if(zbRxBuffStruct.rxBuffer[i+4] < sensTypesNum){
                         //sensor type has beem found -> next the data is coming
                         meas.sensorId = zbRxBuffStruct.rxBuffer[i+4];
                         //just copy the data to a struct
                         memcpy(meas.data, &zbRxBuffStruct.rxBuffer[i+5], MEAS_DATA_BUF_SIZE);
                         if(zbRxBuffStruct.rxBuffer[i+7] == END_FRAME) measFound = true;
                       }
                     break;
                   }
                 }
               }
             }
           }
         }
       }
     }
  }
  if(measFound){
    //check if a board with a permission is sending
    if(meas.boardId == macStatus.boardIdWithPermission){
        Serial.print("\nZiB server        : message from board id : ");
        Serial.print(meas.boardId);
        
        if(meas.boardId == 1){
          if(meas.data[0] == 0 && meas.data[1] == 0) digitalWrite(greenLedPin, LOW);
          else digitalWrite(greenLedPin, HIGH);
        }else{
          if(meas.data[0] == 0 && meas.data[1] == 0) digitalWrite(redLedPin, LOW);
          else digitalWrite(redLedPin, HIGH);
        }
        
        if(measData.numOfMeas < MEAS_ARR_SIZE){
          //save this measurement if we have space for it
          measData.measArr[measData.numOfMeas] = meas;
          measData.numOfMeas++;
          
          return true;
        }else{
          //can not save the measurement - no place for it. need to do smth
          //do smth
          return false;
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

void sendBuffSerial(unsigned char* buff, unsigned char num)
{
  unsigned char cou = 0;
   while(cou < num){
      Serial.print(buff[cou]);
      cou++;
  }
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
