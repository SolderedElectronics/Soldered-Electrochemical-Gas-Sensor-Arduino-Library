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

// This define will cause debug messages to print in our library
// Useful when calibrating the sensor
//#define ELECTROCHEMICAL_SENSOR_DEBUG

#include "Arduino.h"
#include "libs/ADS1X15/ADS1X15.h"
#include "libs/LMP91000/LMP91000.h"
#include "sensorConfigData.h"

#define DEFAULT_LMP_ADDR 0x48
#define DEFAULT_ADC_ADDR 0x49
#define REF_VOLTAGE      2.5F

class ElectrochemicalGasSensor
{
  public:
    ElectrochemicalGasSensor(sensorType _t, uint8_t _adcAddr = DEFAULT_ADC_ADDR, int _configPin = -1);
    bool begin();
    bool configureLMP();
    double getVoltage();
    double getPPM();
    double getPPB();
    double getAveragedPPM(uint8_t _numMeasurements = 5, uint8_t _secondsDelay = 2);
    double getAveragedPPB(uint8_t _numMeasurements = 5, uint8_t _secondsDelay = 2);
    void setCustomTiaGain(float _tiaGain);
    void setCustomZeroCalibration(double calibration);

  private:
    LMP91000 *lmp;
    ADS1015 *ads;
    uint8_t adcAddr;
    int configPin;
    sensorType type;
    float tiaGainInKOHms;
    float internalZeroPercent;
    float getTiaGain();
    float getInternalZeroPercent();
};

#endif
