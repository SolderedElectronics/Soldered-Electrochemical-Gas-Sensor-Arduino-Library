#include "AdsBridge.h"

#define ADS1X15_REG_CONVERT 0x00
#define ADS1X15_REG_CONFIG  0x01

#define ADS_OS_START_SINGLE 0x8000
#define ADS_OS_NOT_BUSY      0x8000
#define ADS_READ_0           0x4000 // single-ended channel 0
#define ADS_MODE_SINGLE      0x0100

// Fixed comparator defaults matching ADS1X15::reset(): compPol=ACTIVE_HIGH (0x0008),
// compQueConvert=DISABLE (0x0003). compMode/compLatch default to 0, no bits needed.
#define ADS_COMPARATOR_DEFAULTS 0x000B

AdsBridge::AdsBridge(SoftI2C *bus) : _bus(bus), _gainMask(0x0000), _dataRateMask(0x0080 /* rate 4, default */)
{
}

void AdsBridge::setGain(uint8_t gainCode)
{
    switch (gainCode)
    {
    default:
    case 0:
        _gainMask = 0x0000; // 6.144V
        break;
    case 1:
        _gainMask = 0x0200; // 4.096V
        break;
    case 2:
        _gainMask = 0x0400; // 2.048V
        break;
    case 4:
        _gainMask = 0x0600; // 1.024V
        break;
    case 8:
        _gainMask = 0x0800; // 0.512V
        break;
    case 16:
        _gainMask = 0x0A00; // 0.256V
        break;
    }
}

void AdsBridge::setDataRate(uint8_t dataRate)
{
    if (dataRate > 7)
        dataRate = 4;
    _dataRateMask = ((uint16_t)dataRate) << 5;
}

void AdsBridge::writeConfigRegister(uint16_t config)
{
    _bus->beginTransmission(ADS1X15_I2C_ADDRESS);
    _bus->write(ADS1X15_REG_CONFIG);
    _bus->write((uint8_t)(config >> 8));
    _bus->write((uint8_t)(config & 0xFF));
    _bus->endTransmission();
}

uint16_t AdsBridge::readConfigRegister()
{
    _bus->beginTransmission(ADS1X15_I2C_ADDRESS);
    _bus->write(ADS1X15_REG_CONFIG);
    _bus->endTransmission();

    uint8_t buf[2] = {0, 0};
    _bus->requestFrom(ADS1X15_I2C_ADDRESS, buf, 2);
    return ((uint16_t)buf[0] << 8) | buf[1];
}

void AdsBridge::triggerConversion()
{
    uint16_t config =
        ADS_OS_START_SINGLE | ADS_READ_0 | _gainMask | ADS_MODE_SINGLE | _dataRateMask | ADS_COMPARATOR_DEFAULTS;
    writeConfigRegister(config);
}

bool AdsBridge::isBusy()
{
    uint16_t val = readConfigRegister();
    return (val & ADS_OS_NOT_BUSY) == 0;
}

int16_t AdsBridge::readValue()
{
    _bus->beginTransmission(ADS1X15_I2C_ADDRESS);
    _bus->write(ADS1X15_REG_CONVERT);
    _bus->endTransmission();

    uint8_t buf[2] = {0, 0};
    _bus->requestFrom(ADS1X15_I2C_ADDRESS, buf, 2);
    return (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);
}
