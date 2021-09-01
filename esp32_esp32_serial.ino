// ESP # 1

#include "HardwareSerial.h"
#include "painlessMesh.h"
#include <Arduino_JSON.h>
//#include <TaskSchedulerDeclarations.h>
//Serial is used for debugging 
// Serial2 is used for serial communication with other modules.

int inputValue = 0;
String inputString = "";
boolean stringComplete = false;

#define RXD2 16
#define TXD2 17


void taskserialCallback();
int SIMBotNumber=1;
String readings;

Scheduler userScheduler; // to control your personal task
Task taskserial(2000, TASK_FOREVER, &taskserialCallback); // 2sec, forever task




String getReadings () {
  JSONVar jsonReadings;
  jsonReadings["node"] = SIMBotNumber;
  jsonReadings["temp"] = 1;
  jsonReadings["hum"] = 1;
  jsonReadings["pres"] = 1;  
  readings = JSON.stringify(jsonReadings);
  return readings;
}

void taskserialCallback() {
  String msg = getReadings();
  // Serial2.print("t1: ");
  Serial2.println(msg);
//  Serial.println(msg);
    
}

void clearbuffer ()
{  stringComplete = false;
   inputString="";
}

void serialEvent() 
{ 
  while (Serial2.available()) 
  { 
    // get the new byte:
    char inChar = (char)Serial2.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    
    if (inChar == '\n') 
    {  
       stringComplete = true;
       Serial.println(inputString);
       inputString= "";
    }
  }
}
void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);
  Serial2.begin(57600, SERIAL_8N1, RXD2, TXD2);
  delay(1000);
  Serial.println("Serial Txd is on pin: "+String(TXD2));
  Serial.println("Serial Rxd is on pin: "+String(RXD2));

  userScheduler.addTask(taskserial);
  taskserial.enable();
}

void loop() 
{ 
  userScheduler.execute();
   
  
  if(stringComplete)
   {  
     Serial.println(inputString);
     clearbuffer();
   }
      
  
  if(Serial2.available()>0) 
  {    serialEvent();    }
  
}
