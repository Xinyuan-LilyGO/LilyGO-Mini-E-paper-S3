/*!
 * @file stk8baxx.cpp
 *
 * @mainpage SonsorTek STK8xxx Digital Output 3-axis MEMS Accelerometer
 *
 * @section author Author
 *
 * Reworked by Michael Gjelsø, for use with Meshtastic and RadioMaster Bandit
 * Credits to ExpressLRS for their work.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 * See the LICENSE file for details.
 *
 */

#include "stk8baxx.h"
#include <Arduino.h>
#include <Wire.h>

#define PID_SIZE 16
uint8_t chipid_temp = 0x00;
uint8_t range_temp = 0x00;
uint8_t stk8xxx_pid_list[PID_SIZE] = {STK8xxx_CHIPID_VAL, STK8BA50_X_CHIPID_VAL, STK8BA5X_CHIPID_VAL, STK8327_CHIPID_VAL};

/*!
 *   @brief  Read from I2C Register
 *   @param  reg Read from address
 *   @param  data Store in Data.
 */
void STK8xxx::ReadAccRegister(uint8_t reg, uint8_t *data)
{
    Wire.beginTransmission(STK8xxx_SLAVE_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(STK8xxx_SLAVE_ADDRESS, 1); // request 1 bytes from slave device
    *data = Wire.read();                        // receive a byte
}

/*!
 *   @brief  Write to I2C Register
 *   @param  reg Write to address
 *   @param  data Data to be written.
 */
void STK8xxx::WriteAccRegister(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(STK8xxx_SLAVE_ADDRESS);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}

/*!
 *   @brief  Setup for Any motion.
 */
void STK8xxx::STK8xxx_Anymotion_init()
{
    unsigned char ARegWriteValue;

    /* Enable X Y Z-axis any-motion (slope) interrupt */
    ARegWriteValue = STK8xxx_VAL_SLP_EN_X | STK8xxx_VAL_SLP_EN_Y | STK8xxx_VAL_SLP_EN_Z;
    WriteAccRegister(STK8xxx_REG_INTEN1, ARegWriteValue);

    /* Set any motion Interrupt trigger threshold */
    WriteAccRegister(STK8xxx_REG_SLOPETHD, STK8xxx_VAL_SLP_DFLT);

    /* Enable any-motion */
    WriteAccRegister(STK8xxx_REG_SIGMOT2, STK8xxx_VAL_ANY_MOT_EN);

    /* Map any-motion (slope) interrupt to INT1 */
    WriteAccRegister(STK8xxx_REG_INTMAP1, STK8xxx_VAL_ANYMOT2INT1);
}

/*!
 *   @brief  Setup for Significant motion
 */
void STK8xxx::STK8xxx_Sigmotion_init()
{
    unsigned char SRegWriteValue;

    /* Enable X Y Z-axis sig-motion (slope) interrupt */
    SRegWriteValue = STK8xxx_VAL_SLP_EN_X | STK8xxx_VAL_SLP_EN_Y | STK8xxx_VAL_SLP_EN_Z;
    WriteAccRegister(STK8xxx_REG_INTEN1, SRegWriteValue);

    /* Set sig-motion Interrupt trigger threshold */
    WriteAccRegister(STK8xxx_REG_SLOPETHD, STK8xxx_VAL_SLP_DFLT);

    /* Enable significant motion */
    WriteAccRegister(STK8xxx_REG_SIGMOT2, STK8xxx_VAL_SIG_MOT_EN);

    /* Map significant motion interrupt to INT1 */
    WriteAccRegister(STK8xxx_REG_INTMAP1, STK8xxx_VAL_SIGMOT2INT1);
}

/*!
 *   @brief  Disable motion
 */
void STK8xxx::STK8xxx_Disable_Motion()
{
    /* Disable X Y Z-axis motion (slope) interrupt */
    WriteAccRegister(STK8xxx_REG_INTEN1, 0x00);

    /* Disable motion */
    WriteAccRegister(STK8xxx_REG_SIGMOT2, 0x00);
}

/*!
 *   @brief  Suspend Mode
 */
void STK8xxx::STK8xxx_Suspend_mode()
{
    /* suspend mode enable */
    WriteAccRegister(STK8xxx_REG_POWMODE, STK8xxx_VAL_SUSPEND);
}

/*!
 *   @brief Check for a valid CHIP ID
 *   @returns true on success, false otherwise
 *   Stores CHIP ID in chipid_temp
 */
bool STK8xxx::STK8xxx_Check_chipid()
{
    uint8_t RegAddr = STK_REG_CHIPID;
    int i = 0, pid_num = (sizeof(stk8xxx_pid_list) / sizeof(stk8xxx_pid_list[0]));

    ReadAccRegister(RegAddr, &chipid_temp);
    for (i = 0; i < pid_num; i++) {
        if (chipid_temp == stk8xxx_pid_list[i]) {
            return true;
        }
    }
    return false;
}

/*!
 *   @brief Initializes the CHIP with a specific Sensing Range
 *   @param range Is used to set Sensing Range
 */
int STK8xxx::STK8xxx_Initialization(uint8_t range)
{
    if (!STK8xxx_Check_chipid()) {
        return -1;
    }

    range_temp = range;

    /* soft-reset */
    WriteAccRegister(STK8xxx_REG_SWRST, STK8xxx_VAL_SWRST_RESET);
    delay(50); // unit ms

    /* set range, resolution */
    WriteAccRegister(STK8xxx_REG_RANGESEL, range);

    /* set power mode to active */
    WriteAccRegister(STK8xxx_REG_POWMODE, STK8xxx_VAL_SLEEP_05);

    /* set bandwidth 125Hz*/
    WriteAccRegister(STK8xxx_REG_BWSEL, STK8xxx_VAL_BW_125);

    /* set i2c watch dog, enable watch dog */
    WriteAccRegister(STK8xxx_REG_INTFCFG, STK8xxx_VAL_I2C_WDT_EN);

    /* int config INT1/INT2 push-pull, active high */
    WriteAccRegister(STK8xxx_REG_INTCFG1, STK8xxx_VAL_INT_LV);

    return chipid_temp;
}

/*!
 *   @brief Set Bandwidth Range
 *   @param bandwidth Is used to set bandwidth range
 */
void STK8xxx::STK8xxx_Set_Bandwidth(uint8_t bandwidth)
{
    /* set bandwidth */
    WriteAccRegister(STK8xxx_REG_BWSEL, bandwidth);
}

/*!
 *   @brief Set Sensing Range
 *   @param range Is used to set Sensing Range
 */
void STK8xxx::STK8xxx_Set_Range(uint8_t range)
{
    /* set range, resolution */
    range_temp = range;
    WriteAccRegister(STK8xxx_REG_RANGESEL, range);
}

/*!
 *   @brief Get Sensitivity
 *   @returns Sensitivity value
 */
int STK8xxx::STK8xxx_Get_Sensitivity()
{
    int sensitivity = 0;
    if (0x86 == chipid_temp) {
        // resolution = 10 bit
        sensitivity = 1 << 9;
    } else {
        // resolution = 12 bit
        sensitivity = 1 << 11;
    }
    sensitivity = sensitivity / range_temp;
    return sensitivity;
}

/*!
 *   @brief Read XYZ sensor data
 *   @param X_DataOut X-axis data output
 *   @param Y_DataOut Y-axis data output
 *   @param Z_DataOut Z-axis data output
 */
void STK8xxx::STK8xxx_Getregister_data(float *X_DataOut, float *Y_DataOut, float *Z_DataOut)
{
    uint8_t RegAddr, RegReadValue[2];
    int16_t x, y, z;

    RegAddr = STK8xxx_REG_XOUT1;
    RegReadValue[0] = 0x00;
    ReadAccRegister(RegAddr, &RegReadValue[0]);
    RegAddr = STK8xxx_REG_XOUT2;
    RegReadValue[1] = 0x00;
    ReadAccRegister(RegAddr, &RegReadValue[1]);
    x = (short int)(RegReadValue[1] << 8 | RegReadValue[0]);

    RegAddr = STK8xxx_REG_YOUT1;
    RegReadValue[0] = 0x00;
    ReadAccRegister(RegAddr, &RegReadValue[0]);
    RegAddr = STK8xxx_REG_YOUT2;
    RegReadValue[1] = 0x00;
    ReadAccRegister(RegAddr, &RegReadValue[1]);
    y = (short int)(RegReadValue[1] << 8 | RegReadValue[0]);

    RegAddr = STK8xxx_REG_ZOUT1;
    RegReadValue[0] = 0x00;
    ReadAccRegister(RegAddr, &RegReadValue[0]);
    RegAddr = STK8xxx_REG_ZOUT2;
    RegReadValue[1] = 0x00;
    ReadAccRegister(RegAddr, &RegReadValue[1]);
    z = (short int)(RegReadValue[1] << 8 | RegReadValue[0]);

    if (0x86 == chipid_temp) {
        // resolution = 10 bit
        x >>= 6;
        *X_DataOut = (float)x / STK8xxx_Get_Sensitivity();

        y >>= 6;
        *Y_DataOut = (float)y / STK8xxx_Get_Sensitivity();

        z >>= 6;
        *Z_DataOut = (float)z / STK8xxx_Get_Sensitivity();
    } else {
        // resolution = 12 bit
        x >>= 4;
        *X_DataOut = (float)x / STK8xxx_Get_Sensitivity();

        y >>= 4;
        *Y_DataOut = (float)y / STK8xxx_Get_Sensitivity();

        z >>= 4;
        *Z_DataOut = (float)z / STK8xxx_Get_Sensitivity();
    }
}
