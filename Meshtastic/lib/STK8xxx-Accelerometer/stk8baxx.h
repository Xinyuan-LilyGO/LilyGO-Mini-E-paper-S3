#pragma once
#include <Arduino.h>

class STK8xxx
{
  private:
    void ReadAccRegister(uint8_t reg, uint8_t *data);
    void WriteAccRegister(uint8_t reg, uint8_t data);
    void STK8xxx_Suspend_mode();

    /** @brief Checks CHIP ID.
        @returns True if a supported CHIP ID has been found */
    bool STK8xxx_Check_chipid();

  public:
    /** @brief Sets a specified sensing range
        @param range Is used to set sensing range */
    void STK8xxx_Set_Range(uint8_t range);

    /** @brief Sets a specified bandwidth
        @param bandwidth Is used to set bandwidth range */
    void STK8xxx_Set_Bandwidth(uint8_t bandwidth);

    /** @brief Enables detection for Any motion */
    void STK8xxx_Anymotion_init();

    /** @brief Enables detection for Significant motion */
    void STK8xxx_Sigmotion_init();

    /** @brief Disables motion */
    void STK8xxx_Disable_Motion();

    /** @brief Initializes the CHIP with a specific Sensing Range
        @param range Set range to +/- 2G, 4G & 8G */
    int STK8xxx_Initialization(uint8_t range);

    /** @brief Gets Sensitivity value
        @returns Sensitivity value */
    int STK8xxx_Get_Sensitivity();

    /** @brief Read XYZ sensor data
        @returns X_DataOut X-axis data output
        @returns Y_DataOut Y-axis data output
        @returns Z_DataOut Z-axis data output */
    void STK8xxx_Getregister_data(float *X_DataOut, float *Y_DataOut, float *Z_DataOut);
};

/*
 * I2C Address of the CHIP
 */
#define STK8xxx_SLAVE_ADDRESS 0x18

/*
 * CHIP ID request address
 */
#define STK_REG_CHIPID 0x00

/*
 * Supported CHIP IDs
 */
// STK8321 or STK8323 CHIP ID = 0x23
#define STK8xxx_CHIPID_VAL 0x23
#define STK8327_CHIPID_VAL 0x26
// STK8BAxx
// S or R resolution = 10 bit
#define STK8BA50_X_CHIPID_VAL 0x86
#define STK8BA5X_CHIPID_VAL 0x87

/*
 * POWMODE, This register contains the power mode selection and the sleep time duration setting.
 */
#define STK8xxx_REG_POWMODE 0x11 // POWMODE Register Address
#define STK8xxx_VAL_SLEEP_05 0b0000 // 0.5ms
#define STK8xxx_VAL_SLEEP_1 0b0110 // 1ms
#define STK8xxx_VAL_SLEEP_2 0b0111 // 2ms
#define STK8xxx_VAL_SLEEP_4 0b1000 // 4ms
#define STK8xxx_VAL_SLEEP_6 0b1001 // 6ms
#define STK8xxx_VAL_SLEEP_10 0b1010 // 10ms
#define STK8xxx_VAL_SLEEP_25 0b1011 // 25ms
#define STK8xxx_VAL_SLEEP_50 0b1100 // 50ms
#define STK8xxx_VAL_SLEEP_100 0b1101 // 100ms
#define STK8xxx_VAL_SLEEP_500 0b1110 // 500ms
#define STK8xxx_VAL_SLEEP_1000 0b1111 // 1000ms
#define STK8xxx_VAL_SLEEP bit(5) // Sleep timer control bit in low-power mode.  0 : event driven, 1 : equidistant sampling
#define STK8xxx_VAL_LOWPOWER bit(6) // Low Power Mode 0 : Disable, 1 : Enable
#define STK8xxx_VAL_SUSPEND bit(7) // Suspend Mode 0 : Disable, 1 : Enable

/*
 * RANGESEL, This register contains the acceleration sensing range.
 */
#define STK8xxx_REG_RANGESEL 0x0F // RANGESEL Register Address
#define STK8xxx_VAL_RANGE_2G 0x03 // +/- 2G
#define STK8xxx_VAL_RANGE_4G 0x05 // +/- 4G
#define STK8xxx_VAL_RANGE_8G 0x08 // +/- 8G

/*
 * Acceleration Data
 */
