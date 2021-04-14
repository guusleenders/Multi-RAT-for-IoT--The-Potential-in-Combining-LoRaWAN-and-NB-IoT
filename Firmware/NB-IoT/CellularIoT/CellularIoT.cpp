/*
     ____  ____      _    __  __  ____ ___
  * |  _ \|  _ \    / \  |  \/  |/ ___/ _ \
  * | | | | |_) |  / _ \ | |\/| | |  | | | |
  * | |_| |  _ <  / ___ \| |  | | |__| |_| |
  * |____/|_| \_\/_/   \_\_|  |_|\____\___/
  *                           research group
  *                             dramco.be/
  *
  *  KU Leuven - Technology Campus Gent,
  *  Gebroeders De Smetstraat 1,
  *  B-9000 Gent, Belgium
 
  CellularIoT.cpp 
  -
  BG96 Library based on Sixfab Arduino CellularIoT Library (Yasin Kaya (selengalp))
  Created by Dramco 
  
*/

#include "CellularIoT.h"


#define BG96_AT Serial1
#define DEBUG Serial

// default
CellularIoT::CellularIoT()
{

}

/******************************************************************************************
 *** Base Functions : Set or Clear Hardwares - Status Controls - Helper AT Functions  *****
 ******************************************************************************************/

// function for initializing BG96 module.
void CellularIoT::init()
{
  // setting pin directions
  pinMode(DTR, OUTPUT);
  pinMode(RING_INDICATOR, INPUT);
  
  enable();

  // setting serials
  BG96_AT.begin(9600);
  DEBUG.begin(115200);
  
  powerUp();
  
  DEBUG.println("Module initializing");
  delay(500); // wait until module ready.

  delay(3000);
  sendATComm("ATE1","OK\r\n"); 
  sendATComm("ATE0","OK\r\n");
  sendATComm("AT","OK\r\n");
}

// power up BG96 module and all peripherals from voltage regulator 
void CellularIoT::enable()
{
  pinMode(BG96_ENABLE, OUTPUT);
  digitalWrite(BG96_ENABLE,HIGH);
}

// power down BG96 module and all peripherals from voltage regulator 
void CellularIoT::disable()
{
  digitalWrite(BG96_ENABLE,LOW);
}

// power up or down BG96 module
void CellularIoT::powerUp()
{
  pinMode(BG96_POWERKEY,OUTPUT);
  delay(10);
  digitalWrite(BG96_POWERKEY,HIGH);
  delay(500);
  digitalWrite(BG96_POWERKEY,LOW);
  
  delay(10);
  digitalWrite(BG96_RESETKEY,HIGH);
  delay(500);
  digitalWrite(BG96_RESETKEY,LOW);
  
}

void CellularIoT::powerDown()
{
  sendATComm("AT+QPOWD=1","OK\r\n");
  sendATComm("","POWERED DOWN");
}

// power up or down BG96 module
/*uint8_t CellularIoT::getModemStatus()
{
  //pinMode(STATUS,INPUT);
  //delay(10);
  return digitalRead(STATUS);
}*/

// send at comamand to module
void CellularIoT::sendATCommOnce(const char *comm)
{
	DEBUG.println(comm);
  BG96_AT.print(comm);
  BG96_AT.print("\r");
  //DEBUG.println(comm);
}

// function for sending at command to BG96_AT.
const char* CellularIoT::sendATComm(const char *command, const char *desired_reponse)
{
  uint32_t timer;
  char response[AT_RESPONSE_LEN]; // module response for AT commands. 

  memset(response, 0 , AT_RESPONSE_LEN);
  BG96_AT.flush();

  sendATCommOnce(command);

  timer = millis();
   while(true){
    if(millis()-timer > timeout){
      sendATCommOnce(command);
      timer = millis();
    }
    char c;
    int i = 0; 

    while(BG96_AT.available()){
      c = BG96_AT.read();
      DEBUG.write(c);
      response[i++]=c;
      delay(2);
      }
      if(strstr(response, desired_reponse)){
        return response;
        memset(response, 0 , strlen(response));
        break;
      }    
  }
}

// function for sending at command to BG96_AT.
void CellularIoT::sendATCommBetter(const char *command, const char *desired_reponse, char * response)
{
  uint32_t timer;

  BG96_AT.flush();

  sendATCommOnce(command);

  timer = millis();
   while(true){
    if(millis()-timer > timeout){
      sendATCommOnce(command);
      timer = millis();
    }
    char c;
    int i = 0; 

    while(BG96_AT.available()){
      c = BG96_AT.read();
      DEBUG.write(c);
      response[i++]=c;
      delay(2);
      }
      if(strstr(response, desired_reponse)){
        return;
      }    
  }
}


