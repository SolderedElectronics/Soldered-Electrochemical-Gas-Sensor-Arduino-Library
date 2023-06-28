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

// Struct which contains the data for each sensor
struct sensorType
{
    uint8_t tiacn;
    uint8_t refcn;
    uint8_t modecn;
    double nanoAmperesPerPPM;
    double tiaGainInKOhms;
};

// These are the different sensor types
// Refer to the official LMP91000 datasheet, chapter 7.6 for more info

/**
 * SGX-4NO-250 - Nitric Oxide sensor 
 * tiacn: TIA_GAIN: 120kOhm, RLOAD: 10Ohm
 * refcn: REF_SOURCE: external, INT_Z: 50%, BIAS_SIGN: positive, BIAS: 12%
 * modecn: FET_SHORT: disabled, OP_MODE: 3-lead amperometric cell
 * nanoAmperesPerPPM: 400
 * tiaGainInKOhms: 120
*/
const sensorType SENSOR_NO = {0x18, 0xB7, 0x03, 400.0F, 120};

/**
 * SGX-4NO2 - Nitrogen Dioxide sensor 
 * tiacn: TIA_GAIN: 120kOhm, RLOAD: 10Ohm
 * refcn: REF_SOURCE: external, INT_Z: 50%, BIAS_SIGN: negative, BIAS: 0%
 * modecn: FET_SHORT: disabled, OP_MODE: 3-lead amperometric cell
 * nanoAmperesPerPPM: 600
 * tiaGainInKOhms: 120
*/
const sensorType SENSOR_NO2 = {0x18, 0xA0, 0x03, 600.0F, 120};

/**
 * SGX-4SO2 - Sulphur Dioxide sensor 
 * tiacn: TIA_GAIN: 120kOhm, RLOAD: 10Ohm
 * refcn: REF_SOURCE: external, INT_Z: 50%, BIAS_SIGN: negative, BIAS: 0%
 * modecn: FET_SHORT: disabled, OP_MODE: 3-lead amperometric cell
 * nanoAmperesPerPPM: 400
 * tiaGainInKOhms: 120
*/
const sensorType SENSOR_SO2 = {0x18, 0xA0, 0x03, 400.0F, 120};

#endif