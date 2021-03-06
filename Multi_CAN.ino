#include <SPI.h>          //Library for using SPI Communication 
#include <mcp2515.h>      //Library for using CAN Communication

#define potPin A0   
int potValue=0;
int lastValue = -1;
int LED = 7;
int x = -1;
int increment = 6;
int switchBoard = 5;

int check;

int tempo = 5;

bool recieved = false;

bool board = false;


//int tempValue=0;    
struct can_frame canMsg;


int CS1 = 10;
int CS2 = 9;


MCP2515 mcp2515(10); // chip select CS pin 10

void setup() 
{
  pinMode(CS1,OUTPUT);
  pinMode(CS2,OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(increment,INPUT);
  pinMode(switchBoard,INPUT);
  Serial.begin(9600);
  SPI.begin();               //Begins SPI communication
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();

  canMsg.can_id  = 0x036;           //CAN id as 0x036
  canMsg.can_dlc = 1;               //CAN data length as 1 byte
  
 
}

void loop() 
{
  potValue = analogRead(potPin);
  potValue = map(potValue,0,1023,0,255);


  check = digitalRead(switchBoard);

//--------switching which CAN Board we are using--------------------

if(check == HIGH){
  while(check == HIGH){
    check = digitalRead(switchBoard);
  }
  board = not board;
}

if (board == false){

  digitalWrite(CS1,LOW);
  digitalWrite(CS2,HIGH);

}

else{

  digitalWrite(CS1,HIGH);
  digitalWrite(CS2,LOW);
  
}

//------------------sending a message-----------------------------------

  if(not board){
  check = digitalRead(increment);
  
  if (check == HIGH){

    while(check == HIGH){
      check = digitalRead(increment);
    }

    if (tempo <= 245){
    tempo = tempo + 10;
    }
    else{
      tempo = 0;
    }
  }
  }
  else{


    tempo = 7;

    
  }

  potValue = analogRead(potPin);
  potValue = map(potValue,0,1023,0,255);



  
  
  //Serial.println(potValue);

  //if (potValue != lastValue){
  if(tempo != lastValue){  
  //sendMessage(potValue);
  //lastValue = potValue;
  sendMessage(tempo);
  lastValue = tempo;

  
  }
  
  
  recieveMessage(0x069,1);

  
  if (recieved == false){
    //Serial.println(x);
    digitalWrite(LED,HIGH);
  }
  else{
    digitalWrite(LED,LOW);
  }
  
}

void sendMessage(int y){
  canMsg.data[0] = y;        //Update pot value in [0]
  //canMsg.data[1]= 0x00;
  mcp2515.sendMessage(&canMsg);     //Sends the CAN message
  Serial.print("Sending Message: ");
  Serial.println(y);
  
}

void recieveMessage(int address, int bytes){
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) // To receive data (Poll Read)
  {
    /*if(canMsg.can_id==address)
    {*/
     x = canMsg.data[bytes-1]; 
     recieved = not recieved; 
     Serial.print("Recieved Message ");
     Serial.println(x);      
   /* }
    else{
      x = -1;
    }*/  
}     
 else{
  x = -1; 
 }
}