// function for sending data to BG96_AT.
const char* CellularIoT::sendDataComm(const char *command, const char *desired_reponse){
  uint32_t timer;
  char response[AT_RESPONSE_LEN]; // module response for AT commands. 

  memset(response, 0 , AT_RESPONSE_LEN);
  BG96_AT.flush();

  BG96_AT.print(command);
  int i=0 ;
  timer = millis();
  while(true){
	
    if(millis()-timer > timeout){
      BG96_AT.print(command);
      timer = millis();
	  i = 0; 
    }
    char c;
    

    while(BG96_AT.available()){
      c = BG96_AT.read();
      DEBUG.write(c);
      response[i++]=c;
	  delay(1);
	  Serial.print("response");
	  Serial.println(response);
	  if(strstr(response, desired_reponse)){
		return response;
		memset(response, 0 , strlen(response));
		break;
	  }

    }
	if(strstr(response, desired_reponse)){
		return response;
		memset(response, 0 , strlen(response));
		break;
	}

  }
}

// function for reset BG96_AT module
void CellularIoT::resetModule()
{
  //saveConfigurations();
  delay(200);

  digitalWrite(BG96_ENABLE,LOW);
  delay(200);
  digitalWrite(BG96_ENABLE,HIGH);
  delay(200);

  powerUp();
}

// Function for save configurations that be done in current session. 
void CellularIoT::saveConfigurations()
{
  sendATComm("AT&W","OK\r\n");
}

// Function for reset configurations
void CellularIoT::resetConfigurations()
{
  sendATComm("ATZ","OK\r\n");
}

// Function for getting IMEI number
const char* CellularIoT::getIMEI()
{
  return sendATComm("AT+CGSN","OK\r\n");
}

// Function for getting firmware info
const char* CellularIoT::getFirmwareInfo()
{
  return sendATComm("AT+CGMR","OK\r\n");
}

//Function for getting hardware info
const char* CellularIoT::getHardwareInfo()
{
  return sendATComm("AT+CGMM","OK\r\n");
}


// Function for setting GSM Band
void CellularIoT::setGSMBand(const char *gsm_band)
{
  strcpy(compose, "AT+QCFG=\"band\",");
  strcat(compose, gsm_band);
  strcat(compose, ",");
  strcat(compose, LTE_NO_CHANGE);
  strcat(compose, ",");
  strcat(compose, LTE_NO_CHANGE);

  sendATComm(compose,"OK\r\n");
  clear_compose();
}

// Function for setting Cat.M1 Band
void CellularIoT::setCATM1Band(const char *catm1_band)
{
  strcpy(compose, "AT+QCFG=\"band\",");
  strcat(compose, GSM_NO_CHANGE);
  strcat(compose, ",");
  strcat(compose, catm1_band);
  strcat(compose, ",");
  strcat(compose, LTE_NO_CHANGE);

  sendATComm(compose, "OK\r\n");
  clear_compose();
}

// Function for setting NB-IoT Band
void CellularIoT::setNBIoTBand(const char *nbiot_band)
{
  strcpy(compose, "AT+QCFG=\"band\",");
  strcat(compose, GSM_NO_CHANGE);
  strcat(compose, ",");
  strcat(compose, LTE_NO_CHANGE);
  strcat(compose, ",");
  strcat(compose, nbiot_band);

  sendATComm(compose,"OK\r\n");
  clear_compose();
}

// Function for getting current band settings
const char* CellularIoT::getBandConfiguration()
{
  return sendATComm("AT+QCFG=\"band\"","OK\r\n");
}


// Function for setting scramble feature configuration 
void CellularIoT::setScrambleConf(const char *scramble)
{
  strcpy(compose, "AT+QCFG=\"nbsibscramble\",");
  strcat(compose, scramble);
  sendATComm(compose,"OK\r\n");
  clear_compose();
}

