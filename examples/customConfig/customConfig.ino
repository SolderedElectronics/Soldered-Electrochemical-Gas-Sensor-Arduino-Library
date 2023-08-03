/**
 **************************************************
 *
 * @file        customConfig.ino
 * @brief       Create a custom config for the LMP91000 and upload it to the device.
 *              This enables you to use this breakout with any electrochemical sensor.
 *
 *              NOTE: For the LMP91000 to be accessible via I2C, you must connect LMPEN to GND!
 *
 *              To successfully run the sketch:
 *              - Connect LMPEN to GND
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

/**
 * Here are parameters to configure for the custom sensor:
 * 
 * TIA_GAIN: The gain of the amplifier, 3 bits
 * 000              External resistance (default)
 * 001              2.75 kOhm
 * 010              3.5k kOhm
 * 011              7 kOhm
 * 100              14 kOhm
 * 101              35 kOhm
 * 110              120 kOhm
 * 111              350 kOhm
 * 
 * RLOAD: The load resistor, 2 bits
 * 00               10 Ohm
 * 01               33 Ohm
 * 10               50 Ohm
 * 11               100 Ohm (default)
 * 
 * REF_SOURCE: Source of the control voltage reference, 1 bit
 * 0                Internal (default)
 * 1                External
 * 
 * INT_Z: Internal zero selection, 2 bits
 * 00               20%
 * 01               50% (default)
 * 10               67%
 * 11               Internal zero bypassed
 * 
 * BIAS_SIGN: The polarity of the bias voltage, 1 bit
 * 0                Netagive (default)
 * 1                Positive
 * 
 * BIAS: Percentage of the source reference, 4 bits
 * 0000             0% (default)
 * 0001             1%
 * 0010             2%
 * 0011             4%
 * 0100             6%
 * ...
 * 1100             22%
 * 1101             24%
 * 
 * FET_SHORT: Short the FET, 1 bit
 * 0                Disabled (default)
 * 1                Enabled
 * 
 * OP_MODE: Mode of operation, 3 bits
 * 000              Deep Sleep (default)
 * 001              2-lead ground referred galvanic cell
 * 010              Standby
 * 011              3-lead amperometric cell
 * 110              Temperature measurement (TIA Off)
 * 111              Temperature measurement (TIA On)
 * 
 * Here's how these values are written in the parameters:
 * tiacn: [000][TIA_GAIN][RLOAD]
 * refcn: [REF_SOURCE][INT_Z][BIAS_SIGN][BIAS]
 * modecn: [FET_SHORT][0000][OP_MODE]
 * Lastly, set the nanoAmperesPerPPM value and the tiaGainInKOhms.
 * 
 * Here is an example for a sensor configured with the following parameters:
 * TIA_GAIN: 120kOhm, RLOAD: 10 Ohm, REF_SOURCE: external,
 * INT_Z: 50%, BIAS_SIGN: negative, BIAS: 0%,
 * FET_SHORT: disabled, OP_MODE: 3-lead amperometric cell,
 * nanoAmperesPerPPM: 75, tiaGainInKOhms: 120 (as set before)
 */
const sensorType SENSOR_CUSTOM = {0b00011000, 0x10100000, 0x00000011, 75.0F, 120};

// Create the sensor object with the custom type
ElectrochemicalGasSensor sensor(SENSOR_CUSTOM);

// If you are using a custom I2C address, create the object like this:
// ElectrochemicalGasSensor sensor(SENSOR_CUSTOM, 0x4A);
// For more info, see customAddress.ino

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

    // Configure LMP91000
    if (!sensor.configureLMP())
    {
        // Error? Notify the user and go to infinite loop
        Serial.println("ERROR: Can't configure LMP91000!");
        while (true)
        {
            delay(100);
        }
    }
    Serial.println("Sensor configured successfully!");
}

void loop()
{
    // Make the reading
    double reading = sensor.getPPM();
    
    // Print the reading with 10 digits of precision
    Serial.print("Sensor reading: ");
    Serial.print(reading, 10);
    Serial.println("ppm");
    
    // Wait a bit before reading again
    delay(5000);
}