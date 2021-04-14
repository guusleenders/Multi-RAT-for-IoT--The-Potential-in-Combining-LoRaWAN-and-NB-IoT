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

#ifndef _CELLULAR_H
#define _CELLULAR_H

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
    #include "pins.h"    
#endif

#include <stdio.h>
#include <string.h>


// determine board type
// Arduino Geniuno / Uno or Mega
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define GNSS Serial // 115200 baud rate
  #define DEBUG Serial // 115200 baud rate
// Tinylab, Arduino Leonardo or Micro  
#elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  #define GNSS Serial1 // 115200 baud rate
  #define DEBUG Serial // 115200 baud rate
#endif

//extern HardwareSerial BG96_AT;

// enum for setMode function
enum {
    AUTO_MODE = 0,
    GSM_MODE,
    CATM1_MODE,
    CATNB1_MODE,
};

// Peripheral Pin Definations
#define BG96_ENABLE 3
#define BG96_POWERKEY 6 //
#define BG96_RESETKEY 5
#define RING_INDICATOR 7
#define DTR 8

// Constants  
#define TIMEOUT 1000
#define IP_ADDRESS_LEN 30
#define DOMAIN_NAME_LEN 50
#define PORT_NUMBER_LEN 8  
#define AT_COMM_LEN 50
#define AT_RESPONSE_LEN 200
#define DATA_COMPOSE_LEN 100
#define DATA_LEN_LEN 3  

// LTE Bands
#define LTE_B1 "1"
#define LTE_B2 "2"
#define LTE_B3 "4"
#define LTE_B4 "8"
#define LTE_B5 "10"
#define LTE_B8 "80"
#define LTE_B12 "800"
#define LTE_B13 "1000"
#define LTE_B18 "20000"
#define LTE_B19 "40000"
#define LTE_B20 "80000"
#define LTE_B26 "2000000"
#define LTE_B28 "8000000"
#define LTE_B39 "4000000000" // catm1 only
#define LTE_CATM1_ANY "400A0E189F"
#define LTE_CATNB1_ANY "A0E189F"
#define LTE_NO_CHANGE "0"

// GSM Bands
#define GSM_NO_CHANGE "0"
#define GSM_900 "1"
#define GSM_1800 "2"
#define GSM_850 "4"
#define GSM_1900 "8"
#define GSM_ANY "f"

#define SCRAMBLE_ON "0"
#define SCRAMBLE_OFF "1"

class CellularIoT
{
  public:

    /*
    Default constructer with no parameter

    [no-return]
    ---
    [no-param]
    */
    CellularIoT();
    
/******************************************************************************************
 *** Base Functions : Set or Clear Hardwares - Status Controls - Helper AT Functions  *****
 ******************************************************************************************/
    
    /*
    Function for initializing module and peripherals
    This function do things below:
    * enables BG96 module
    * sets pin directions

    [no-return]
    ---
    [no-param]
    */
    void init(); // initialize

    /*
    Function for powering up BG96 module and all peripherals from voltage regulator 

    [no-return]
    ---
    [no-param]
    */
    void enable();
    
    /*
    Function for powering down BG96 module and all peripherals from voltage regulator 

    [no-return]
    ---
    [no-param]
    */
    void disable();

    /*
    Function for powering up or down BG96 module

    [no-return]
    ---
    [no-param]
    */
    void powerUp();
	void powerDown();

        /*
    Function for getting modem status (is powered up?).
    
    [return] :  uint8_t modem status (0 : powered up) - (1: powered down)
    ---
    [no-param]
    */
    uint8_t getModemStatus();

    /*
    Function for sending AT [param #1] command to BG96.
    
    [no-return]
    ---
    [param #1] : const char* AT command word
    */
    void sendATCommOnce(const char *);
	void getSignalConnectionStatus();
	uint16_t getPacketCounterDownlink();
	uint16_t getPacketCounterUplink();
	void resetPacketCounter();

