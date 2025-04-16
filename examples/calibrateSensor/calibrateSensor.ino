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

// This is the custom value we set for the calibration voltage
// This value is added to the voltage read from the ADC
// In an environment with 0 of the target gas, the voltage after calibration should be
// roughly 0, adjust customCalibration to get it to that value.
const double customCalibration = 0.05;


// Create the sensor object with the according type
ElectrochemicalGasSensor sensor(SENSOR_NH3, 0x49, 32);



void setup()
{
    Serial.begin(115200); // For debugging

    // Initialize the breakout
    if (!sensor.begin())
    {
        // Can't init? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't init the sensor! Check connections!");
        while (true)
            delay(100);
    }

    //Set the custom calibration value
    sensor.setCustomZeroCalibration(customCalibration);

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
    // These values can help you adjust the internal zero calibration

    

    // Print the reading with 5 digits of precision
    Serial.print("Sensor reading: ");
    Serial.print(reading, 5);
    Serial.println(" PPM");

    // Wait a bit before reading again
    delay(2500);
}