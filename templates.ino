#include <EEPROM.h>
#include <Arduino.h>  // for type definitions

// =========== EEPROM Write Anything ===========
// http://playground.arduino.cc/Code/EEPROMWriteAnything
template <class T> int EEPROM_write(int ee, const T& value) {
  const byte* p = (const byte*)(const void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    EEPROM.write(ee++, *p++);
  return i;
}

template <class T> int EEPROM_read(int ee, T& value) {
  byte* p = (byte*)(void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
  return i;
}


// =========== Array size ===========
template< typename T, size_t N > size_t ArraySize (T (&) [N]){ return N; }
