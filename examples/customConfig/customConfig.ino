/**
 **************************************************
 *
 * @file        customConfig.ino
 * @brief       Create a custom config for the LMP91000 and upload it to the device.
 *              This enables you to use this breakout with any electrochemical sensor.
 *
 *              To successfully run the sketch:
 *              - Edit the custom configuration
 *              - Connect LMPEN to any GPIO pin
 *              - Connect the breakout to your Dasduino board via easyC
 *              - Run the sketch and open serial monitor at 115200 baud!
 *
 *              Electrochemical Gas Sensor Breakout: solde.red/333218
 *              Dasduino Core: www.solde.red/333037
 *              Dasduino Connect: www.solde.red/333034
 *              Dasduino ConnectPlus: www.solde.red/333033
 *
 * @authors     Robert @ soldered.com
 ***************************************************/

// Include the required library
#include "Electrochemical-Gas-Sensor-SOLDERED.h"

// For details on how to configure the LMP for any sensor, check sensorConfigData.h
// It has #defines for all available values
// and the LMP91000 datasheet, chapter 7.6
// Here is an example of a custom configuration:
const sensorType SENSOR_CUSTOM = {
    100.0F,                   // nanoAmperesPerPPM
    0,                        // internalZeroCalibration
    ADS_GAIN_4_096V,          // adsGain
    TIA_GAIN_120_KOHM,        // TIA_GAIN_IN_KOHMS
    RLOAD_10_OHM,             // RLOAD
    REF_EXTERNAL,             // REF_SOURCE
    INTERNAL_ZERO_20_PERCENT, // INTERNAL_ZERO
    BIAS_SIGN_NEGATIVE,       // BIAS_SIGN
    BIAS_0_PERCENT,           // BIAS
    FET_SHORT_DISABLED,       // FET_SHORT
    OP_MODE_3LEAD_AMP_CELL,   // OP_MODE
};

// Create the sensor object with the custom type
ElectrochemicalGasSensor sensor(SENSOR_CUSTOM);

void setup()
{
    Serial.begin(115200); // For debugging

    // Init the breakout
    if (!sensor.begin())
    {
        // Can't init? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't init the sensor! Check connections!");
        while (true)
            delay(100);
    }

    Serial.println("Sensor initialized successfully!");
}

void loop()
{
    // Make the reading
    double reading = sensor.getPPM();
    
    // If you're working with a different type of sensor, you will likely want to know the voltage to fine-tune the sensor
    double voltage = sensor.getVoltage();
    // In an environment with no target gas, this should be really close to REF_VOLTAGE * internalZeroPercent
    // Use internalZeroCalibration to fine tune it to that

    // Print the reading with 5 digits of precision
    Serial.println("Sensor reading:");
    Serial.print(reading, 5);
    Serial.println(" PPM");
    Serial.print(voltage, 5); // Also print the voltage
    Serial.println(" V");

    // Wait a bit before reading again
    delay(2500);
}