//Function for setting running mode.
void CellularIoT::setMode(uint8_t mode)
{
  if(mode == AUTO_MODE){
    sendATComm("AT+QCFG=\"nwscanseq\",00,1","OK\r\n");
    sendATComm("AT+QCFG=\"nwscanmode\",0,1","OK\r\n");
    sendATComm("AT+QCFG=\"iotopmode\",2,1","OK\r\n");
    DEBUG.println("Modem configuration : AUTO_MODE");
    DEBUG.println(F("*Priority Table (Cat.M1 -> Cat.NB1 -> GSM)"));
  }else if(mode == GSM_MODE){
    sendATComm("AT+QCFG=\"nwscanseq\",01,1","OK\r\n");
    sendATComm("AT+QCFG=\"nwscanmode\",1,1","OK\r\n");
    sendATComm("AT+QCFG=\"iotopmode\",2,1","OK\r\n");
    DEBUG.println(F("Modem configuration : GSM_MODE"));
  }else if(mode == CATM1_MODE){
    sendATComm("AT+QCFG=\"nwscanseq\",02,1","OK\r\n");
    sendATComm("AT+QCFG=\"nwscanmode\",3,1","OK\r\n");
    sendATComm("AT+QCFG=\"iotopmode\",0,1","OK\r\n");
    DEBUG.println(F("Modem configuration : CATM1_MODE"));
  }else if(mode == CATNB1_MODE){
    sendATComm("AT+QCFG=\"nwscanseq\",03,1","OK\r\n");
    sendATComm("AT+QCFG=\"nwscanmode\",3,1","OK\r\n");
    sendATComm("AT+QCFG=\"iotopmode\",1,1","OK\r\n");
    DEBUG.println(F("Modem configuration : CATNB1_MODE ( NB-IoT )"));
  }
}

// function for getting ip_address
const char* CellularIoT::getIPAddress()
{
  return ip_address;
}

// function for setting ip_address
void CellularIoT::setIPAddress(char *ip)
{
  strcpy(ip_address, ip);
}

// function for getting domain_name
const char* CellularIoT::getDomainName()
{
  return domain_name;
}

// function for setting domain name
void CellularIoT::setDomainName(char *domain)
{
	strcpy(domain_name, domain);
}

// function for getting domain_name
const char* CellularIoT::getPort()
{
  return port_number;
}

// function for setting port
void CellularIoT::setPort(char *port)
{
	strcpy(port_number, port);
}

// get timout in ms
uint16_t CellularIoT::getTimeout()
{
  return timeout;
}

// set timeout in ms    
void CellularIoT::setTimeout(uint16_t new_timeout)
{
  timeout = new_timeout; 
}

// Function for enabling sleep
void CellularIoT::sleep(){
  sendATComm("AT+QSCLK=1","OK\r\n");
  digitalWrite(DTR, HIGH);
}

// Function for waking up
void CellularIoT::wake(){
  digitalWrite(DTR, LOW);
}

// Function for enabling sleep
void CellularIoT::enableEIDRX(uint8_t edrx){
  sendATComm("AT+CEDRXS=1,5,\"0000\"","OK\r\n");
}

/******************************************************************************************
 *** Network Service Functions ************************************************************
 ******************************************************************************************/ 

//
const char* CellularIoT::getSignalQuality(){
  return sendATComm("AT+CSQ","OK\r\n");
}

//
const char* CellularIoT::getQueryNetworkInfo(){
  return sendATComm("AT+QNWINFO","OK");
}
  
// connect to base station of operator
void CellularIoT::selectOperator(unsigned int op){
  printf(compose, "AT+COPS=1,2,\"%d\",9", op);
  sendATComm(compose,"OK\r\n");
	Serial.println("done");
}

// connect to base station of operator
void CellularIoT::setPDPContextProximus(){
	sendATComm("AT+CGDCONT=1,\"IP\",\"m2minternet.proximus.be\"","OK\r\n"); 
}


// connect to base station of operator
void CellularIoT::connectToOperator(){
  DEBUG.println(F("Trying to connect base station of operator..."));
  sendATComm("AT+CGATT?","+CGATT: 1\r\n");
  
  getSignalQuality(); 
}

// Function for requesting enhancement level
void CellularIoT::getCELevel(){
  sendATComm("AT+QCFG=\"celevel\"","OK\r\n");
}

// Function for requesting signal strength
void CellularIoT::getSignalStrength(){
  sendATComm("AT+QCSQ","OK\r\n");
}

// Function for requesting network registration status
void CellularIoT::getNetworkRegistrationStatus(){
  sendATComm("AT+CEREG=4","OK\r\n");
  sendATComm("AT+CEREG?","OK\r\n");
}


/******************************************************************************************
 *** GNSS Functions ***********************************************************************
 ******************************************************************************************/

