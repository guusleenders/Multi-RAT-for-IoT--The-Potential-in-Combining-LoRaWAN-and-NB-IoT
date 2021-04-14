/*
  localHost.ino - This is basic local host example.
  Created by Yasin Kaya (selengalp), August 18, 2018.

*/

#include "CellularIoT.h"

CellularIoT node;

char your_ip[] = "81.11.131.98"; // change with your ip
char your_port[] = "8891"; // change with your port

#define SIGNALPIN 52
#define STEP 20
#define START 0

// setup
void setup() {
  pinMode(8, OUTPUT);
  //

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println("start");
  pinMode(SIGNALPIN, OUTPUT);
  digitalWrite(SIGNALPIN, LOW);

  // Boot
  digitalWrite(SIGNALPIN, HIGH);
  node.init();
  Serial1.flush();
  digitalWrite(SIGNALPIN, LOW);
  node.changeBaudRate(9600);

  node.getIMEI();
  node.getFirmwareInfo();
  node.getHardwareInfo();

  node.setIPAddress(your_ip);
  node.setPort(your_port);

  //node.setGSMBand(GSM_sli900);
  //node.setCATM1Band(LTE_B5);
  //node.setNBIoTBand(LTE_B20);
  //node.getBandConfiguration();
  node.setMode(CATNB1_MODE);
  //node.setMode(GSM_MODE);

  node.selectOperator(20601);
  //node.setPDPContextProximus();
  digitalWrite(SIGNALPIN, LOW);

  // Connect to operator
  digitalWrite(SIGNALPIN, HIGH);
  node.connectToOperator();
  Serial1.flush();
  digitalWrite(SIGNALPIN, LOW);
  
  delay(10);
  node.getSignalQuality();
  node.getQueryNetworkInfo();

  // Activate context
  digitalWrite(SIGNALPIN, HIGH);
  node.deactivateContext();
  node.activateContext();
  node.closeConnection();

  node.setEDRX();

  
  node.startUDPService();
  Serial1.flush();
  digitalWrite(SIGNALPIN, LOW);
  
  
  //node.sendDataATT("y8HCg0RqXXa47tI46mdlwccK","maker:4JSt6bR2Sibzm0lqFyIDRS5If0RQ22bTDqMTYra","gps","{\"latitude\":51.2565, \"longitude\":7.751654}");

  //node.turnOnGNSS();
  //node.postDataHTTP();
  Serial.println("settings edrx");
  
}

// loop
void loop() {
  node.setTimeout(1000);
  /*float lat, lon;
  node.getFixedLocation(&lat, &lon);

  char payloadValue[64];

  int lat1 = floor(lat);
  int lon1 = floor(lon);
  unsigned int lat2 = abs(round((lat - lat1) * 10000));
  unsigned int lon2 = abs(round((lon - lon1) * 10000));

  sprintf(payloadValue, "{\"latitude\":%d.%04d, \"longitude\":%d.%04d}", lat1, lat2, lon1, lon2);
  Serial.println(payloadValue);

  node.sendDataATT("y8HCg0RqXXa47tI46mdlwccK", "maker:4JSt6bR2Sibzm0lqFyIDRS5If0RQ22bTDqMTYra", "gps", payloadValue);
*/
  digitalWrite(8, HIGH);
  delay(200);
  digitalWrite(8, LOW);
  
  Serial.println("---MEASUREMENTS---");
  node.getCELevel();
  node.getSignalStrength();
  node.getSignalQuality();
  node.getSignalQuality();
  node.getNetworkRegistrationStatus();
  Serial.println("------------------");

  node.resetPacketCounter();
  node.getPacketCounterDownlink();
  node.getPacketCounterUplink();

  uint16_t downlink = 0;
  uint16_t uplink = 0;

  delay(40000);
  
  for(uint16_t i=START; i<=1460; i=i+STEP){
  //int i = 5;
    // Packet
      node.deactivateContext();
      
      digitalWrite(SIGNALPIN, HIGH);
      downlink = node.getPacketCounterDownlink();
      uplink = node.getPacketCounterUplink();
      
      node.activateContext();
      node.startUDPService();
      if(i == 0)
        node.sendNumberOfData(1);
      else
        node.sendNumberOfData(i);
      node.getPacketCounterUplink();
      node.getPacketCounterDownlink();
      node.getPacketCounterUplink();
      node.getPacketCounterDownlink();
      node.closeConnection();
      node.deactivateContext();
      
      Serial1.flush();
      Serial.flush();

      Serial.println("Sent");
      delay(6000);

      digitalWrite(SIGNALPIN, LOW);
      
      uint16_t currentDownlink = node.getPacketCounterDownlink();
      uint16_t currentUplink = node.getPacketCounterUplink();
      Serial.print("Downlink: ");
      Serial.println(currentDownlink);
      Serial.print("Uplink: ");
      Serial.println(currentUplink);

      if(downlink != currentDownlink || uplink == currentUplink){
        Serial.print("-----------RETRY ");
        Serial.print(i);
        Serial.println("-----------");
        i = i-STEP;
      }
      downlink = currentDownlink;
      
      delay(12000);

    }

    
  
  

  delay(200000);
  digitalWrite(6, LOW); 
  delay(100);
  digitalWrite(6, HIGH);

}
