/**
 **************************************************
 *
 * @file        sensorConfigData.h
 * @brief       This file contains all the configurations for the sensors
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     Robert @ soldered.com
 ***************************************************/

#ifndef __ELECTROCHEMICAL_GAS_SENSOR_CONFIG_DATA_SOLDERED__
#define __ELECTROCHEMICAL_GAS_SENSOR_CONFIG_DATA_SOLDERED__

// Defines for configuring the analog frontend
// For more info, please refer to the official datasheet of the LMP91000

#define TIA_GAIN_EXTERNAL  0x00
#define TIA_GAIN_2_75_KOHM 0x01
#define TIA_GAIN_3_5_KOHM  0x02
#define TIA_GAIN_7_KOHM    0x03
#define TIA_GAIN_14_KOHM   0x04
#define TIA_GAIN_35_KOHM   0x05
#define TIA_GAIN_120_KOHM  0x06
#define TIA_GAIN_350_KOHM  0x07

#define RLOAD_10_OHM  0x00
#define RLOAD_33_OHM  0x01
#define RLOAD_50_OHM  0x02
#define RLOAD_100_OHM 0x03

#define REF_INTERNAL 0x00
#define REF_EXTERNAL 0x01

#define INTERNAL_ZERO_20_PERCENT 0x00
#define INTERNAL_ZERO_50_PERCENT 0x01
#define INTERNAL_ZERO_67_PERCENT 0x02
#define INTERNAL_ZERO_BYPASSED   0x03

#define BIAS_SIGN_NEGATIVE 0x00
#define BIAS_SIGN_POSITIVE 0x01

#define BIAS_0_PERCENT  0x00
#define BIAS_1_PERCENT  0x01
#define BIAS_2_PERCENT  0x02
#define BIAS_4_PERCENT  0x03
#define BIAS_6_PERCENT  0x04
#define BIAS_8_PERCENT  0x05
#define BIAS_10_PERCENT 0x06
#define BIAS_12_PERCENT 0x07
#define BIAS_14_PERCENT 0x08
#define BIAS_16_PERCENT 0x09
#define BIAS_18_PERCENT 0x0A
#define BIAS_20_PERCENT 0x0B
#define BIAS_22_PERCENT 0x0C
#define BIAS_24_PERCENT 0x0D

#define FET_SHORT_DISABLED 0x00
#define FET_SHORT_ENABLED  0x01

#define OP_MODE_DEEP_SLEEP          0x00
#define OP_MODE_2LEAD_GROUND_CELL   0x01
#define OP_MODE_STANDBY             0x02
#define OP_MODE_3LEAD_AMP_CELL      0x03
#define OP_MODE_TEMPERATURE_TIA_OFF 0x06
#define OP_MODE_TEMPERATURE_TIA_ON  0x07

// Defines for settings the ADS gain
#define ADS_GAIN_6_144V 0x0000
#define ADS_GAIN_4_096V 0x0200
#define ADS_GAIN_2_048V 0x0400
#define ADS_GAIN_1_024V 0x0600
#define ADS_GAIN_0_512V 0x0800
#define ADS_GAIN_0_256V 0x0A00

// The struct which holds the sensor type and is used in init'ing the breakout
// Also, the data from here is used to calcualte PPM
// Refer to the official LMP91000 datasheet, chapter 7.6 for more info
// You can add your own or configure an existing one if nescessary

struct sensorType
{
    double nanoAmperesPerPPM;
    double internalZeroCalibration;
    uint16_t adsGain;
    uint8_t TIA_GAIN_IN_KOHMS;
    uint8_t RLOAD;
    uint8_t REF_SOURCE;
    uint8_t INTERNAL_ZERO;
    uint8_t BIAS_SIGN;
    uint8_t BIAS;
    uint8_t FET_SHORT;
    uint8_t OP_MODE;
};

// NOTE: The reference voltage is always 2.5V
// The internal zero voltage has an offset in the PPM calculation - internalZeroCalibration
// This is due to the fact high gains amplify the noise as well so it's best to further calibrate it

