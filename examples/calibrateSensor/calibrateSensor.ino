/**
 **************************************************
 *
 * @file        calibrateSensor.ino
 * @brief       See how to calibrate an electrochemical sensor
 *
 *              To successfully run the sketch:
 *              - Uncomment the #define in the library header file
 *              - Connect the breakout to your Dasduino board via easyC
 *              - Connect LMPEN pin to GND or a GPIO pin so the breakout can be configured
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

// This is the value used to calibrate the sensor, described below
const double internalZeroCalibration = 0.078;

// Let's make a copy of the CO sensor type:
const sensorType SENSOR_CO_CALIBRATED = {
    70.0F,                    // nanoAmperesPerPPM
    internalZeroCalibration,  // internalZeroCalibration
    ADS_GAIN_4_096V,          // adsGain
    TIA_GAIN_350_KOHM,        // TIA_GAIN_IN_KOHMS
    RLOAD_10_OHM,             // RLOAD
    REF_EXTERNAL,             // REF_SOURCE
    INTERNAL_ZERO_20_PERCENT, // INTERNAL_ZERO
    BIAS_SIGN_NEGATIVE,       // BIAS_SIGN
    BIAS_0_PERCENT,           // BIAS
    FET_SHORT_DISABLED,       // FET_SHORT
    OP_MODE_3LEAD_AMP_CELL,   // OP_MODE
};

// Create the sensor object with the according type
ElectrochemicalGasSensor sensor(SENSOR_SO2, 0x4B, 32);

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
    // This function will also print debug messages if the #define in
    // the header file of the library is uncommented
    // It will print:
    // - The raw voltage measurement
    // - Voltage without reference value
    // - Voltage after calibration

    // In an environment with 0 of the target gas, the voltage after calibration should be
    // roughly 0, adjust internalZeroCalibration to get it to that value.

    // Print the reading with 5 digits of precision
    Serial.print("Sensor reading: ");
    Serial.print(reading, 5);
    Serial.println(" PPM");

    // Wait a bit before reading again
    delay(2500);
}