#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include <ESPAsyncWebSrv.h>

#define ledR D2
#define ledG D3

int t;
int h;
int l;
int g;
int lg;
int ly;
int lr;

int temp;
int hum;
int lux;
int grn;
int yel;
int gas;

int red;

const char* ssid = "wc";
const char* password = "8B]\"u.=S{e&2%<!!qG93C8E.[.h^?,-<g+]KfqA/F{-~LeZ:5ga966<rb\\b:'mE";

AsyncWebServer server(80); // Измените на ESPAsyncWebServer

String ipToString(IPAddress ip) {
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

unsigned long lastRequestTime = 0; // Переменная для отслеживания времени последнего запроса

const unsigned long requestInterval = 10000;//1200000; // 20 минут в миллисекундах

void handle_OnConnect(AsyncWebServerRequest *request) {
  JSONVar sensorReadings;
  sensorReadings["IP"] = ipToString(WiFi.localIP());

  temp = t;
  hum = h;
  lux = l;
  gas = g;
  
  grn = lg;
  yel = ly;
  red = lr;

  sensorReadings["Sensor1"] = temp;
  sensorReadings["Sensor2"] = hum;
  sensorReadings["Sensor3"] = lux;
  sensorReadings["Sensor4"] = gas;
  
  sensorReadings["Sensor5"] = grn;
  sensorReadings["Sensor6"] = yel;
  sensorReadings["Sensor7"] = red;

  String jsonStr = JSON.stringify(sensorReadings);
  request->send(200, "application/json", jsonStr);

  // Обновляем время последнего запроса
  lastRequestTime = millis();
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handle_OnConnect); // Измените на ESPAsyncWebServer
  server.begin();
  Serial.println("HTTP server started");

  pinMode(ledR, OUTPUT); // Установите пин для красного светодиода как выход
  pinMode(ledG, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');
    char *token = strtok((char *)receivedData.c_str(), "_");
    if (token != NULL) {
      t = atoi(token);

      token = strtok(NULL, "_");

      h = atoi(token);

      token = strtok(NULL, "_");

      l = atoi(token);

      token = strtok(NULL, "_");

      g = atoi(token);

      token = strtok(NULL, "_");
      
      lg = atoi(token);

      token = strtok(NULL, "_");

      ly = atoi(token);

      token = strtok(NULL, "_");

      lr = atoi(token);

      // Обновляем время последнего запроса
      lastRequestTime = millis();
    }
  }

  // Проверяем, прошло ли 20 минут с последнего запроса
  if (millis() - lastRequestTime >= requestInterval) {
    // Если да, включаем красный светодиод
    digitalWrite(ledG, LOW);
    digitalWrite(ledR, HIGH);
  } else {
    // В противном случае, включаем зеленый светодиод и выключаем красный
    digitalWrite(ledG, HIGH);
    digitalWrite(ledR, LOW);
  }
}
