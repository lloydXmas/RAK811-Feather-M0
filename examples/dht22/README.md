## Encoding on the Feather

```cpp
  uint8_t data[3];
  data[0] = highByte(t);
  data[1] = lowByte(t);
  data[2] = highByte(h);
  data[3] = lowByte(h);
```

## TTN Payload Decoder
Allows for negative temeprature values
```javascript
function Decoder(bytes, port) {
  var decoded = {};
  
  // Decode bytes to int
  var tempC = (bytes[0] & 0x80 ? 0xFFFF<<16 : 0) | bytes[0]<<8 | bytes[1];
  var h = (bytes[2] << 8) | bytes[3];
  
  decoded.temp = tempC / 100;
  decoded.humidity = h / 100;
  
  return decoded;
}
```