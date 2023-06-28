/**
 **************************************************
 *
 * @file        customAddress.ino
 * @brief       See how using different I2C addresses works for the Electrochemical Gas Sensor Breakout.
 *              This allows you to connect multiple Electrochemical Gas Sensor Breakouts with one Dasduino!
 *
 *              To successfully run the sketch:
 *              -Set your breakout's address with the jumpers on the board (info below)
 *              -Connect the breakout(s) to your Dasduino board via easyC
 *              -Run the sketch and open serial monitor at 115200 baud!
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

/**
 * The board has three jumpers which determine the address:
 * 
 * JP5 is connected by default, it makes the address 0x49. It has to be connected because
 * both the LMP91000 and ADS1015 have the default address 0x48.
 * 
 * Disconnecting JP5 and connecting JP6 makes the address 0x4A.
 * 
 * Disconnecting JP5 and connecting JP7 makes the address 0x4B.
 * 
*/

// If you are using a custom I2C address, create the object like this:
ElectrochemicalGasSensor sensor(SENSOR_SO2, 0x4A);


void setup()
{
    Serial.begin(115200); // For debugging

    // Init the sensor
    if (!sensor.begin())
    {
        // Can't init? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't init the sensor! Check connections!");
        while (true)
        {
            delay(100);
        }
    }

    Serial.println("Sensor initialized successfully!");
}

void loop()
{
    // Make the reading
    float reading = sensor.getPPM();
    
    // Print the reading with 10 digits of precision
    Serial.print("Sensor reading: ");
    Serial.print(reading, 10);
    Serial.println("ppm");
    
    // Wait a bit before reading again
    delay(5000);
}