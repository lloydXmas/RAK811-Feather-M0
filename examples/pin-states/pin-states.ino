/*
An example of reading pin states and battery voltage.

Sketch assumes Serial2 on D10/D11 is being used. Comment-out
SERCOM1 lines and use Serial1 if using defualt UART pins.
*/
#include <Arduino.h>
#include <wiring_private.h>

// Create Serial2 from SERCOM pins 11 & 10
Uart Serial2(&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
void SERCOM1_Handler() { Serial2.IrqHandler(); }

void setup() {
  Serial.begin(115200);   // USB Serial line
  Serial2.begin(115200);  // LoRa Serial line

  // Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(10, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);

  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
}

void loop() {
  uint8_t digital5, digital6, digital12, digital13;
  uint8_t pinstates;
  uint8_t battery;
  float measuredvbat;

  // Read fethaer's battery voltage on pin A7
  measuredvbat = analogRead(A7);
  measuredvbat *= 2;
  measuredvbat *= 3.3;   // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024;  // convert to voltage

  // Convert voltage to percent
  battery = roundf(measuredvbat * 10) / 10;
  battery = round(measuredvbat * 10);
  battery = map(battery, 30, 43, 0, 100);
  Serial.print(F("Bat SoC: "));
  Serial.println(battery);

  // Read pin states of D5, D6, D12, D13
  digital5 = digitalRead(5);
  digital6 = digitalRead(6);
  digital12 = digitalRead(12);
  digital13 = digitalRead(13);

  // Add pin readings to buffer
  pinstates |= digital5;
  pinstates |= digital6 << 1;
  pinstates |= digital12 << 2;
  pinstates |= digital13 << 3;

  // Create packet with the formatted data
  uint8_t data[1];
  data[0] = pinstates;
  data[1] = battery;

  // Send packet
  sendPayload(data, sizeof(data));

  delay(15000);
}

// Formats payload to hex & sends it
void sendPayload(uint8_t* payload, size_t length) {
  uint8_t i = 0;
  Serial2.print("at+send=lora:2:");
  Serial.print(F("Payload: "));
  for (i = 0; i < length; i++) {
    if (payload[i] < 16) {
      Serial2.print("0");
      Serial2.print(payload[i], HEX);
      Serial.print(F("0"));
      Serial.print(payload[i], HEX);
    } else {
      Serial2.print(payload[i], HEX);
      Serial.print(payload[i], HEX);
    }
  }
  Serial.println("");
  Serial2.write("\r\n");
  delay(10);
}