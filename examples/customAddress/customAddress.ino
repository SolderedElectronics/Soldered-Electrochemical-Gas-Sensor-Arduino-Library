/**
 **************************************************
 *
 * @file        customAddress.ino
 * @brief       See how to init a sensor on a custom address
 *
 *              To successfully run the sketch:
 *              - Set the custom address using the jumpers on the back of the board
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

// Create the sensor object with the according type
// To set a custom address, connect one of the following jumpers:
// JP5 for 0x49
// JP6 for 0x4A
// JP7 for 0x4B
// The final parameter is the GPIO pin for LMPEN
ElectrochemicalGasSensor sensor(SENSOR_SO2, 0x4B, 5);

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

    // If PPB is more relevant for your sensor, you can use:
    // double reading = sensor.getPPB();

    // Print the reading with 5 digits of precision
    Serial.print("Sensor reading:");
    Serial.print(reading, 5);
    Serial.println(" PPM");

    // Wait a bit before reading again
    delay(2500);
}