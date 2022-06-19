#include <OneWire.h>                                  
#include <DallasTemperature.h>                                                                      //define what pin tx is goin to be 
#include "DFRobot_PH.h"
#include <EEPROM.h>
#define PH_PIN A1
//Data wire is connected to the Arduino digital pin 44
#define ONE_WIRE_BUS 34
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DFRobot_PH ph; 

float temp,phValue,voltage,orp = 0;

String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product

void setup() {                                        //set up the hardware
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  Serial3.begin(9600);      //orp
  Serial1.begin(9600); //esp2866
  sensors.begin();
  ph.begin();
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product

}

void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}

void serialEvent3(){
   sensorstring = Serial3.readStringUntil(13);         //read the string until we see a <CR>
  sensor_string_complete = true; 
}

void loop() {                                         //here we go...
  
  if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
    Serial3.print(inputstring);                      //send that string to the Atlas Scientific product
    Serial3.print('\r');                             //add a <CR> to the end of the string
    inputstring = "";                                 //clear the string
    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
  }

 if(sensor_string_complete==true){
  sensors.requestTemperatures();
  temp= sensors.getTempCByIndex(0);
  voltage = analogRead(PH_PIN)/1024.0*5000; //read voltage 
  phValue = ph.readPH(voltage,temp);   
  orp =sensorstring.toFloat();
  orp = orp -200;
  
  Serial.println(temp);
  Serial.println(phValue);
  Serial.println(orp);

  Serial1.println(temp);
  Serial1.println(phValue);
  Serial1.println(orp);

  sensorstring = "";
  sensor_string_complete = false; 
 }
  }
