#ifndef LMP_BRIDGE_H
#define LMP_BRIDGE_H

#include <Arduino.h>
#include "SoftI2C.h"

// Mirrors src/libs/LMP91000/LMP91000.cpp's register writes exactly, but over
// the local SoftI2C bus instead of the hardware Wire object (which the
// ATtiny needs free for the host-facing slave role).
#define LMP91000_I2C_ADDRESS (0x48)
#define LMP91000_STATUS_REG  (0x00)
#define LMP91000_LOCK_REG    (0x01)
#define LMP91000_TIACN_REG   (0x10)
#define LMP91000_REFCN_REG   (0x11)
#define LMP91000_MODECN_REG  (0x12)

#define LMP91000_WRITE_LOCK   (0x01)
#define LMP91000_WRITE_UNLOCK (0x00)
#define LMP91000_READY        (0x01)

class LmpBridge
{
  public:
    LmpBridge(SoftI2C *bus);
    bool configure(uint8_t tiacn, uint8_t refcn, uint8_t modecn);

  private:
    SoftI2C *_bus;
    void writeReg(uint8_t reg, uint8_t data);
    uint8_t readReg(uint8_t reg);
    uint8_t status();
    void unlock();
    void lock();
};

#endif
