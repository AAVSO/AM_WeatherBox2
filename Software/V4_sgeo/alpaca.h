// WBox2 alpaca interface
#ifndef _WBOX2_ALPACA_H_
#define _WBOX2_ALPACA_H_

/*
  Alpaca documentation: 
  https://github.com/ASCOMInitiative/ASCOMRemote/blob/master/Documentation/ASCOM%20Alpaca%20API%20Reference.pdf
  https://ascom-standards.org/api/
*/

bool connected;

// for response to preUri/driverinfo
const String DriverInfo=         "V4_sgeo";
// for response to preUri/driverversion
const String DriverVersion=      DRIVER_VERSION;

// for response to /management/apiversions    and  preUri/interfaceversion 
const String InterfaceVersion= "1";   // alpaca v1
const String DiscoveryPacket= "alpacadiscovery"+ InterfaceVersion; // ends with the interface version

// for response to /management/v1/configureddevices
const String Description=        "AAVSO AM_WeatherBox2, ESP8266, NodeMCU 0.9(ESP-12 Module)" ;
const String DriverName=         "observingconditions"; // recognised ASCOM device type    also for response to preUri/name and priUri/description
#define INSTANCE_NUMBER 0
const String GUID=    "fa7b12dc-dff9-407c-a7f5-3b5e73b77c04";

// for response to /management/v1/description
const String SERVERNAME= "Alpaca device";
const String MFG= "Alan Sliski Telescope Works and George Silvis";
const String MFG_VERSION= DRIVER_VERSION; // see main
const String LOCATION= "Bourne, MA";

// ======================================
// things needed by the ASCOM API Common headers
// list of actions supported
#define SUPPORTED_ACTIONS_COUNT 0
const char* SupportedActions[]= {}; //""; // or { "Action1", "Action2b" }; 

#define SUPPORTED_METHODS_COUNT 5
const char* SupportedMethods[]= { "SkyTemperature", "Temperature" , "Humidity", "DewPoint", "RainRate"};
const char* MethodsDescription[] = { "MLX90614 infared", "SHT20", "SHT20", "computed", "RG-11, binary"} ;
double MethodsLastTime[SUPPORTED_METHODS_COUNT]= {0};

#include <ASCOMAPICommon_rest.h>  // https://github.com/gasilvis/ESP_Alpaca_common
#include <ASCOMAPIObservingConditions_rest.h>

//__FlashStringHelper* fshClientID[]= "ClientID";

// ======================================
// ObservingConditions handlers

float AveragePeriod = 0.25;  // in hours. 0 means no averaging
void handleAveragePeriodGet(void) {
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "AveragePeriod", AE_Success, "" );    
    root["Value"]= AveragePeriod;    
    serializeJson(doc, message);
    server.send(200, "application/json", message);
    return ;
}

void handleAveragePeriodPut(void) {
    String message;
    double ap;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "", AE_Success, "" ); 
    ap= (double)server.arg("AveragePeriod").toDouble();   
    if(ap >= 0.0) {
       AveragePeriod= ap;  
       root["Value"]= AveragePeriod;    
    } else {
       root["Value"]= ap;
       root["ErrorNumber"]= AE_invalidValue;
       root["ErrorMessage"] = "Cannot be negative";      
    }
       serializeJson(doc, message);
       server.send(200, "application/json", message);
}

void handleSkytemperatureGet(void) {
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "", AE_Success, "" );    
    root["Value"]= skytempC; // send reading here
    MethodsLastTime[MethodsIndex("skytemperature")]= millis();
    serializeJson(doc, message);
    server.send(200, "application/json", message);
}

void handleTemperatureGet(void) {
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "", AE_Success, "" );    
    root["Value"]= airtempC; // send reading here
    MethodsLastTime[MethodsIndex("temperature")]= millis();
    serializeJson(doc, message);
    server.send(200, "application/json", message);
}
void handleHumidityGet(void) {
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "", AE_Success, "" );    
    root["Value"]= airhum; // send reading here
    MethodsLastTime[MethodsIndex("humidity")]= millis();
    serializeJson(doc, message);
    server.send(200, "application/json", message);
}
void handleDewpointGet(void) {
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "", AE_Success, "" );    
    root["Value"]= dewpoint; // send reading here
    MethodsLastTime[MethodsIndex("dewpoint")]= millis();
    serializeJson(doc, message);
    server.send(200, "application/json", message);
}
void handleRainrateGet(void) {
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "", AE_Success, "" );    
    root["Value"]= RainSense? 0.0: 9.9; // if sensed, class as heavy rain
    MethodsLastTime[MethodsIndex("rainrate")]= millis();
    serializeJson(doc, message);
    server.send(200, "application/json", message);
}
/*
    Light rain — when the precipitation rate is < 2.5 mm (0.098 in) per hour
    Moderate rain — when the precipitation rate is between 2.5 mm (0.098 ) - 7.6 mm (0.30 in) or 10 mm (0.39 in) per hour
    Heavy rain — when the precipitation rate is > 7.6 mm (0.30 in) per hour, or between 10 mm (0.39 in) and 50 mm (2.0 in) per hour
    Violent rain — when the precipitation rate is > 50 mm (2.0 in) per hour    
 */

