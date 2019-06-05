/*
 * ESP8266 NodeMCU Real Time Graphs Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define LED 2  //On board LED

//SSID and Password of your WiFi router
static const char ssid[] = "CRI-MAKERLAB";//"CRI-MAKERLAB";
static const char password[] = "--criMAKER--";//"--criMAKER--";
MDNSResponder mdns;

static void writeLED(bool);

ESP8266WebServer server(80); //Server on port 80
WebSocketsServer webSocket = WebSocketsServer(81);

#include "page2.h" //Our HTML webpage contents with javascripts

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================

bool movSTATUS;

// fonction activée lorsqu'on reçoit des données dans le websocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\r\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // payload = contenu du message envoyé
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\r\n", num, payload);

      if (payload[0] == '1') {  //lorsque on recoit "1" dans le websock, appelle la fonction "A"
        A();
      }
      else if (payload[0] == '2') {     
         S();  
      }
      else if (payload[0] == '3') {  
         R();  
      } 
      else if (payload[0] == '4') {
        G();
      }            
      else if (payload[0] == '5') {
         D();  
      }      
      else {
        Serial.println("Unknown command");
      }
      // send data to all connected clients
      //webSocket.broadcastTXT(payload, length);  // envoie le message reçu à tous les appareils connectés
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\r\n", num, length);
      hexdump(payload, length);

      // echo data back to browser
      webSocket.sendBIN(num, payload, length);
      break;
    default:
      Serial.printf("Invalid WStype [%d]\r\n", type);
      break;
  }
}


void handleRoot() {
 server.send_P(200, "text/html", INDEX_HTML);
 Serial.println("handleRoot");
}

void handleNotFound()
{
  Serial.println("notfound");
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


//===============================================================
// Fonctions contrôlant le déplacement du robot
//===============================================================
void A() {
Serial.print("le robot avance"); 
webSocket.sendTXT(0, "le robot avance");     
digitalWrite(16, HIGH);
digitalWrite(15, HIGH);
digitalWrite(0, LOW);
digitalWrite(13, LOW); 
}

void R() {
Serial.print("le robot recule");
webSocket.sendTXT(0, "le robot recule"); 
digitalWrite(16, LOW);
digitalWrite(15, LOW);
digitalWrite(0, HIGH);
digitalWrite(13, HIGH); 
}

void G() {
Serial.print("le robot tourne vers la gauche");
webSocket.sendTXT(0, "le robot tourne vers la gauche"); 
digitalWrite(16, HIGH);
digitalWrite(15, LOW);
digitalWrite(0, LOW);
digitalWrite(13, HIGH);
}

void D() {
Serial.print("le robot tourne vers la droite");
webSocket.sendTXT(0, "le robot tourne vers la droite"); 
digitalWrite(16, LOW);
digitalWrite(15, HIGH);
digitalWrite(0, HIGH);
digitalWrite(13, LOW);
}

void S() {
Serial.print("le robot s'arrête");
webSocket.sendTXT(0, "le robot s'arrête"); 
digitalWrite(16, LOW);
digitalWrite(15, LOW);
digitalWrite(0, LOW);
digitalWrite(13, LOW);
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);


  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\r\n", t);
    Serial.flush();
    delay(1000);
  }  

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("a");  

  //initialisation des pins
  pinMode(16,OUTPUT); 
  pinMode(0,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(4,OUTPUT); 
  pinMode(5,OUTPUT);
  pinMode(12,INPUT);
  pinMode(LED,OUTPUT);
  digitalWrite(16,LOW);
  digitalWrite(0,LOW);
  digitalWrite(15,LOW);
  digitalWrite(13,LOW);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  if (mdns.begin("espWebSock", WiFi.localIP())) {
    Serial.println("MDNS responder started");
    mdns.addService("http", "tcp", 80);
    mdns.addService("ws", "tcp", 81);
  }
  else {
    Serial.println("MDNS.begin failed");
  }
  Serial.print("Connect to http://espWebSock.local or http://");
  Serial.println(WiFi.localIP());
  
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.onNotFound(handleNotFound);

  //Start server
  server.begin();                  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);  //Lors d'un message venant du websocket, appelle la fonction webSocketEvent
  
  Serial.println("HTTP server started"); 
} 
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  webSocket.loop();
  server.handleClient(); 
}
