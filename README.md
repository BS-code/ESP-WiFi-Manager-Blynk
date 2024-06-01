# ESP-WiFi-Manager-Blynk
Wifi Manager input SSID, PASS, TOKEN, SERVER

![Screenshot 2024-06-01 201254](https://github.com/BS-code/ESP-WiFi-Manager-Blynk/assets/43749883/242944b7-a948-4521-8eac-b1455e19e356)
Password = "password"

 // Automatically connect using saved credentials or start configuration portal
  if (!wm.autoConnect("AutoConnectAP", "password")) {
    Serial.println("Failed to connect");
    ESP.restart();
    delay(1000);
  }

![Screenshot 2024-06-01 200742](https://github.com/BS-code/ESP-WiFi-Manager-Blynk/assets/43749883/f91475bf-2c2e-44e8-aa7a-1fd6367e962d)

![Screenshot 2024-06-01 200817](https://github.com/BS-code/ESP-WiFi-Manager-Blynk/assets/43749883/4feae532-acbd-4fb1-abae-a5f63c4f2135)


