#ifndef SOFT_I2C_H
#define SOFT_I2C_H

#include <Arduino.h>

// Bit-banged I2C master, used for the ATtiny's local bus to the onboard
// LMP91000 + ADS chip. Uses only pinMode()/digitalWrite()/digitalRead() -
// no direct port-register access - so it stays portable across cores instead
// of depending on classic AVR DDRx/PORTx/PINx registers that don't exist in
// that form on tinyAVR 0/1-series parts.
//
// The ATtiny404 has exactly one hardware TWI peripheral, reserved for the
// host-facing slave role (Wire, timing-critical). This bit-banged bus is
// master-only and self-paced, so software timing is acceptable here.
class SoftI2C
{
  public:
    SoftI2C(uint8_t sdaPin, uint8_t sclPin);
    void begin();

    bool beginTransmission(uint8_t address); // returns ACK of the address byte
    bool write(uint8_t data);                // returns ACK of the data byte
    void endTransmission();

    // Combined restart+address+read+NACK-last-byte sequence, mirroring Wire.requestFrom().
    bool requestFrom(uint8_t address, uint8_t *buffer, uint8_t length);

  private:
    uint8_t _sda;
    uint8_t _scl;

    void sdaHigh();
    void sdaLow();
    void sclHigh();
    void sclLow();
    void start();
    void stop();
    bool writeByte(uint8_t data);
    uint8_t readByte(bool ack);
};

#endif
