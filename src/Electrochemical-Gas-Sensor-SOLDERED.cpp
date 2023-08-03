/**
 * **************************************************
 *
 * @file        Generic-easyC-SOLDERED.cpp
 * @brief       Example functions to overload in base class.
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     @ soldered.com
 ***************************************************/

#include "Electrochemical-Gas-Sensor-SOLDERED.h"

/**
 * @brief                   Constructor on custom address
 *
 * @param sensorType _t     The type of the sensor
 *
 * @param uint8_t _adcAddr  The custom address of the ADC
 *
 */
ElectrochemicalGasSensor::ElectrochemicalGasSensor(sensorType _t, uint8_t _adcAddr, uint8_t _configPin)
{
    adcAddr = _adcAddr;
    type = _t;
    configPin = _configPin;
}

/**
 * @brief                   Init the sensor and begin measuring with the ADC, must be called before using
 *
 * @returns                 True if it was successful, false if it failed
 *
 */
bool ElectrochemicalGasSensor::begin()
{
    // Init twoWire communication
    Wire.begin();

    // Create objects in memory
    lmp = (LMP91000 *)malloc(sizeof(LMP91000));
    lmp = new LMP91000();

    ads = (ADS1015 *)malloc(sizeof(ADS1015));
    ads = new ADS1015(adcAddr);

    // Begin ADS
    bool result = ads->begin();
    ads->setGain(1); // Set gain to 4.096V

    // Begin the config pin if it's set
    if (configPin != -1)
    {
        pinMode(configPin, OUTPUT);
        digitalWrite(configPin, HIGH); // Disable LMP config
    }

    return result;
}

/**
 * @brief                   Configure the LMP91000, only has to be done once, see configLMP.ino for more details
 *
 * @returns                 True if it was successful, false if it failed
 *
 */
bool ElectrochemicalGasSensor::configureLMP()
{
    // If there is a pin set for configuring, pull it LOW
    if (configPin != -1)
        digitalWrite(configPin, LOW);

    // The data used for configuring is in sensorConfigData.h
    uint8_t res = lmp->configure(type.tiacn, type.refcn, type.modecn);

    // Disable config again
    if (configPin != -1)
        digitalWrite(configPin, HIGH);

    // Notify the user if the configuration went well or not
    if (res == 1)
        return true;
    else
        return false;
}

/**
 * @brief                   Make a measurement with the ADC and calculate the PPM value of the measured gas
 *
 * @returns                 double value of the PPM
 *
 */
double ElectrochemicalGasSensor::getPPM()
{
    // Get raw reading and calculate voltage
    int16_t rawReading = ads->readADC(0);
    double voltage = ads->toVoltage(rawReading);

    // Calculate current and calculate PPM based on datasheet
    double current = voltage / (type.tiaGainInKOhms * 1000); // Divide by TIA gain
    double ppm = current / (type.nanoAmperesPerPPM * (double)1e-9);
    return ppm;
}
