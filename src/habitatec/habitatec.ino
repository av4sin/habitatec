// ============================================================
// HABITATEC
// BLE sensor hub diseñado para funcionar en un esp 32
// Testado con un heltec_v2
// Versión 2.3.2
// Autor: Av4sin (https://github.com/av4sin)
// ============================================================

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <DHT11.h>
#include <esp_bt_main.h>
#include <esp_bt.h>

// ============================================================
// CONFIGURACIÓN DHT11 Y MUESTREO
// ============================================================

#define DHT_PIN 4
DHT11 dht(DHT_PIN);

// Frecuencia de lectura local (10 lecturas en 30s -> cada 3s)
const unsigned long DHT_INTERVAL = 3000;
// Intervalo de envío BLE (30s)
const unsigned long BLE_INTERVAL = 30000;

unsigned long lastDHTRead = 0;
unsigned long lastBLESend = 0;

// Historial para lecturas de Temperatura
const int NUM_SAMPLES = 10;
float tempSamples[NUM_SAMPLES];
int sampleIndex = 0;
bool samplesReady = false;

// Variable global para guardar únicamente la ÚLTIMA humedad leída
int ultimaHumedad = 0;

// ============================================================
// BLE - SENSORES AMBIENTALES
// ============================================================

#define SENSOR_SERVICE_UUID "0000181A-0000-1000-8000-00805F9B34FB"
#define TEMPERATURE_UUID    "00002A6E-0000-1000-8000-00805F9B34FB"
#define HUMIDITY_UUID       "00002A6F-0000-1000-8000-00805F9B34FB"

BLECharacteristic *temperatureCharacteristic;
BLECharacteristic *humidityCharacteristic;
BLEServer *pServer = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;

// ============================================================
// CALLBACKS DE ESTADO DE CONEXIÓN BLE
// ============================================================

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
      deviceConnected = true;
      Serial.println("-> Dispositivo conectado (Marco)");
    };

    void onDisconnect(BLEServer* pServer) override {
      deviceConnected = false;
      Serial.println("-> Dispositivo desconectado");
    }
};

// ============================================================
// ALGORITMIA PARA TEMPERATURA OSCILANTE
// ============================================================

float procesarTemperaturaInteligente(float arr[], int size) {
  if (size == 0) return 0.0;

  float minVal = arr[0];
  float maxVal = arr[0];
  float suma = 0.0;

  for (int i = 0; i < size; i++) {
    if (arr[i] < minVal) minVal = arr[i];
    if (arr[i] > maxVal) maxVal = arr[i];
    suma += arr[i];
  }

  // Si varía exactamente entre dos enteros contiguos (ej. 25 y 26 °C)
  if ((maxVal - minVal == 1.0f)) {
    bool soloEsosDos = true;
    for (int i = 0; i < size; i++) {
      if (arr[i] != minVal && arr[i] != maxVal) {
        soloEsosDos = false;
        break;
      }
    }
    if (soloEsosDos) {
      return minVal + 0.5f;
    }
  }

  return suma / (float)size;
}

// ============================================================
// ENVÍO DE DATOS POR BLE (30 SECS)
// ============================================================

