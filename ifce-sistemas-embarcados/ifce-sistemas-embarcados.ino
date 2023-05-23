
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>


//CREDENCIAIS DO WIFI
const char* ssid = "Lincoln 2G";
const char* password = "JL09NS28";

//DEFININDO OS PINOS DOS LEDS
const int RED_LED_PIN = 13;
const int WHITE_LED_PIN = 12;
const int BLUE_LED_PIN = 27;

WebServer server(80);

void handleRoot() {
  StaticJsonDocument<200> jsonResponse;
 
  jsonResponse["status"] = "ok";
  jsonResponse["ssid"] = WiFi.SSID();
  jsonResponse["ip"] = WiFi.localIP().toString();

  String jsonResponseString;
  serializeJson(jsonResponse, jsonResponseString);
  
  server.send(200, "application/json", jsonResponseString);
}

void handleTrigger() {

  StaticJsonDocument<1024> jsonRequest;
  String requestBody = server.arg("plain");
    
  deserializeJson(jsonRequest, requestBody);
  
  String trigger = jsonRequest["queryResult"]["parameters"]["trigger"];
  
  Serial.println("trigger value: " + trigger);

  String response = "";

   switch (trigger.toInt()) {
      case 10: {
        digitalWrite(RED_LED_PIN, LOW);
        
        response = "Desligando LED vermelho.";
        
        break;
      }
      case 11:{
        digitalWrite(RED_LED_PIN, HIGH);

        response = "Ligando LED vermelho.";
        
        break;
      }
      case 20:{
        digitalWrite(WHITE_LED_PIN, LOW);

        response = "Desligando LED branco.";
        
        break;
      }
      case 21:{
        digitalWrite(WHITE_LED_PIN, HIGH);
        
        response = "Ligando LED branco.";
        
        break;
      }
      case 30:{
        digitalWrite(BLUE_LED_PIN, LOW);
        
        response = "Desligando LED azul.";
        
        break;
      }
      case 31:{
        digitalWrite(BLUE_LED_PIN, HIGH);

        response = "Ligando LED azul.";
        
        break;
      }
      default:{
      
        response = "Infelizmente, não consegui acionar a carga desejada.";
        
        break;
      }
    }

  StaticJsonDocument<200> jsonResponse;
 
  jsonResponse["payload"]["google"]["expectUserResponse"] = true;
  jsonResponse["payload"]["google"]["richResponse"]["items"][0]["simpleResponse"]["textToSpeech"] = response;

  String jsonResponseString;
  serializeJson(jsonResponse, jsonResponseString);
  
  server.send(200, "application/json", jsonResponseString);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(WHITE_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    
    Serial.println("Conectando à rede WiFi...");
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Conectado à rede WiFi");
  Serial.println("Endereço IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/trigger", HTTP_POST, handleTrigger);

  server.begin();
}

void loop() {
  server.handleClient();
}


//LIBERAR PORTA NO LINUX: sudo chmod a+rw /dev/ttyACM0
//IP DA MINHA REDE: 192.168.100.39
//SERVIDOR PUBLICO: ngrok http://192.168.100.39:80
//MODIFICAR URL DO WEBHOOK NO DIALOGFLOW