// Function for turning on GNSS
void CellularIoT::turnOnGNSS(){
  sendATComm("AT+QGPS=1","OK\r\n");
}


// Function for turning of GNSS
void CellularIoT::turnOffGNSS(){
  sendATComm("AT+QGPSEND","OK\r\n");
}


// Function for getting fixed location 
void CellularIoT::getFixedLocation(float * latitudePointer, float * longitudePointer){

  char response[AT_RESPONSE_LEN]; // module response for AT commands. 
  memset(response, 0 , AT_RESPONSE_LEN);

  sendATCommBetter("AT+QGPSLOC?","+QGPSLOC:",response);
  char * startLocation = strstr(response, "+QGPSLOC: ");
  
  char tempBuffer[10];
  strncpy(tempBuffer, startLocation+19, 2);
  tempBuffer[2] = '\0';
  int latitudeDegrees = atoi(tempBuffer);
  
  strncpy(tempBuffer, startLocation+21,8);
  tempBuffer[8] = '\0';
  float latitudeMinutes = atof(tempBuffer);

  int latitudeSign;
  if(*(startLocation + 28) == 'N'){
	  latitudeSign = 1;
  }else{
	  latitudeSign = -1;
  }
  
  strncpy(tempBuffer, startLocation+30,3);
  tempBuffer[3] = '\0';
  int longitudeDegrees = atoi(tempBuffer);
  
  strncpy(tempBuffer, startLocation+33,8);
  tempBuffer[6] = '\0';
  float longitudeMinutes = atof(tempBuffer);
  
  int longitudeSign;
  if(*(startLocation + 40) == 'E'){
	  longitudeSign = 1;
  }else{
	  longitudeSign = -1;
  }
 
  *latitudePointer = (latitudeDegrees + latitudeMinutes/60)*latitudeSign;
  *longitudePointer = (longitudeDegrees + longitudeMinutes/60)*longitudeSign;  
} 

void CellularIoT::getTime(char *timeresult){
	char response[AT_RESPONSE_LEN];
	sendATCommBetter("AT+QLTS=2","+QLTS:",response);
	strncpy(timeresult, startLocation+9, 2);
	Serial.println("---timeresult---");
	Serial.println(timeresult);
	Serial.println("----");
}

/******************************************************************************************
 *** TCP & UDP Protocols Functions ********************************************************
 ******************************************************************************************/

// function for configurating and activating TCP context 
void CellularIoT::activateContext(){
  sendATComm("AT+QICSGP=1","OK\r\n"); 
  delay(1000);
  sendATComm("AT+QIACT=1","\r\n");
}

// function for deactivating TCP context 
void CellularIoT::deactivateContext(){
  sendATComm("AT+QIDEACT=1","\r\n");
}

// function for connecting to server via UDP
void CellularIoT::startUDPService(){
  char *port = "3005";

  strcpy(compose, "AT+QIOPEN=1,1,\"UDP SERVICE\",\"");
  strcat(compose, ip_address);
  strcat(compose, "\",0,");
  strcat(compose, port_number);
  strcat(compose, ",0");
  
  sendATComm(compose,"OK\r\n");
  clear_compose();

  sendATComm("AT+QISTATE=0,1","\r\n");
}

// fuction for sending data via udp.
void CellularIoT::sendDataUDP(const char *data){
  char data_len[5];
  sprintf(data_len, "%d", strlen(data));

  strcpy(compose, "AT+QISEND=1,");
  strcat(compose, "100");
  strcat(compose, ",\"");
  strcat(compose, ip_address);
  strcat(compose, "\",");
  strcat(compose, port_number);

  sendATComm(compose,">");

  sendATComm(data,"SEND OK");
  clear_compose();
}

void CellularIoT::changeBaudRate(unsigned int rate){
	clear_compose();
	sprintf(compose, "AT+IPR=%u;&W", rate);
	sendATComm(compose,"OK");
	BG96_AT.end();
	BG96_AT.end();
	BG96_AT.begin(rate);
}