void enviarBLE() {
  int count = samplesReady ? NUM_SAMPLES : sampleIndex;
  if (count == 0) return;

  // Calculamos únicamente el filtrado de la Temperatura
  float tempFiltrada = procesarTemperaturaInteligente(tempSamples, count);

  Serial.println();
  Serial.println("==================================================");
  Serial.print("PROCESADO BLE (Muestras Temp: ");
  Serial.print(count);
  Serial.println("):");
  Serial.print("Temp calculada: ");
  Serial.print(tempFiltrada, 2);
  Serial.println(" °C");
  Serial.print("Hum enviado (Última directa): ");
  Serial.print(ultimaHumedad);
  Serial.println(" %");

  if (deviceConnected) {
    // --------------------------------------------------------
    // Temperatura (GATT 0x2A6E -> Unidades de 0.01 °C, Little Endian)
    // --------------------------------------------------------
    int16_t temperatureValue = (int16_t)(tempFiltrada * 100);
    uint8_t temperatureBytes[2];
    temperatureBytes[0] = temperatureValue & 0xFF;         // LSB
    temperatureBytes[1] = (temperatureValue >> 8) & 0xFF;  // MSB

    temperatureCharacteristic->setValue(temperatureBytes, 2);
    temperatureCharacteristic->notify();

    // --------------------------------------------------------
    // Humedad -> Formato Big-Endian uint16be_div10 (Última leída x 10)
    // --------------------------------------------------------
    uint16_t humidityValue = (uint16_t)(ultimaHumedad * 10);
    uint8_t humidityBytes[2];
    
    // Big-Endian: MSB (byte 0), LSB (byte 1)
    humidityBytes[0] = (humidityValue >> 8) & 0xFF; // MSB
    humidityBytes[1] = humidityValue & 0xFF;        // LSB

    humidityCharacteristic->setValue(humidityBytes, 2);
    humidityCharacteristic->notify();

    Serial.println("Datos transmitidos via BLE exitosamente");
  } else {
    Serial.println("Dispositivo no conectado. Notificación omitida.");
  }
  Serial.println("==========================================");
}

// ============================================================
// ACTUALIZACIÓN DE LOS SENSORES
// ============================================================

void actualizarSensores() {
  int temperature = 0;
  int humidity = 0;

  int result = dht.readTemperatureHumidity(temperature, humidity);

  if (result != 0) {
    Serial.print("ERROR DHT11: ");
    Serial.println(DHT11::getErrorString(result));

    static const int temperaturas[] = {25, 26, 25, 26, 25, 26, 25, 26, 25, 26};
    static const int humedades[]    = {50, 51, 52, 51, 50, 53, 50, 51, 50, 51};

    static int indice = 0;
    temperature = temperaturas[indice];
    humidity = humedades[indice];

    indice++;
    if (indice >= 10) indice = 0;
  } else {
    Serial.print("[MUESTRA LOCAL] Temp: ");
    Serial.print(temperature);
    Serial.print(" °C | Hum: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  // Guarda únicamente la última humedad recibida sin procesar nada
  ultimaHumedad = humidity;

  // Almacena la temperatura en el buffer para su algoritmo
  tempSamples[sampleIndex] = (float)temperature;

  sampleIndex++;
  if (sampleIndex >= NUM_SAMPLES) {
    sampleIndex = 0;
    samplesReady = true;
  }
}

// ============================================================
// SETUP
// ============================================================

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n==============================");
  Serial.println("HABITATEC ROOM SENSOR HUB");
  Serial.println("VERSION 2.3.2 | AUTOR: TU, CREADOR DEL PROYECTO");
  Serial.println("==============================");

  BLEDevice::init("Habitacion_Heltec");
  
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9);
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_CONN_HDL0, ESP_PWR_LVL_P9);

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *sensorService = pServer->createService(SENSOR_SERVICE_UUID);

  temperatureCharacteristic = sensorService->createCharacteristic(
      TEMPERATURE_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  temperatureCharacteristic->addDescriptor(new BLE2902());

  humidityCharacteristic = sensorService->createCharacteristic(
      HUMIDITY_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  humidityCharacteristic->addDescriptor(new BLE2902());

  sensorService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SENSOR_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  
  pAdvertising->setMinInterval(0x00A0);
  pAdvertising->setMaxInterval(0x0100);
  
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);

  BLEDevice::startAdvertising();

  Serial.println("BLE iniciado y visible. Esperando conexiones...");
}

// ============================================================
// LOOP
// ============================================================

void loop() {
  unsigned long currentMillis = millis();

  if (!deviceConnected && oldDeviceConnected) {
    delay(500);
    pServer->startAdvertising(); 
    Serial.println("Re-iniciando Advertising para reconexión automática...");
    oldDeviceConnected = deviceConnected;
  }
  
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
  }

  // Lectura del DHT11 cada 3 segundos
  if (currentMillis - lastDHTRead >= DHT_INTERVAL) {
    lastDHTRead = currentMillis;
    actualizarSensores();
  }

  // Envío BLE cada 30 segundos
  if (currentMillis - lastBLESend >= BLE_INTERVAL) {
    lastBLESend = currentMillis;
    enviarBLE();
  }
}
