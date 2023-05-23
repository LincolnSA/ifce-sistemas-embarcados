
# IFCE Sistemas Embarcados

# O projeto
    - Um assistente virtual em desenvolvimento para a disciplina de sistemas embarcados no qual pode-se executar comandos através da conversação utilizando processamento de linguagem natural. Para acionar uma carga é muito simples, você pode acionar com o seguinte comando “Ligar Led Vermelho” ou “Desligar Led Vermelho”.

### Dialogflow
    - Criar uma conta na plataforma do Dialogflow;
    - Criar um agente conversacional;
    - Integrar com Google Assistant;
    - Acessar o zip disponível ir em export and import no Dialogflow e restore from zip.

### Console Actions
    - Criar um novo projeto;
    - Link ao Dialogflow.

### Código
    - Copiar/colar o código e baixar no seu microcontrolador
``` arduino

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>


//CREDENCIAIS DO WIFI
const char* ssid = ""; //Nome da sua rede
const char* password = ""; //Senha da sua rede

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

```

### Executar
 - Ao realizar todos os passos anteriores;
 - executar o código do microcontrolador, em sua serial apresentará o ip necessário para export seu servidor local;
 - Para torná-lo público é necessário instalar o ngrok e executar o seguinte comando (ngrok http http://IP_INFORMADO_NA_SERIAL:80);
 - Com a url pública com https, ir em fullfiment no Dialogflow, adicionar e salvar;
 - A conexão estará ok (Agente e microcontrolador);
 
