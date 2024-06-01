

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "xxxxxx"
#define BLYNK_TEMPLATE_NAME "poject3"

#include <ESP8266WiFi.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <BlynkSimpleEsp8266.h>
#include <FS.h> // Include the SPIFFS library
#include <ArduinoJson.h> // Include the ArduinoJSON library

// Define your custom parameters (Blynk token and server)
char blynk_token[33] = "";
char blynk_server[40] = "blynk-cloud.com";

// WiFiManager and parameters
WiFiManager wm;
WiFiManagerParameter custom_blynk_token("blynk", "Blynk Token", blynk_token, 32);
WiFiManagerParameter custom_blynk_server("server", "Blynk Server", blynk_server, 39);
WiFiManagerParameter custom_wifi_ssid("ssid", "WiFi SSID", "", 32);
WiFiManagerParameter custom_wifi_password("password", "WiFi Password", "", 64);

void setup() {
  Serial.begin(115200);
  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  // Read configuration from SPIFFS
  if (!loadConfig()) {
    Serial.println("Failed to load config");
  }

  // Add custom parameters to WiFiManager
  wm.addParameter(&custom_blynk_token);
  wm.addParameter(&custom_blynk_server);
  wm.addParameter(&custom_wifi_ssid);
  wm.addParameter(&custom_wifi_password);

  // Set custom HTML elements to get the WiFi credentials
  wm.setSaveConfigCallback(saveConfigCallback);

  // Automatically connect using saved credentials or start configuration portal
  if (!wm.autoConnect("AutoConnectAP", "password")) {
    Serial.println("Failed to connect");
    ESP.restart();
    delay(1000);
  }

  // Save custom parameters
  strcpy(blynk_token, custom_blynk_token.getValue());
  strcpy(blynk_server, custom_blynk_server.getValue());
  String custom_ssid = wm.getWiFiSSID();
  String custom_password = wm.getWiFiPass();

  // Save configuration to SPIFFS
  saveConfig();

  // Connect to Blynk using the parameters from the configuration portal
  WiFi.begin(custom_ssid.c_str(), custom_password.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Blynk.config(blynk_token, blynk_server);
  Blynk.connect();

  Serial.println("Connected to Blynk server");
}

void loop() {
  Blynk.run();
}

void saveConfigCallback() {
  Serial.println("Configuration saved");
  saveConfig();  // Save configuration to SPIFFS
}

// Function to load configuration from SPIFFS
bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  DynamicJsonDocument json(1024); // Gunakan DynamicJsonDocument dari ArduinoJSON
  DeserializationError error = deserializeJson(json, buf.get());

  if (error) {
    Serial.println("Failed to parse config file");
    return false;
  }

  strcpy(blynk_token, json["blynk_token"]);
  strcpy(blynk_server, json["blynk_server"]);
  return true;
}

// Function to save configuration to SPIFFS
void saveConfig() {
  DynamicJsonDocument json(1024); // Gunakan DynamicJsonDocument dari ArduinoJSON
  json["blynk_token"] = blynk_token;
  json["blynk_server"] = blynk_server;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
  }

  serializeJson(json, configFile);
}