    /*
    Function for sending AT [param #1] command to BG96. If the desired [param #2] 
    response isn't recevived, function resend the AT command wait a time as [timeout].
    
    [return] : const char* response of AT command that received from bg96 modem
    ---
    [param #1] : const char* AT command word
    [param #2] : const char* AT desired_response word
    [param #3] : const char* AT response word
    */
    const char* sendATComm(const char *, const char *); 
	
	/*
    Function for sending Data [param #1] to BG96. If the desired [param #2] 
    response isn't recevived, function resend the Data wait a time as [timeout].
    
    [return] : const char* response of Data that received from bg96 modem
    ---
    [param #1] : const char* Data word
    [param #2] : const char* Data desired_response word
    [param #3] : const char* Data response word
    */
	const char* sendDataComm(const char *, const char *);
	
	/*
    Function for sending AT [param #1] command to BG96. If the desired [param #2] 
    response isn't recevived, function resend the AT command wait a time as [timeout].
    
    [no-return] 
    ---
    [param #1] : const char* AT command word
    [param #2] : const char* AT desired_response word
    [param #3] : char* response buffer for putting data in
    */
	void sendATCommBetter(const char *command, const char *desired_reponse, char * response);
    /*
    Function for resetting bg96 module and all peripherals.

    [no-return]
    ---
    [no-param]
    */
    void resetModule();

   /*
    Function to save configurations that be done in current session.   
    
    [no-return]
    --
    [no-param]
    */
    void saveConfigurations();
	void resetConfigurations();

    /*
    Function for getting IMEI number

    [return] : const char* IMEI number
    [no-param]
    */
    const char* getIMEI();

    /*
    Function for getting firmware info

    [return] : const char* firmware info
    ---
    [no-param]
    */
    const char* getFirmwareInfo();

    /*
    Function for getting hardware info

    [return] : const char* hardware info
    ---
    [no-param]
    */
    const char* getHardwareInfo();

    /*
    Function for setting GSM Band

    [no-return]
    ---
    [param #1] : const char* GSM Band
    */
    void setGSMBand(const char *);

    /*
    Function for setting Cat.M1 Band

    [no-return]
    ---
    [param #1] : const char * Cat.M1 LTE Band
    */
    void setCATM1Band(const char *);

    /*
    Function for setting NB-IoT Band

    [no-return]
    ---
    [param #1] : const char * NB_IoT LTE Band
    */
    void setNBIoTBand(const char *);

    /*
    Function for getting band configuration

    [return] : const char* band configuration
    ---
    [no-param]
    */
    const char* getBandConfiguration();

    /*
    Function for setting scramble feature configuration 

    [no-return]
    ---
    [param #1] : const char * Scramble on / off ( '0' is on, '1' is off)
    */
    void setScrambleConf(const char *);

    /*
    Function for setting running mode.
    
    [no-return]
    ---
    [param #1] : mode selector
        [MODE_AUTO] [0] --> Automatic *Priority Table (Cat.M1 -> Cat.NB1 -> GSM)
        [MODE_GSM] [1] --> GSM
        [MODE_CATM1] [2] --> Cat.M1
        [MODE_CATNB1] [3] --> Cat.NB1 (NB-IoT) 
    */
    void setMode(uint8_t);

    /*
    Function for getting described ip address

    [return] : const char* IP Address
    ---
    [no-param]
    */
    const char* getIPAddress();

    /*
    Function for setting desired ip address

    [no-return]
    ---
    [param #1] : char * desired ip address
    */
    void setIPAddress(char *);

    /*
    Function for getting described domain name

    [return] : const char* domain name
    ---
    [no-param]
    */
    const char* getDomainName();

    /*
    Function for setting desired domain name

    [no-return]
    ---
    [param #1] : char * desired domain name
    */
    void setDomainName(char *);

    /*
    Function for getting described port number

    [return] : const char* port number
    ---
    [no-param]
    */
    const char* getPort();

