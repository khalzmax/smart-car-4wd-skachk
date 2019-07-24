#include <Arduino.h>

int readSerial()
{
    char buffer[16];
  // if we get a command, turn the LED on or off:
  if (Serial.available() > 0) {
    int size = Serial.readBytesUntil('\n', buffer, 12);
    if (buffer[0] == 'Y') {
      // digitalWrite(LED_BUILTIN, HIGH);
    }
    if (buffer[0] == 'N') {
      // digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