#define STK8xxx_REG_XOUT1 0x02 // XOUT1/XOUT2 register contain the x-axis acceleration data and the new data flag for the x-axis.
#define STK8xxx_REG_XOUT2 0x03 //
#define STK8xxx_REG_YOUT1 0x04 // YOUT1/YOUT2 register contain the y-axis acceleration data and the new data flag for the y-axis.
#define STK8xxx_REG_YOUT2 0x05 //
#define STK8xxx_REG_ZOUT1 0x06 // ZOUT1/ZOUT2 register contain the z-axis acceleration data and the new data flag for the z-axis.
#define STK8xxx_REG_ZOUT2 0x07 //

/*
 * OFSTCOMP1, This register contains the offset compensation value for the x-axis data output.
 */
#define STK8xxx_REG_OFSTCOMP1 0x36 // OFSTCOMP1 Register Address
#define STK8xxx_VAL_OFST_RST bit(7) // 1 : Reset all the offset compensation register (register 0x38 ~ 0x3A) to zero.

/*
 * OFSTX, This register contains the offset compensation value for the x-axis data output.
 */
#define STK8xxx_REG_OFSTX 0x38 // OFSTX Register Address

/*
 * OFSTY, This register contains the offset compensation value for the y-axis data output.
 */
#define STK8xxx_REG_OFSTY 0x39 // OFSTY Register Address

/*
 * OFSTZ, This register contains the offset compensation value for the z-axis data output.
 */
#define STK8xxx_REG_OFSTZ 0x3A // OFSTZ Register Address

/*
 * BWSEL, This register contains the output data bandwidth selection.
 */
#define STK8xxx_REG_BWSEL 0x10 // BWSEL Register Address
#define STK8xxx_VAL_BW_7_81 0b01000 // 7.81Hz
#define STK8xxx_VAL_BW_15_63 0b01001 //15.63Hz
#define STK8xxx_VAL_BW_31_25 0b01010 // 31.25Hz
#define STK8xxx_VAL_BW_62_5 0b01011 // 62.5Hz
#define STK8xxx_VAL_BW_125 0b01100 // 125Hz
#define STK8xxx_VAL_BW_250 0b01101 // 250Hz
#define STK8xxx_VAL_BW_500 0b01110 // 500Hz
#define STK8xxx_VAL_BW_1000 0b01111 // 1000Hz

/*
 * INT1, This register contains the several interrupt enable bit.
 */
#define STK8xxx_REG_INTEN1 0x16 // INT1 Register Address
#define STK8xxx_VAL_SLP_EN_X bit(0) // 0 : Disable X-axis any-motion (slope) interrupt, 1 : Enable X-axis any-motion (slope) interrupt.
#define STK8xxx_VAL_SLP_EN_Y bit(1) // 0 : Disable Y-axis any-motion (slope) interrupt, 1 : Enable Y-axis any-motion (slope) interrupt.
#define STK8xxx_VAL_SLP_EN_Z bit(2) // 0 : Disable Z-axis any-motion (slope) interrupt, 1 : Enable Z-axis any-motion (slope) interrupt.

/*
 * INT2, This register contains the several interrupt enable bit.
 */
#define STK8xxx_REG_INTEN2 0x17 // INT2 Register Address
#define STK8xxx_VAL_DATA_EN (bit4) // 0 : Disable new data interrupt, 1 : Enable new data interrupt.

/*
 * INTMAP1, This register is used to map the related interrupt to the desired INT pin.
 */
#define STK8xxx_REG_INTMAP1 0x19 // INTMAP1 Register Address
#define STK8xxx_VAL_SIGMOT2INT1 bit(0) // 0 : Do not map significant motion interrupt to INT1, 1 : Map significant motion interrupt to INT1.
#define STK8xxx_VAL_ANYMOT2INT1 bit(2) // 0 : Do not map any-motion (slope) interrupt to INT1, 1 : Map any-motion (slope) interrupt to INT1.

/*
 * INTMAP2, This register is used to map the related interrupt to the desired INT pin.
 */
#define STK8xxx_REG_INTMAP2 0x1A // INTMAP2 Register Address
#define STK8xxx_VAL_DATA2INT1 // 0 : Do not map new data interrupt to INT1, 1 : Map new data interrupt to INT1.

/*
 * INTCFG1, This register is used to define the INT1 pin output type and active level.
 * Open-drain or Push-pull output type and active high or active low can be selected.
 */
