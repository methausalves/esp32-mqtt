/*
      Receptor.ino

      Comunicação entre dois dispositivos via MQTT
      Código para o dispositivo receptor, sendo que o mesmo está subscrevendo em mais de um tópico

      Referência: https://forum.arduino.cc/t/subscribe-and-store-value-into-a-variable-mqtt/1045522

*/

// INCLUSÃO DE BIBLIOTECAS
#include <WiFi.h>
#include <PubSubClient.h>

// PARÂMETROS PARA CONEXÃO WIFI
const char* ssid = " NOME DA REDE ";
const char* password = " SENHA ";

// PARÂMETROS PARA CONEXÃO MQTT
const char* mqtt_server = "broker.hivemq.com";

// INSTANCIANDO OBJETOS
WiFiClient espClient;
PubSubClient MQTT(espClient);  // cliente MQTT passando o objeto espClient

// DEFININDO VARIÁVEIS AUXILIARES FUNÇÃO CALLBACK
volatile bool msg_recebida = false;
const int payloadSize = 100;
char mqtt_payload[payloadSize] = { '\0' };
String mqtt_topic;

// FUNÇÃO CALLBACK RETORNO DE CHAMADA MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  if (!msg_recebida) {
    memset(mqtt_payload, '\0', payloadSize);  // clear payload char buffer
    mqtt_topic = "";                          // clear topic string buffer
    mqtt_topic = topic;                       // store new topic
    memcpy(mqtt_payload, payload, length);
    msg_recebida = true;
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  MQTT.setServer(mqtt_server, 1883);
  MQTT.setCallback(callback);
}

void loop() {

  if (msg_recebida) {

    String mensagem;
    mensagem = mqtt_payload;  // armazenando payload recebido como String

    if (mqtt_topic == "/topico1/test") {  // tópico 1

      if (mensagem == "A") {

        Serial.print("Mensagem recebida! Tópico: ");
        Serial.print(mqtt_topic);
        Serial.print(" - Mensagem: ");
        Serial.print(mensagem);
        Serial.println();

      }
      if (mensagem == "B") {

        Serial.print("Mensagem recebida! Tópico: ");
        Serial.print(mqtt_topic);
        Serial.print(" - Mensagem: ");
        Serial.print(mensagem);
        Serial.println();
      }
    }

    if (mqtt_topic == "/topico2/test") {  // tópico 2

      if (mensagem == "A") {

        Serial.print("Mensagem recebida! Tópico: ");
        Serial.print(mqtt_topic);
        Serial.print(" - Mensagem: ");
        Serial.print(mensagem);
        Serial.println();

      }
      if (mensagem == "B") {

        Serial.print("Mensagem recebida! Tópico: ");
        Serial.print(mqtt_topic);
        Serial.print(" - Mensagem: ");
        Serial.print(mensagem);
        Serial.println();

      }
    }

    msg_recebida = false;

  }

  if (!MQTT.connected()) {
    reconnect();
  }

  MQTT.loop();
}

// FUNÇÃO PARA CONEXÃO WIFI
void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Conectando à rede: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// FUNÇÃO PARA CONEXÃO E RECONEXÃO MQTT
void reconnect() {
  // Loop until we're reconnected
  while (!MQTT.connected()) {
    Serial.println("Aguardando conexão MQTT...");
    // Create a random client ID
    String clientId = "CLIENT_MQTT";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (MQTT.connect(clientId.c_str())) {
      Serial.println("Conexão MQTT realizada!\n");
      // Subscrevendo tópicos
      MQTT.subscribe("/topico1/test");
      MQTT.subscribe("/topico2/test");

    } else {
      Serial.println("falhou, rc=");
      Serial.print(MQTT.state());
      Serial.println("Tentando novamente em 5 segundos...");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}