    /*
    Function for setting desired port number

    [no-return]
    ---
    [param #1] : char * desired port number
    */
    void setPort(char *);

    /*
    Function for getting described timeout

    [return] : uint16_t described timeout in ms
    ---
    [no-param]
    */
    uint16_t getTimeout();

    /*
    Function for setting desired timeot

    [no-return]
    ---
    [param #1] : uint16_t desired timeout in ms
    */
    void setTimeout(uint16_t);
	
	void sleep();
	void wake();
	void enableEIDRX(uint8_t edrx);
	void getCELevel();
	void getNetworkRegistrationStatus();


/******************************************************************************************
 *** Network Service Functions ************************************************************
 ******************************************************************************************/ 

    /*
    Function for getting signal quality

    [return] : const char* signal quality
    ---
    [no-param]
    */
    const char* getSignalQuality();
	void getSignalStrength();
		
    /*
    Function for getting query network info

    [return] : const char* query network info
    ---
    [no-param]
    */
    const char* getQueryNetworkInfo();
	
	void selectOperator(unsigned int op);
	void setPDPContextProximus();
		
   /*
    Function for connecting to base station of operator

    [no-return]
    ---
    [no-param]
    */
    void connectToOperator();

/******************************************************************************************
 *** GNSS Functions ***********************************************************************
 ******************************************************************************************/

    /*
    Function for connecting to base station of operator

    [no-return]
    ---
    [no-param]
    */
    void turnOnGNSS();

    /*
    Function for connecting to base station of operator

    [no-return]
    ---
    [no-param]
    */
    void turnOffGNSS();

    /*
    Function for connecting to base station of operator

    [return] : Fixed location. Lattitude and Longitude
	---
	[latitudePointer]  : float * latitudePointer pointer to float for putting latitude
	[longitudePointer]  : float * longitudePointer pointer to float for putting longitude
    */
    void getFixedLocation(float * latitudePointer, float * longitudePointer);
    

/******************************************************************************************
 *** TCP & UDP Protocols Functions ********************************************************
 ******************************************************************************************/
    /* 
    function for configurating and activating TCP context 

    [no-return]
    ---
    [no-param]
    */ 
    void activateContext();

    /* 
    function for deactivating TCP context

    [no-return]
    ---
    [no-param]
    */ 
    void deactivateContext();

	/* 
    // function for changing baud rate of BG96

    [no-return]
    ---
    [rate]  : unsigned int rate is baud rate
    */
	void changeBaudRate(unsigned int rate);

    /* 
    // function for connecting to server via UDP

    [no-return]
    ---
    [no-param]
    */
    void startUDPService();

    /*
    Function for sending data via UDP protocol. 
    First use setIPAddress and setPort functions before 
    try to send data with this function.  

    [param #1] : const char* data word
    */
    void sendDataUDP(const char *);
	void sendDataATT(const char *address, const char *key, const char *field, const char *value );

    /* 
    Function for closing server connection
    
    [no-return]
    ---
    [no-param]
    */
    void closeConnection();
	
	void getDataHTTP();
	void postDataHTTP();
	
	void sendNumberOfData(int amount);
	void setPSM();
	void setEDRX();
	void getTime(char *timeresult);

  private:
    char compose[255];
	char url[68];

    char ip_address[IP_ADDRESS_LEN]; //ip address       
    char domain_name[DOMAIN_NAME_LEN]; // domain name   
    char port_number[PORT_NUMBER_LEN]; // port number 
    uint16_t timeout = TIMEOUT; // default timeout for function and methods on this library.

/******************************************************************************************
 *** Private Functions that be used in public methods, in order to ease the operations ****
 ******************************************************************************************/
    /* 
    Function for clear command buffer #private param : compose[100].
    
    [no-return]
    ---
    [no-param]
    */
    void clear_compose()
    {
        memset(compose,0,sizeof(compose));
    }
};
#endif