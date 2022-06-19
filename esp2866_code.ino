#include "defines.h"
#include "Credentials.h"
#include <MySQL_Generic.h>
#include <SoftwareSerial.h>

//started softwareSerial at Rx and Tx of ESP8266
#define USING_HOST_NAME     true

#if USING_HOST_NAME
  // Optional using hostname, and Ethernet built-in DNS lookup
   char server[] = "h2ocapstone2022.ddns.net"; // change to your server's hostname/URL
#else
  IPAddress server(149, 61, 26, 75);
#endif


uint16_t server_port = 3306;    //3306;

char default_database[] = "Sensors_h2o";           //"test_arduino";
char table_1[]    = "sensors_data";         //"test_arduino";

char INSERT_DATA[] = "INSERT INTO %s.%s (sensor_name_1,value_1,sensor_name_2,value_2,sensor_name_3,value_3) VALUES ('%s',%f,'%s',%d,'%s',%d)";                
char query[128];

//String values,arduino_data,tempString,phString,orpString; 
float arduino_data;
MySQL_Connection conn((Client *)&client);

#if !( ESP32 || ESP8266 || defined(CORE_TEENSY) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3) || defined(STM32F4) || defined(STM32F7) || ( defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_ARCH_MBED) ) ) 

char *dtostrf(double val, signed char width, unsigned char prec, char *sout)
{
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
#endif

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  MYSQL_DISPLAY1("\nStarting Complex_Insert_WiFi on", BOARD_NAME);
  MYSQL_DISPLAY(MYSQL_MARIADB_GENERIC_VERSION);

  // Remember to initialize your WiFi module
#if ( USING_WIFI_ESP8266_AT  || USING_WIFIESPAT_LIB ) 
  #if ( USING_WIFI_ESP8266_AT )
    MYSQL_DISPLAY("Using ESP8266_AT/ESP8266_AT_WebServer Library");
  #elif ( USING_WIFIESPAT_LIB )
    MYSQL_DISPLAY("Using WiFiEspAT Library");
  #endif
  
  // initialize serial for ESP module
  EspSerial.begin(9600); /// I changed ***
  // initialize ESP module
  WiFi.init(&EspSerial);

  MYSQL_DISPLAY(F("WiFi shield init done"));

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD)
  {
    MYSQL_DISPLAY(F("WiFi shield not present"));
    // don't continue
    while (true);
  }
#endif

  // Begin WiFi section
  MYSQL_DISPLAY1("Connecting to", ssid);

  WiFi.begin(ssid);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
   // delay(500);
    MYSQL_DISPLAY0(".");
  }

  // print out info about the connection:
  MYSQL_DISPLAY1("Connected to network. My IP address is:", WiFi.localIP());

  MYSQL_DISPLAY3("Connecting to SQL Server @", server, ", Port =", server_port);
  MYSQL_DISPLAY5("User =", user, ", PW =", password, ", DB =", default_database);
}

void runInsert()
{
  // Initiate the query class instance
  MySQL_Query query_mem = MySQL_Query(&conn);

  if (conn.connected())
  {
    bool Sr =false;
  if(Serial.available()) 
   {
    
    arduino_data = Serial.read();
    Sr=true;
   }
   

   if(Sr==true){
 //   values=arduino_data;
//    int fristCommaIndex = values.indexOf(',');
 //   int secondCommaIndex = values.indexOf(',', fristCommaIndex+1);
 //   int thirdCommaIndex = values.indexOf(',', secondCommaIndex + 1);
    //get sensors data from values variable by  spliting by commas and put in to variables  
//  String tempString = values.substring(0, fristCommaIndex);
//  String phString = values.substring(fristCommaIndex+1, secondCommaIndex);
  //String orpString = values.substring(secondCommaIndex+1);
  float temp = arduino_data;
  int ph= 56;
  int orp= 97;
  sprintf(query, INSERT_DATA, default_database,table_1,"temp sensor",temp,"pH sensor",ph,"ORP sensor",orp);
   }
 // int temp = arduino_data.toInt();
 // int ph= 56;
 // int orp= 97; //orpString.toInt(); 
  
  //  sprintf(query, INSERT_DATA, default_database,table_1,"temp sensor",temp,"pH sensor",ph,"ORP sensor",orp);
  
 //  delay(10);
    // Execute the query
    MYSQL_DISPLAY(query);
  
    
    // KH, check if valid before fetching
   if ( !query_mem.execute(query) )
    {
      MYSQL_DISPLAY("Complex Insert error");
    }
    else
    {
      MYSQL_DISPLAY("Complex Data Inserted.");
    }
  // }
  }  // if (conn.connected())
  else
  {
    MYSQL_DISPLAY("Disconnected from Server. Can't insert.");
  }
  
}

void loop()
{
  MYSQL_DISPLAY("Connecting...");
  
  //if (conn.connect(server, server_port, user, password))
  if (conn.connectNonBlocking(server, server_port, user, password) != RESULT_FAIL)
  {
 //   delay(500);
    runInsert();
  conn.close();                     // close the connection
  } 
  else 
  {
    MYSQL_DISPLAY("\nConnect failed. Trying again on next iteration.");
  }

  MYSQL_DISPLAY("\nSleeping...");
  MYSQL_DISPLAY("================================================");
 
//  delay(2000);
}
