#ifndef ADS_BRIDGE_H
#define ADS_BRIDGE_H

#include <Arduino.h>
#include "SoftI2C.h"

// Mirrors src/libs/ADS1X15/ADS1X15.cpp's CONFIG-register construction and
// single-shot channel-0 read exactly (same bit masks), over the local
// SoftI2C bus. Only what ElectrochemicalGasSensor actually uses is
// implemented: single-ended channel 0, single-shot mode, comparator
// disabled - matching today's ads->readADC(0) usage.
#define ADS1X15_I2C_ADDRESS (0x49)

class AdsBridge
{
  public:
    AdsBridge(SoftI2C *bus);

    void setGain(uint8_t gainCode);     // same ADS_GAIN_* values as sensorConfigData.h: 0,1,2,4,8,16
    void setDataRate(uint8_t dataRate); // 0..7, 0 = slowest (clamped like ADS1X15::setDataRate)

    void triggerConversion(); // starts a single-shot conversion on channel 0
    bool isBusy();
    int16_t readValue(); // raw signed conversion result, call once isBusy() == false

  private:
    SoftI2C *_bus;
    uint16_t _gainMask;
    uint16_t _dataRateMask;

    void writeConfigRegister(uint16_t config);
    uint16_t readConfigRegister();
};

#endif
