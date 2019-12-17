## Encoding on the Feather

```cpp
  pinstates |= digital5;
  pinstates |= digital6 << 1;
  pinstates |= digital12 << 2;
  pinstates |= digital13 << 3;
  
  data[0] = pinstates;
  data[1] = battery;
```

## TTN Payload Decoder
The first byte in the payload holds our pin states. The encoding above put `digital5` into `pinstates` first bit so need to be masked off to retrieve that value.

```javascript
function Decoder(bytes, port) {
  var decoded = {};
  
  // Mask unwanted bits in byte
  decoded.pin5 = (bytes[0] & 1) > 0 ? "HIGH" : "LOW";
  decoded.pin6 = (bytes[0] & 2) > 0 ? "HIGH" : "LOW";
  decoded.pin12 = (bytes[0] & 4) > 0 ? "HIGH" : "LOW";
  decoded.pin13 =  (bytes[0] & 8) > 0 ? "HIGH" : "LOW";
  // Battery charge
  decoded.bat = bytes[1];   

  return decoded;
}
```