void CellularIoT::sendDataATT(const char *address, const char *key, const char *field, const char *value ){
  timeout = 20000;

  char data_len[4];
  uint8_t l= strlen(address)+1+strlen(key)+1+strlen(field)+4+strlen(value)+11; //
  sprintf(data_len, "%d", l);
  
  clear_compose();
  
  
  strcpy(compose, "AT+QISEND=1,");
  strcat(compose, data_len);
  strcat(compose, ",\"");
  strcat(compose, ip_address);
  strcat(compose, "\",");
  strcat(compose, port_number);
  sendATComm(compose,">");
  strcpy(compose, address);
  strcat(compose, "\n");
  strcat(compose, key);
  strcat(compose, "\n");
  
  BG96_AT.print(compose);

  clear_compose();
  //sprintf(compose, "{\"%s\":{\"value\":%s}}", field, value);
  sprintf(compose, "{\"%s\":{\"value\":%s}}", field, value);
  sendDataComm(compose,"SEND OK");


  clear_compose();
  delay(2000);
}

void CellularIoT::sendNumberOfData(int amount){
  timeout = 20000;

  char data_len[4];
  memset(data_len, '\0', 4);
  int l = amount; //
  sprintf(data_len, "%d", l);
  
  Serial.println(l);
  Serial.println(data_len);
  clear_compose();
  
  
  strcpy(compose, "AT+QISEND=1,");
  strcat(compose, data_len);
  strcat(compose, ",\"");
  strcat(compose, ip_address);
  strcat(compose, "\",");
  strcat(compose, port_number);
  Serial.println(compose);
 
  sendATComm(compose,">");
  
  const int packsize = 160;
  int sent = 0;
  int packlen = 0;
	while(sent < l){
		delay(10);
		if(l - sent > packsize){
			packlen = packsize;
			memset(compose, 'a', packlen);
			compose[packlen] = '\0';
			Serial.println("send print now");
			BG96_AT.print(compose);
			Serial.println("done sending print");
		}
		else{
			packlen = l - sent;
			memset(compose, 'a', packlen);
			compose[packlen] = '\0';
			Serial.println("send command now");
			sendDataComm(compose,"SEND OK");
			Serial.println("done sending command");
		}
		
		sent += packsize;
	}
	Serial.println("end while");
  
  
  /*clear_compose();
  
  int secondCounter = 0;
  for(int firstCounter = 0; firstCounter < i; firstCounter++){
	if(secondCounter == 0){
		strcpy(compose, "a");
	}else{
		strcat(compose, "a");
	}
	if(secondCounter == 203){
		BG96_AT.print(compose);
		clear_compose();
		secondCounter = 0;
	}
	secondCounter++;
  }
  
  sendDataComm(compose,"SEND OK");
  clear_compose();
  delay(2000);*/
}


// function for closing server connection
void CellularIoT::closeConnection(){
  sendATComm("AT+QICLOSE=1","\r\n");
}

void CellularIoT::getSignalConnectionStatus(){
  sendATComm("AT+QCSCON?","OK\r\n");
}

uint16_t CellularIoT::getPacketCounterDownlink(){
	const uint8_t bufsize = 32;
  char buffer[bufsize];
  memset(buffer, 0, bufsize);
  sendATCommBetter("AT+QGDCNT?","OK\r\n", buffer);

  char * nextPointer;
  nextPointer = strtok(buffer," ,\"");
  uint8_t index = 0;
  
  uint16_t result = 0;
  
  while (nextPointer != NULL){
	Serial.println(nextPointer);
    if(index == 2){
        result = atoi(nextPointer);
	}
	nextPointer = strtok(NULL, ",\"");
	index ++;
  }
  
  return result;
}

uint16_t CellularIoT::getPacketCounterUplink(){
	const uint8_t bufsize = 32;
  char buffer[bufsize];
  memset(buffer, 0, bufsize);
  sendATCommBetter("AT+QGDCNT?","OK\r\n", buffer);
  char * nextPointer;
  nextPointer = strtok(buffer," ,\"");
  uint8_t index = 0;
  uint16_t result = 0;
  while (nextPointer != NULL){
    if(index == 1){
        result = atoi(nextPointer);
	}
	nextPointer = strtok(NULL, ",\"");
	index ++;
  }
  
  return result;
}

void CellularIoT::resetPacketCounter(){
  sendATComm("AT+QGDCNT=0","OK\r\n");
}


void CellularIoT::setPSM(){
  sendATComm("AT+CPSMS=1,,,\"01100101\",\"00001111\"", "OK\r\n");
}

void CellularIoT::setEDRX(){
  sendATComm("AT+CEDRXRDP", "OK\r\n");
  sendATComm("AT+CEDRXS=?", "OK\r\n");
  sendATComm("AT+CEDRXS?", "OK\r\n");
  sendATComm("AT+CEDRXS=1,5,\"0001\"", "OK\r\n");
  sendATComm("AT+CEDRXRDP", "OK\r\n");
  sendATComm("AT+QPSMS=1,,,\"00000100\",\"00011110\"", "OK\r\n");
  sendATComm("AT+QPSMS?", "OK\r\n");
}

