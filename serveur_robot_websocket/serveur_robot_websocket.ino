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
int lum;
int x;
//SSID and Password of your WiFi router
static const char ssid[] = "CRI-";//"CRI-MAKERLAB";
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
const char avance[] = "avance";
const char halte[] = "halte";
const char droite[] = "droite";
const char gauche[] = "gauche";
const char recule[] = "recule";


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
        // Send the current LED status
    /*    if (movSTATUS) {
          Serial.println("movSTATUS");
          webSocket.sendTXT(num, avance, strlen(avance));
        }
        else {
          webSocket.sendTXT(num, halte, strlen(halte));
          Serial.println("movSTATUS else");
        }  */
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\r\n", num, payload);

      if (strcmp(avance, (const char *)payload) == 0) {
       // writeLED (true);      
        A();
      }
      else if (strcmp(halte, (const char *)payload) == 0) {
      //  writeLED(false);     
         S();  
      }
      else if (strcmp(droite, (const char *)payload) == 0) {  
         D();  
      }      
      else if (strcmp(gauche, (const char *)payload) == 0) {
        G();
      }
      else if (strcmp(recule, (const char *)payload) == 0) {  
         R();  
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
/*
void handleADC() {
 int a = digitalRead(12);
 String adcValue = String(a);
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}  */

void A() {
Serial.print("A"); 
webSocket.sendTXT(0, "A");     
          digitalWrite(16, HIGH);
          digitalWrite(15, HIGH);
          digitalWrite(0, LOW);
          digitalWrite(13, LOW); 
}

void R() {
Serial.print("R");
webSocket.sendTXT(0, "R"); 
          digitalWrite(16, LOW);
          digitalWrite(15, LOW);
          digitalWrite(0, HIGH);
          digitalWrite(13, HIGH); 
}

void G() {
Serial.print("G");
webSocket.sendTXT(0, "G"); 
          digitalWrite(16, HIGH);
          digitalWrite(15, LOW);
          digitalWrite(0, LOW);
          digitalWrite(13, HIGH);
}

void D() {
Serial.print("D");
webSocket.sendTXT(0, "D"); 
digitalWrite(16, LOW);
digitalWrite(15, HIGH);
digitalWrite(0, HIGH);
digitalWrite(13, LOW);
}

void S() {
Serial.print("S");
webSocket.sendTXT(0, "S"); 
digitalWrite(16, LOW);
digitalWrite(15, LOW);
digitalWrite(0, LOW);
digitalWrite(13, LOW);
}

void handleLED2() {
if (lum==0) {
  lum = 1;
  digitalWrite(LED,HIGH);
}
else {
  lum = 0;
  digitalWrite(LED,LOW);
}
}

/*
static void writeLED(bool avance)
{
  movSTATUS = avance;
  // Note inverted logic for Adafruit HUZZAH board
  if (avance) {
          digitalWrite(16, HIGH);
          digitalWrite(15, HIGH);
          digitalWrite(0, LOW);
          digitalWrite(13, LOW); 
  }
  else {
          digitalWrite(16, LOW);
          digitalWrite(15, LOW);
          digitalWrite(0, LOW);
          digitalWrite(13, LOW); 
  }
}
*/
//==============================================================
//                  SETUP
void setup(void){
  Serial.begin(115200);


  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\r\n", t);
    Serial.flush();
    delay(1000);
  }  

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("a");  

  //Onboard LED port Direction output
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
  /*
  server.on("/LED", handleLED2); 
  server.on("/avance", A);
  server.on("/recule", R);
  server.on("/gauche", G);
  server.on("/droite", D);
  server.on("/halte", S);
  server.on("/readADC", handleADC); //This page is called by java Script AJAX  
*/
  server.onNotFound(handleNotFound);

  
  server.begin();                  //Start server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
  Serial.println("HTTP server started"); 
} 
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  webSocket.loop();
  
  server.handleClient(); 

 /*  if (Serial.available() > 0) {
    char inByte = Serial.read();
    //Serial.println(inByte);
    //Ping
    if (inByte == '?') {
      Serial.println("?");
    }

    else if (inByte == 'a') {
      Serial.println("a");
      handleLED2();
    }
   }
*/

}
