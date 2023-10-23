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
ElectrochemicalGasSensor::ElectrochemicalGasSensor(sensorType _t, uint8_t _adcAddr, int _configPin)
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
    ads->setGain(type.adsGain); // Set gain to the one which is in the config
    ads->setDataRate(0);        // Set data rate to slowest for more precision

    // Begin the config pin if it's set
    if (configPin != -1)
    {
        pinMode(configPin, OUTPUT);
        digitalWrite(configPin, HIGH); // Disable LMP config at first
    }

    // Now, configure the LMP analog frontend as well:
    result &= configureLMP();

    // Will return 1 if both ads->begin and configureLMP() were OK
    return result;
}

/**
 * @brief                   Configure the LMP91000, has to be done at startup
 *
 * @returns                 True if it was successful, false if it failed
 *
 */
bool ElectrochemicalGasSensor::configureLMP()
{
    // If there is a pin set for configuring, pull it LOW
    if (configPin != -1)
        digitalWrite(configPin, LOW);

    // Crate the values to write in the sensor to configure it
    // tiacn register
    uint8_t tiacn = 0x00;
    tiacn |= (type.TIA_GAIN_IN_KOHMS << 2);
    tiacn |= type.RLOAD;

    // refcn register
    uint8_t refcn = 0x00;
    refcn |= (type.REF_SOURCE << 7);
    refcn |= (type.INTERNAL_ZERO << 5);
    refcn |= (type.BIAS_SIGN << 4);
    refcn |= type.BIAS;

    // modecn register
    uint8_t modecn = 0x00;
    modecn |= (type.FET_SHORT << 7);
    modecn |= type.OP_MODE;

    // Configure it!
    uint8_t res = lmp->configure(tiacn, refcn, modecn);

    // Save key variables in the class as well so we don't have to keep getting them:
    tiaGainInKOHms = getTiaGain();
    internalZeroPercent = getInternalZeroPercent();

    // Disable config again
    if (configPin != -1)
        digitalWrite(configPin, HIGH);

    // Notify the user if the configuration went well or not
    return res;
}

/**
 * @brief                   get the voltage which the ADS is currently measuring
 *
 * @returns                 double value of the voltage in volts
 *
 */
double ElectrochemicalGasSensor::getVoltage()
{
    // Get raw reading and calculate voltage
    int16_t rawReading = ads->readADC(0);
    double voltage = ads->toVoltage(rawReading);
    return voltage;
}


/**
 * @brief                   Make a measurement with the ADC and calculate the PPM value of the measured gas
 *
 * @returns                 double value of the PPM
 *
 */
double ElectrochemicalGasSensor::getPPM()
{
    // Get the voltage from the ADS
    double voltage = getVoltage();

    Serial.println();
    Serial.print("Volts: ");
    Serial.println(voltage, 10);

    // Calculate current and calculate PPM based on datasheet
    double voltsNoRef = voltage - (REF_VOLTAGE * (internalZeroPercent / 100.0F));
    Serial.print("Volts no ref: ");
    Serial.println(voltsNoRef, 10);
    voltsNoRef += type.internalZeroCalibration; // Add the calibration value as well
    Serial.print("Volts no ref after calib: ");
    Serial.println(voltsNoRef, 10);

    double current = voltsNoRef / (tiaGainInKOHms * 1000);
    double ppm = current / (type.nanoAmperesPerPPM * (double)1e-9);

    // Due to noise when making really small precise measurements (in ppb)
    // ppm can sometimes go into negative due to noise - just round it to zero
    if (ppm < 0)
        ppm = 0;
    return ppm;
}

/**
 * @brief                   Calcualte PPB values from PPM
 *
 * @returns                 double value of PPB
 *
 */
double ElectrochemicalGasSensor::getPPB()
{
    return getPPM() * 1000.0;
}

/**
 * @brief                               Calculate an averaged PPM measurement from a number of separate measurements
 *
 * @note                                This is a blocking function, it won't exit until it's completed
 *
 * @param uint8_t _numMeasurements     How many measurements to do
 *
 * @param uint8_t _secondsDelay         How many seconds to wait between each measurement
 *
 * @returns                 double value of the calculated averaged PPM
 *
 */
double ElectrochemicalGasSensor::getAveragedPPM(uint8_t _numMeasurements, uint8_t _secondsDelay)
{
    double totalMeasurements = 0;

    // Sum all the separate measurements
    for (int i = 0; i < _numMeasurements; i++)
    {
        // Make the individual measurement
        totalMeasurements += getPPM();
        // Wait for the set number of seconds
        delay(1000 * _secondsDelay);
    }

    // Return the final value
    return totalMeasurements / _numMeasurements;
}

/**
 * @brief                               Calculate an averaged PPB measurement from a number of separate measurements
 *
 * @note                                This is a blocking function, it won't exit until it's completed
 *
 * @param uint8_t _numMeasurements     How many measurements to do
 *
 * @param uint8_t _secondsDelay         How many seconds to wait between each measurement
 *
 * @returns                 double value of the calculated averaged PPB
 *
 */
double ElectrochemicalGasSensor::getAveragedPPB(uint8_t _numMeasurements, uint8_t _secondsDelay)
{
    return getAveragedPPM(_numMeasurements, _secondsDelay) * 1000;
}

/**
 * @brief                              Get the actual number of the kOhms in the TIA gain
 *
 * @note                                Only used internally in the class
 *
 * @returns                 float of the TIA gain depending on the config
 *
 */
float ElectrochemicalGasSensor::getTiaGain()
{
    switch (type.TIA_GAIN_IN_KOHMS)
    {
    case TIA_GAIN_EXTERNAL:
        // For this, use setCustomTiaGain
        return -1;
        break;
    case TIA_GAIN_2_75_KOHM:
        return 2750.00F;
        break;
    case TIA_GAIN_3_5_KOHM:
        return 3500.00F;
        break;
    case TIA_GAIN_7_KOHM:
        return 7000.00F;
        break;
    case TIA_GAIN_14_KOHM:
        return 14000.00F;
        break;
    case TIA_GAIN_35_KOHM:
        return 35000.00F;
        break;
    case TIA_GAIN_120_KOHM:
        return 120000.00F;
        break;
    case TIA_GAIN_350_KOHM:
        return 350000.00F;
        break;
    default:
        return -1;
        break;
    }
}

/**
 * @brief           Set a custom number of the kOhms in the TIA gain
 *
 * @note            This is if you're using an external resistor
 *
 * @returns         None
 *
 */
void ElectrochemicalGasSensor::setCustomTiaGain(float _tiaGain)
{
    tiaGainInKOHms = _tiaGain;
}

/**
 * @brief           Get the internal zero % setting as an actual float
 *
 * @returns         The internal zero as a float
 *
 */
float ElectrochemicalGasSensor::getInternalZeroPercent()
{
    switch (type.INTERNAL_ZERO)
    {
    case INTERNAL_ZERO_20_PERCENT:
        return 20.00F;
        break;
    case INTERNAL_ZERO_50_PERCENT:
        return 50.00F;
        break;
    case INTERNAL_ZERO_67_PERCENT:
        return 67.00F;
        break;
    case INTERNAL_ZERO_BYPASSED:
        // This is not implemented, just the standard internal zero mode
        return -1;
        break;
    }
}
