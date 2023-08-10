#include <WiFi.h> 
#include "SinricPro.h"
#include "SinricProSwitch.h"
#include <SinricProGarageDoor.h> 
#include<PubSubClient.h>
#include <DHT.h>

#include <map>

// Paramètres du WIFI
const char* WIFI_SSID = "rabie"; 
const char* WIFI_PASS = "123456789";

// Paramètres du Broker MQTT
const char* mqttServer = "192.168.137.244";
const int mqttPort = 1883;

//Thingspeak
const char* serverTh = "api.thingspeak.com";
const char* apiKey = "RKPR1G70NCGDNOGF";

// Senric pro
#define APP_KEY "b7c25375-c9a6-467c-a123-0ac3b6f9e158"                                          
#define APP_SECRET "31c67726-11ee-4047-8887-f51bfd387808-6bd270df-50c4-44c0-9f40-904aa8bf56e8" 

// Paramètres pour l'automatisation
bool previousLampState = false; //Lampe
bool previousClimatState = false; //Climat



//ID dans Senric pro
#define device_ID_1 "647a06cb929949c1da7a198d" //Lampe
#define device_ID_2 "647a3cc1929949c1da7a421f" //Climate
#define device_ID_3 "648727e2743f912070351f08" //Porte


// les portes GPIO
#define RelayPin1 0   //Lampe 
#define RelayPin2 4   //Climate

#define Pin0 17 //Porte
#define Pin1 5 //Porte
#define Pin2 18 //Porte
#define Pin3 19 //Porte

#define RelayPin7 21   //Presence
#define RelayPin8 22   //Temperature

DHT dht(RelayPin8,DHT11);

WiFiClient esclient;
PubSubClient client(esclient);

typedef struct { 
  int relayPIN;
  int flipSwitchPIN;
} deviceConfig_t;

//LLiée chaque id avec un porte GPIO
std::map<String, deviceConfig_t> devices = {
  { device_ID_1, { RelayPin1 } },
  { device_ID_2, { RelayPin2 } },
  { device_ID_3, { 12 } }
};

// fonction pour configurer les porte GPIO en OUTPUT ou bien INPUT
void setupRelays() {
  for (auto &device : devices) {            
    int relayPIN = device.second.relayPIN; 
    pinMode(relayPIN, OUTPUT); 
    digitalWrite(relayPIN, LOW); 
  }
    pinMode(Pin0, OUTPUT);  
    pinMode(Pin1, OUTPUT);  
    pinMode(Pin2, OUTPUT);  
    pinMode(Pin3, OUTPUT); 

  pinMode(RelayPin7, INPUT);   //Presence

}

//Connexion au MQTT
void setupMQTT(){
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      Serial.println("Connecté au Broker MQTT !");
    } else {
      Serial.print("Échec de la connexion au Broker MQTT. Erreur = ");
      Serial.println(client.state());
    }
  }
}

//Connexion au WIFI
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}


// Fonction pour ouvrire ou fermer la porte
void Porte(bool state){
  int _step = 0; 
  if(state){
      for (int i=0; i<8000; i++){
          switch(_step){ 
          case 0: 
            digitalWrite(Pin0, LOW);  
            digitalWrite(Pin1, LOW); 
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, HIGH); 
          break;  
          case 1: 
            digitalWrite(Pin0, LOW);  
            digitalWrite(Pin1, LOW); 
            digitalWrite(Pin2, HIGH); 
            digitalWrite(Pin3, HIGH); 
          break;  
          case 2: 
            digitalWrite(Pin0, LOW);  
            digitalWrite(Pin1, LOW); 
            digitalWrite(Pin2, HIGH); 
            digitalWrite(Pin3, LOW); 
          break;  
          case 3: 
            digitalWrite(Pin0, LOW);  
            digitalWrite(Pin1, HIGH); 
            digitalWrite(Pin2, HIGH); 
            digitalWrite(Pin3, LOW); 
          break;  
          case 4: 
            digitalWrite(Pin0, LOW);  
            digitalWrite(Pin1, HIGH); 
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, LOW); 
          break;  
          case 5: 
            digitalWrite(Pin0, HIGH);  
            digitalWrite(Pin1, HIGH); 
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, LOW); 
          break;  
            case 6: 
            digitalWrite(Pin0, HIGH);  
            digitalWrite(Pin1, LOW); 
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, LOW); 
          break;  
          case 7: 
            digitalWrite(Pin0, HIGH);  
            digitalWrite(Pin1, LOW); 
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, HIGH); 
          break;  
          default: 
            digitalWrite(Pin0, LOW);  
            digitalWrite(Pin1, LOW); 
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, LOW); 
          break;  
          } 
          if(false){ 
          _step++; 
          }else{ 
          _step--; 
          } 
          if(_step>7){ 
          _step=0; 
          } 
          if(_step<0){ 
          _step=7; 
          } 
          delay(1); 
      }
  }else{
      for (int i=0; i<8000; i++){
          switch(_step){ 
          case 0: 
            digitalWrite(Pin0, LOW);  
            digitalWrite(Pin1, LOW); 
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, HIGH); 
          break;  
          case 1: 
            digitalWrite(Pin0, LOW);  
            digitalWrite(Pin1, LOW); 
            digitalWrite(Pin2, HIGH); 
            digitalWrite(Pin3, HIGH); 
          break;  
          case 2: 
            digitalWrite(Pin0, LOW);  
            digitalWrite(Pin1, LOW); 
            digitalWrite(Pin2, HIGH); 
            digitalWrite(Pin3, LOW); 
          break;  
          case 3: 
            digitalWrite(Pin0, LOW);  
            digitalWrite(Pin1, HIGH); 
            digitalWrite(Pin2, HIGH); 
            digitalWrite(Pin3, LOW); 
          break;  
          case 4: 
            digitalWrite(Pin0, LOW);  
            digitalWrite(Pin1, HIGH); 
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, LOW); 
          break;  
          case 5: 
            digitalWrite(Pin0, HIGH);  
            digitalWrite(Pin1, HIGH); 
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, LOW); 
          break;  
            case 6: 
            digitalWrite(Pin0, HIGH);  
            digitalWrite(Pin1, LOW); 
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, LOW); 
          break;  
          case 7: 
            digitalWrite(Pin0, HIGH);  
            digitalWrite(Pin1, LOW); 
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, HIGH); 
          break;  
          default: 
            digitalWrite(Pin0, LOW);  
            digitalWrite(Pin1, LOW); 
            digitalWrite(Pin2, LOW); 
            digitalWrite(Pin3, LOW); 
          break;  
          } 
          if(true){ 
          _step++; 
          }else{ 
          _step--; 
          } 
          if(_step>7){ 
          _step=0; 
          } 
          if(_step<0){ 
          _step=7; 
          } 
          delay(1); 
      }
  }

}

