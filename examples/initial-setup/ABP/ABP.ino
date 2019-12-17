// Initial setup for ABP mode of RAK811 module.

#include <Arduino.h>
#include <wiring_private.h>

// Create Serial2 from SERCOM pins 11 & 10
Uart Serial2(&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
void SERCOM1_Handler() { Serial2.IrqHandler(); }

// Region: EU868, US915
String region = "US915";

// ABP Parameters: Device Address, Network Session Key, App Session Key
String devAddr = "00000000";
String nwksKey = "00000000000000000000000000000000";
String appsKey = "00000000000000000000000000000000";

void setup() {
  Serial.begin(115200);   // USB Serial line
  Serial2.begin(115200);  // LoRa Serial line

  // Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(10, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);
}

void loop() {
  if (Serial) {
    // Set region
    Serial.println(F("Setting Region"));
    setConfig("region", region);
    delay(1000);

    // Set Join Mode to ABP
    Serial.println(F("Setting Join-Mode to ABP"));
    setConfig("join_mode", "1");
    delay(1000);

    // Set as Class A device
    Serial.println(F("Setting Device Type to Class A"));
    setConfig("class", "0");
    delay(1000);

    // ABP parameters
    Serial.println(F("Hardcoding Device Address"));
    setConfig("dev_addr", devAddr);
    delay(500);
    Serial.println(F("Hardcoding NwkSKey"));
    setConfig("nwks_key", nwksKey);
    delay(500);
    Serial.println(F("Hardcoding AppSKey"));
    setConfig("apps_key", appsKey);
    delay(500);

    sendCmd("at+join");
    delay(1000);
    Serial.println(F("Setup complete!"));
    Serial.println(F("If you see any ERROR messages, check your keys"));
    while (1) {
      delay(1);
    }
  }
}

void sendCmd(String cmd) {
  String reply;
  Serial2.print(cmd);
  Serial2.write("\r\n");
  while (Serial2.available()) {
    char c = Serial2.read();
    reply += c;
  }
  Serial.println(reply);
  delay(500);
}

void setConfig(String key, String value) {
  sendCmd("at+set_config=lora:" + key + ":" + value);
}