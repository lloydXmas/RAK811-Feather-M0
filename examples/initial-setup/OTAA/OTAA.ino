
// Initial setup for OTAA mode of RAK811 module.

#include <Arduino.h>
#include <wiring_private.h>

// Create Serial2 from SERCOM pins 11 & 10
Uart Serial2(&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
void SERCOM1_Handler() { Serial2.IrqHandler(); }

// Region: EU868, US915
String region = "US915";

// OTAA Parameters: Device EUI, Application EUI, App Key
String devEui = "00000000";
String appEui = "00000000000000000000000000000000";
String appKey = "00000000000000000000000000000000";

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

    // Set Join Mode to OTAA
    Serial.println(F("Setting Join-Mode to OTAA"));
    setConfig("join_mode", "0");
    delay(1000);

    // Set as Class A device
    Serial.println(F("Setting Device Type to Class A"));
    setConfig("class", "0");
    delay(1000);

    // OTAA parameters
    Serial.println(F("Setting OTAA Keys"));
    setConfig("dev_eui", devEui);
    setConfig("app_eui", appEui);
    setConfig("app_key", appKey);

    // Validate OTAA parameters
    sendCmd("at+join");
    delay(1000);

    Serial.println(F("Setup complete!"));
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
  Serial.print(reply);
  Serial.println(F(""));
  delay(500);
}

void setConfig(String key, String value) {
  sendCmd("at+set_config=lora:" + key + ":" + value);
}