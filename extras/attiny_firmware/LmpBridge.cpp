#include "LmpBridge.h"

LmpBridge::LmpBridge(SoftI2C *bus) : _bus(bus)
{
}

void LmpBridge::writeReg(uint8_t reg, uint8_t data)
{
    _bus->beginTransmission(LMP91000_I2C_ADDRESS);
    _bus->write(reg);
    _bus->write(data);
    _bus->endTransmission();
}

uint8_t LmpBridge::readReg(uint8_t reg)
{
    _bus->beginTransmission(LMP91000_I2C_ADDRESS);
    _bus->write(reg);
    _bus->endTransmission();

    uint8_t value = 0;
    _bus->requestFrom(LMP91000_I2C_ADDRESS, &value, 1);
    return value;
}

uint8_t LmpBridge::status()
{
    return readReg(LMP91000_STATUS_REG);
}

void LmpBridge::unlock()
{
    writeReg(LMP91000_LOCK_REG, LMP91000_WRITE_UNLOCK);
}

void LmpBridge::lock()
{
    writeReg(LMP91000_LOCK_REG, LMP91000_WRITE_LOCK);
}

bool LmpBridge::configure(uint8_t tiacn, uint8_t refcn, uint8_t modecn)
{
    if (status() != LMP91000_READY)
        return false;

    unlock();
    writeReg(LMP91000_TIACN_REG, tiacn);
    writeReg(LMP91000_REFCN_REG, refcn);
    writeReg(LMP91000_MODECN_REG, modecn);
    lock();
    return true;
}