// ======================================
void alpaca_setup() {
   
   // management api
   server.on("/management/apiversions", handleAPIversions);
   server.on("/management/v"+InterfaceVersion+"/configureddevices", handleAPIconfiguredDevices);
   server.on("/management/v"+InterfaceVersion+"/description", handleAPIdescription);
   
   //Common ASCOM handlers
   String preUri = "/api/v"+ InterfaceVersion+ "/";
   preUri += DriverName;
   preUri += "/";
   preUri += INSTANCE_NUMBER;
   preUri += "/";
   server.on(preUri+"supportedactions",    HTTP_GET, handleSupportedActionsGet );
   server.on(preUri+"action",              HTTP_PUT, handleAction );
   server.on(preUri+"commandblind",        HTTP_PUT, handleCommandBlind );
   server.on(preUri+"commandbool",         HTTP_PUT, handleCommandBool );
   server.on(preUri+"commandstring",       HTTP_PUT, handleCommandString );
   server.on(preUri+"connected",           handleConnected );
   server.on(preUri+"description",         HTTP_GET, handleDescriptionGet );     
   server.on(preUri+"driverinfo",          HTTP_GET, handleDriverInfoGet );
   server.on(preUri+"driverversion",       HTTP_GET, handleDriverVersionGet );
   server.on(preUri+"interfaceversion",    HTTP_GET, handleInterfaceVersionGet );
   server.on(preUri+"name",                HTTP_GET, handleNameGet );

   //ObservingConditions
   server.on(preUri+"averageperiod",       HTTP_GET, handleAveragePeriodGet ); 
   server.on(preUri+"averageperiod",       HTTP_PUT, handleAveragePeriodPut ); 
   server.on(preUri+"cloudcover",          handleNotImplemented );
   server.on(preUri+"dewpoint",            HTTP_GET, handleDewpointGet );
   server.on(preUri+"humidity",            HTTP_GET, handleHumidityGet );
   server.on(preUri+"pressure",            handleNotImplemented );
   server.on(preUri+"rainrate",            HTTP_GET, handleRainrateGet );  // rainrate
   server.on(preUri+"skybrightness",       handleNotImplemented );
   server.on(preUri+"skyquality",          handleNotImplemented );
   server.on(preUri+"skytemperature",      HTTP_GET, handleSkytemperatureGet );
   server.on(preUri+"starfwhm",            handleNotImplemented );
   server.on(preUri+"temperature",         HTTP_GET, handleTemperatureGet );  // 
   server.on(preUri+"winddirection",       handleNotImplemented );
   server.on(preUri+"windgust",            handleNotImplemented );
   server.on(preUri+"windspeed",           handleNotImplemented );
   
   server.on(preUri+"refresh",             HTTP_PUT, handleRefresh);
   server.on(preUri+"sensordescription",   HTTP_GET, handleSensorDescription);
   server.on(preUri+"timesincelastupdate", HTTP_GET, handleTimeSinceLastUpdate);
   
}


// ======================================
void handleDiscovery( int udpBytesCount ) {
    char inBytes[64];
    String message;
 
    Serial.printf("UDP: %i bytes received from %s:%i\n", udpBytesCount, Udp.remoteIP().toString().c_str(), Udp.remotePort() );

    // We've received a packet, read the data from it
    Udp.read( inBytes, udpBytesCount); // read the packet into the buffer
    //Is it for us ?
    char protocol[16];
    strncpy( protocol, (char*) inBytes, 16);
    Serial.println(protocol);
    if ( strncasecmp( DiscoveryPacket.c_str(), protocol, 16 ) == 0 )
    {
      StaticJsonDocument<JSON_SIZE> doc;
      JsonObject root = doc.to<JsonObject>();
      Serial.println("responding");
      Udp.beginPacket( Udp.remoteIP(), Udp.remotePort() );
      //Respond with discovery message
      root["AlpacaPort"] = ALPACA_PORT;
      String LocalIP = String() + WiFi.localIP()[0] + "." + WiFi.localIP()[1] + "." + 
                                  WiFi.localIP()[2] + "." + WiFi.localIP()[3];
      root["IPAddress"] = LocalIP.c_str();
      root["Type"] = DriverName;
      root["Name"] = WiFi.hostname();
      root["UniqueID"] = system_get_chip_id();
      serializeJson(doc,  message);
      Udp.write( message.c_str(), strlen(message.c_str()) * sizeof(char) );
      Udp.endPacket();   
      Serial.println(message.c_str());
    }
 }
 

#endif
