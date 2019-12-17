/*
DHT22 sensor is connected to pin 5 of the Feather.

formatFloat() rounds temperature and humidity values to nearest
hundreth & multiplies by 100 to fit nicely into buffer.
Notem that temperature is a signed integer as it may be negative.

Sketch assumes Serial2 on D10/D11 is being used. Comment-out
SERCOM1 lines and use Serial1 if using defualt UART pins.
*/
#include <Arduino.h>
#include <DHT.h>
#include <wiring_private.h>

// DHT definition & instance
#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Create Serial2 from SERCOM pins 11 & 10
Uart Serial2(&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
void SERCOM1_Handler() { Serial2.IrqHandler(); }

void setup() {
  Serial.begin(115200);   // USB Serial line
  Serial2.begin(115200);  // LoRa Serial line

  dht.begin();

  // Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(10, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  Serial.print(F("Humidity: "));
  Serial.println(hum);
  Serial.print(F("Temperature: "));
  Serial.println(temp);

  // Format floats for packet
  int16_t t = formatFloat(temp);
  uint16_t h = formatFloat(hum);

  // Create packet with dht data
  uint8_t data[3];
  data[0] = highByte(t);
  data[1] = lowByte(t);
  data[2] = highByte(h);
  data[3] = lowByte(h);

  // Send packet
  sendPayload(data, sizeof(data));

  delay(15000);
}

// Returns a float rounded to nearest 100th & multiplied by 100
float formatFloat(float input) {
  input = roundf(input * 10) / 10;
  input = round(input * 100);
  return input;
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