// Fonction qui envoi la temperature vers ThingSpeak
void ServerClient(int temperature){

  if (esclient.connect("api.thingspeak.com", 80)) {
    // Construction de la requête HTTP pour envoyer les données à ThingSpeak
    String postData = "api_key=";
    postData += "RKPR1G70NCGDNOGF";
    postData += "&field1=";
    postData += String(temperature);
    Serial.println(temperature);

    esclient.println("POST /update HTTP/1.1");
    esclient.println("Host: " + String("api.thingspeak.com"));
    esclient.println("Connection: close");
    esclient.println("Content-Type: application/x-www-form-urlencoded");
    esclient.println("Content-Length: " + String(postData.length()));
    esclient.println();
    esclient.println(postData);
    esclient.println();
  } else {
    Serial.println("Erreur lors de la connexion au serveur ThingSpeak !");
  }
}

// Fonction pour allumer la lampe automatiquement si il y a un mouvement
void AlumerLampe() {
  int etatCapteur = digitalRead(RelayPin7);
  bool currentLampState = (etatCapteur == HIGH);

  if (currentLampState != previousLampState) {
    digitalWrite(RelayPin1, currentLampState);
    SinricProSwitch &mySwitch = SinricPro[device_ID_1];
    mySwitch.sendPowerStateEvent(currentLampState);
    previousLampState = currentLampState;
  }
}

// Fonction pour demarer la climatiseur si la temperature est superieur a 32
void AlumerClimate() {

  int temp = dht.readTemperature();
  bool currentClimatState;
    currentClimatState=(temp>=32);
    if (currentClimatState != previousClimatState) {
    digitalWrite(RelayPin2, currentClimatState);
    SinricProSwitch &mySwitch = SinricPro[device_ID_2];
    mySwitch.sendPowerStateEvent(currentClimatState);
    previousClimatState = currentClimatState;
 
    }
    ServerClient(temp);
}

// Fonction pour envoyer un message vers le serveur MQTT
void VERSMQTTP(String deviceId, bool state) {
    int relayPIN = devices[deviceId].relayPIN;
    
  // Vérification si la lecture a réussi
  if (isnan(state) || isnan(relayPIN)) {
    Serial.println("Échec de la lecture !");
    return;
  }

  // Conversion des valeurs en chaînes de caractères
  String relayPINString = String(relayPIN);
  String stateString = state ? "ON" : "OFF";

  // Publication des données sur le Broker MQTT
  String topic = "state";
  String payload = "Le LED " + relayPINString + " est " + stateString;
  client.publish(topic.c_str(), payload.c_str());
  Serial.println("Message envoyer !");
}

// Fonction pour changer l'etat d'un perephirique a distance,
//soit par le site sinric pro ou bien Google Home ou bien Alexa d'Amazone 
bool onPowerState(String deviceId, bool &state) {
    int relayPIN = devices[deviceId].relayPIN;
    if(deviceId == device_ID_3){
      VERSMQTTP(deviceId, state);   
      Porte(state);         
      Serial.printf("Porte : %s\r\n", state ? ": Fermee" : ": Ouvert");
      return true;
    }
    else{
      VERSMQTTP(deviceId, state);
      digitalWrite(relayPIN, state);             // set the new relay state
      Serial.printf("%s: %s\r\n", deviceId.c_str(), state ? ": on" : ": off");
      return true;
    }
}


//fonction qui lit la comande a partir de sinric pro ou un autre 
// assistant et changer l'etat d'un periphirique a l'aide de la fonction onPowerState()
void setupSinricPro() {
  SinricProGarageDoor &myGarageDoor = SinricPro[device_ID_3];
  for (auto &device : devices) {
    const char *deviceId = device.first.c_str();
    if(deviceId != device_ID_3){
      SinricProSwitch &mySwitch = SinricPro[deviceId];
      mySwitch.onPowerState(onPowerState);
    }
  }

  SinricPro.begin(APP_KEY, APP_SECRET);
  SinricPro.restoreDeviceStates(true);
}


//La fonction setup()
void setup() {
  Serial.begin(115200);
  setupRelays();
  setupWiFi();
  setupMQTT();
  dht.begin();
  setupSinricPro();
}

//La fonction loop()
void loop() {

  SinricPro.handle();

  AlumerClimate();
  delay(300);

  AlumerLampe();
}
