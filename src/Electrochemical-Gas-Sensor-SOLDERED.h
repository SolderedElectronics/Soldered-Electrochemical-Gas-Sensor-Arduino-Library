/**
 **************************************************
 *
 * @file        Electrochemical-Gas-Sensor-SOLDERED.h
 * @brief       Header file for sensor specific code.
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     Robert @ soldered.com
 ***************************************************/

#ifndef __ELECTROCHEMICAL_GAS_SENSOR_SOLDERED__
#define __ELECTROCHEMICAL_GAS_SENSOR_SOLDERED__

#include "Arduino.h"
#include "libs/ADS1X15/ADS1X15.h"
#include "libs/LMP91000/LMP91000.h"
#include "sensorConfigData.h"

#define DEFAULT_LMP_ADDR 0x48
#define DEFAULT_ADC_ADDR 0x49

class ElectrochemicalGasSensor
{
  public:
    ElectrochemicalGasSensor(sensorType _t, uint8_t _adcAddr = DEFAULT_ADC_ADDR);
    bool begin();
    bool configureLMP();
    double getPPM();

  private:
    LMP91000 *lmp;
    ADS1015 *ads;
    uint8_t adcAddr;
    sensorType type;
};

#endif