/******************************************************************************************
 *** HTTP Protocols Functions ********************************************************
 ******************************************************************************************/
 
 // In development
 void CellularIoT::getDataHTTP(){
  timeout = 20000;
  
  sendATComm("AT+QHTTPCFG=\"contextid\",1","OK\r\n");
  sendATComm("AT+QHTTPCFG=\"responseheader\",1","OK\r\n");
  sendATComm("AT+QIACT?","OK\r\n");
  sendATComm("AT+QICSGP=1,1,\"m2minternet.proximus.be\",\"\",\"\",0","OK\r\n");
  sendATComm("AT+QIACT=1","OK\r\n");
  sendATComm("AT+QIACT?","OK\r\n");
  sendATComm("AT+QIDNSCFG=1,\"8.8.8.8\",\"8.8.4.4\"","OK\r\n");
  sendATComm("AT+QHTTPURL=22,80","CONNECT\r\n");
  //BG96_AT.print("http://www.google.com/");
  sendATComm("http://www.google.com/","OK\r\n");
  sendATComm("AT+QHTTPGET=80","OK\r\n");
  delay(1000);
  sendATComm("AT+QHTTPREAD=80","OK\r\n");
  /*
  char data_len[4];
  uint8_t l= strlen(address)+1+strlen(key)+1+strlen(field)+4+strlen(value)+11; //
  sprintf(data_len, "%d", l);
  
  clear_compose();
  
  
  strcpy(compose, "AT+QISEND=1,");
  strcat(compose, data_len);
  strcat(compose, ",\"");
  strcat(compose, ip_address);
  strcat(compose, "\",");
  strcat(compose, port_number);
  sendATComm(compose,">");
  strcpy(compose, address);
  strcat(compose, "\n");
  strcat(compose, key);
  strcat(compose, "\n");
  
  BG96_AT.print(compose);

  clear_compose();
  //sprintf(compose, "{\"%s\":{\"value\":%s}}", field, value);
  sprintf(compose, "{\"%s\":{\"value\":%s}}", field, value);
  sendDataComm(compose,"SEND OK");


  clear_compose();
  delay(2000);*/
}

void CellularIoT::postDataHTTP(){
  timeout = 20000;
  
  sendATComm("AT+QHTTPCFG=\"contextid\",1","OK\r\n");
  sendATComm("AT+QHTTPCFG=\"responseheader\",1","OK\r\n");
  sendATComm("AT+QIACT?","OK\r\n");
  sendATComm("AT+QICSGP=1,1,\"m2minternet.proximus.be\",\"\",\"\",0","OK\r\n");
  sendATComm("AT+QIACT=1","OK\r\n");
  sendATComm("AT+QIACT?","OK\r\n");
  sendATComm("AT+QIDNSCFG=1,\"8.8.8.8\",\"8.8.4.4\"","OK\r\n");
  sendATComm("AT+QHTTPURL=30,80","CONNECT\r\n");
  //BG96_AT.print("http://www.google.com/");
  sendATComm("https://postman-echo.com/post/","OK\r\n");
  sendATComm("AT+QHTTPPOST=20,80,80","CONNECT\r\n");
  sendATComm("Message=HelloQuectel","OK\r\n");
  delay(1000);
  sendATComm("AT+QHTTPREAD=80","OK\r\n");
  /*
  char data_len[4];
  uint8_t l= strlen(address)+1+strlen(key)+1+strlen(field)+4+strlen(value)+11; //
  sprintf(data_len, "%d", l);
  
  clear_compose();
  
  
  strcpy(compose, "AT+QISEND=1,");
  strcat(compose, data_len);
  strcat(compose, ",\"");
  strcat(compose, ip_address);
  strcat(compose, "\",");
  strcat(compose, port_number);
  sendATComm(compose,">");
  strcpy(compose, address);
  strcat(compose, "\n");
  strcat(compose, key);
  strcat(compose, "\n");
  
  BG96_AT.print(compose);

  clear_compose();
  //sprintf(compose, "{\"%s\":{\"value\":%s}}", field, value);
  sprintf(compose, "{\"%s\":{\"value\":%s}}", field, value);
  sendDataComm(compose,"SEND OK");


  clear_compose();
  delay(2000);*/
}