#define STK8xxx_REG_INTCFG1 0x20 // INTCFG1 Register Address
#define STK8xxx_VAL_INT_LV bit(0) // INT1 active level selection. 0 : Active low, 1 : Active high.
#define STK8xxx_VAL_INT_OD bit(1) // INT1 output type selection. 0 : Push-pull output type, 1 : Open-drain output type.

/*
 * SLOPETHD, This register is used to set the threshold value for the slope detection.
 * The actual slope threshold will depend on sensing range.
 */
#define STK8xxx_REG_SLOPETHD 0x28 // SLOPETHD Register Address
#define STK8xxx_VAL_SLP_DFLT 150 // The default value of SLP_THD[7:0] is 0x14

/*
 * SIGMOT2, This register contains MSB of SKIP_TIME[8:0] for the significant motion, and significant motion interrupt enable bit.
 */
#define STK8xxx_REG_SIGMOT2 0x2A // SIGMOT2 Register Address
#define STK8xxx_VAL_SKIP_TIME bit(0)
#define STK8xxx_VAL_SIG_MOT_EN bit(1) // 0 : Disable significant motion, 1 : Enable significant motion.
#define STK8xxx_VAL_ANY_MOT_EN bit(2) // 0 : Disable any-motion, 1 : Enable any-motion.

/*
 * INTFCFG, This register contains the digital interface parameters for the I2C interface.
 */
#define STK8xxx_REG_INTFCFG 0x34 // INTFCFG Register Address.
#define STK8xxx_VAL_I2C_WDT_SEL bit(1) // I2C watchdog timer period selection. 0 : Watchdog timer period 1ms, 1 : Watchdog timer period 50ms
#define STK8xxx_VAL_I2C_WDT_EN bit(2) // I2C watchdog timer enable bit. 0 : Disable I2C watchdog timer, 1 : Enable I2C watchdog timer.

/*
 * INTCFG2, This register is used to reset latched interrupt pin and select the interrupt mode.
 */
#define STK8xxx_REG_INT_CFG2 0x21 // INTCFG2 Register Address
#define STK8xxx_VAL_INT_LAT_TMP_250MS 0b0001 // Temporary, 250ms
#define STK8xxx_VAL_INT_LAT_TMP_500MS 0b0010 // Temporary, 500ms
#define STK8xxx_VAL_INT_LAT_TMP_1S 0b0011 // Temporary, 1s
#define STK8xxx_VAL_INT_LAT_TMP_2S 0b0100 // Temporary, 2s
#define STK8xxx_VAL_INT_LAT_TMP_4S 0b0101 // Temporary, 4s
#define STK8xxx_VAL_INT_LAT_TMP_8S 0b0110 // Temporary, 8s
#define STK8xxx_VAL_INT_LAT 0b0111 // Latched
#define STK8xxx_VAL_INT_LAT_TMP_250US 0b1001 // Temporary, 250us
#define STK8xxx_VAL_INT_LAT_TMP_500US 0b1010 // Temporary, 500us
#define STK8xxx_VAL_INT_LAT_TMP_1MS 0b1011 // Temporary, 1ms
#define STK8xxx_VAL_INT_LAT_TMP_12_5MS 0b1100 // Temporary, 12.5ms
#define STK8xxx_VAL_INT_LAT_TMP_25MS 0b1101 // Temporary, 25ms
#define STK8xxx_VAL_INT_LAT_TMP_50MS 0b1110 // Temporary, 50ms
#define STK8xxx_VAL_INT_LAT_RST bit(7) // 1 : Reset any latched interrupt pin.

/*
 * DATASETUP, This register is used to select if the output data is filtered or unfiltered and how the output data contained in the
 * register XOUT1/XOUT2, YOUT1/YOUT2, ZOUT1/ZOUT2 are updated.
 */
#define STK8xxx_REG_DATASETUP 0x13 // DATASETUP Register Address
#define STK8xxx_VAL_PROTECT_DIS bit(6) // 0 : Enable the data protection function, 1 : Disable the data protection function
#define STK8xxx_VAL_DATA_SEL bit(7) // 0 : Data output filtered, 1 : Data output unfiltered.

/*
 * SWRST, This register is used to software reset.
 */
#define STK8xxx_REG_SWRST 0x14 // SWRST Register Address
#define STK8xxx_VAL_SWRST_RESET 0xB6 // Resets all the registers to default value.