// SGX-4CO - Carbon Monoxide sensor
const sensorType SENSOR_CO = {
    70.0F,                    // nanoAmperesPerPPM
    0.078,                    // internalZeroCalibration
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

// SGX-4NO2 - Nitrogen Dioxide sensor
const sensorType SENSOR_NO2 = {
    -600.0F,                  // nanoAmperesPerPPM
    -0.015,                   // internalZeroCalibration
    ADS_GAIN_2_048V,          // adsGain
    TIA_GAIN_350_KOHM,        // TIA_GAIN_IN_KOHMS
    RLOAD_10_OHM,             // RLOAD
    REF_EXTERNAL,             // REF_SOURCE
    INTERNAL_ZERO_67_PERCENT, // INTERNAL_ZERO
    BIAS_SIGN_NEGATIVE,       // BIAS_SIGN
    BIAS_0_PERCENT,           // BIAS
    FET_SHORT_DISABLED,       // FET_SHORT
    OP_MODE_3LEAD_AMP_CELL,   // OP_MODE
};

// SGX-4SO2 - Sulphur Dioxide sensor
const sensorType SENSOR_SO2 = {
    400.0F,                   // nanoAmperesPerPPM
    0.1,                      // internalZeroCalibration
    ADS_GAIN_2_048V,          // adsGain
    TIA_GAIN_120_KOHM,        // TIA_GAIN_IN_KOHMS
    RLOAD_10_OHM,             // RLOAD
    REF_EXTERNAL,             // REF_SOURCE
    INTERNAL_ZERO_20_PERCENT, // INTERNAL_ZERO
    BIAS_SIGN_POSITIVE,       // BIAS_SIGN
    BIAS_0_PERCENT,           // BIAS
    FET_SHORT_DISABLED,       // FET_SHORT
    OP_MODE_3LEAD_AMP_CELL,   // OP_MODE
};

// SGX-403-20 - Ozone sensor
const sensorType SENSOR_O3 = {
    -1000.0F,                  // nanoAmperesPerPPM
    -1.65,                      // internalZeroCalibration
    ADS_GAIN_2_048V,          // adsGain
    TIA_GAIN_120_KOHM,        // TIA_GAIN_IN_KOHMS
    RLOAD_10_OHM,            // RLOAD
    REF_EXTERNAL,             // REF_SOURCE
    INTERNAL_ZERO_20_PERCENT, // INTERNAL_ZERO
    BIAS_SIGN_POSITIVE,       // BIAS_SIGN
    BIAS_0_PERCENT,           // BIAS
    FET_SHORT_DISABLED,       // FET_SHORT
    OP_MODE_3LEAD_AMP_CELL,   // OP_MODE
};

// SGX-4NO-250 - Nitric Oxide sensor
const sensorType SENSOR_NO = {
    400.0F,                  // nanoAmperesPerPPM
    0.5,                      // internalZeroCalibration
    ADS_GAIN_2_048V,          // adsGain
    TIA_GAIN_120_KOHM,        // TIA_GAIN_IN_KOHMS
    RLOAD_10_OHM,            // RLOAD
    REF_EXTERNAL,             // REF_SOURCE
    INTERNAL_ZERO_20_PERCENT, // INTERNAL_ZERO
    BIAS_SIGN_POSITIVE,       // BIAS_SIGN
    BIAS_12_PERCENT,           // BIAS
    FET_SHORT_DISABLED,       // FET_SHORT
    OP_MODE_3LEAD_AMP_CELL,   // OP_MODE
};

// SGX-4H2S-100 - Hydrogen Sulphide sensor
const sensorType SENSOR_H2S = {
    1200.0F,                  // nanoAmperesPerPPM
    0.1,                      // internalZeroCalibration
    ADS_GAIN_2_048V,          // adsGain
    TIA_GAIN_350_KOHM,        // TIA_GAIN_IN_KOHMS
    RLOAD_10_OHM,            // RLOAD
    REF_EXTERNAL,             // REF_SOURCE
    INTERNAL_ZERO_20_PERCENT, // INTERNAL_ZERO
    BIAS_SIGN_POSITIVE,       // BIAS_SIGN
    BIAS_0_PERCENT,           // BIAS
    FET_SHORT_DISABLED,       // FET_SHORT
    OP_MODE_3LEAD_AMP_CELL,   // OP_MODE
};

// SGX-4NH3-300 - Ammonia sensor
const sensorType SENSOR_NH3 = {
    40.0F,                  // nanoAmperesPerPPM
    0.5,                      // internalZeroCalibration
    ADS_GAIN_2_048V,          // adsGain
    TIA_GAIN_120_KOHM,        // TIA_GAIN_IN_KOHMS
    RLOAD_10_OHM,            // RLOAD
    REF_EXTERNAL,             // REF_SOURCE
    INTERNAL_ZERO_20_PERCENT, // INTERNAL_ZERO
    BIAS_SIGN_POSITIVE,       // BIAS_SIGN
    BIAS_0_PERCENT,           // BIAS
    FET_SHORT_DISABLED,       // FET_SHORT
    OP_MODE_3LEAD_AMP_CELL,   // OP_MODE
};


#endif