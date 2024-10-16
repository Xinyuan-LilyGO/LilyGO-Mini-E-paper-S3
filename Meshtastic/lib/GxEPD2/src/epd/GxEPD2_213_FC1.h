// ##################################################################
// #   Meshtastic: custom driver for Heltec Wireless Paper V1.1     #
// ##################################################################
//
// From discussion: https://forum.arduino.cc/t/lcmen2r13efc1-discussion-heltec-wireless-paper/1211430
//
// Panel: LCMEN2R13EFC1
// Controller: JD79656 (undocumented as of 2024/02/18)
// Board: Heltec Wireless Paper V1.1
//
// --- NOTE! ---
// This display is NOT officially supported by GxEPD2.
// This fork is not affiliated with the original author.
// This code is not intended for general use.

#ifndef _GxEPD2_213_FC1_H_
#define _GxEPD2_213_FC1_H_

#include "../GxEPD2_EPD.h"

class GxEPD2_213_FC1 : public GxEPD2_EPD
{
  public:
    // attributes
    static const uint16_t WIDTH = 128;
    static const uint16_t WIDTH_VISIBLE = 122;
    static const uint16_t HEIGHT = 250;
    static const GxEPD2::Panel panel = GxEPD2::LCMEN2R13EFC1;
    static const bool hasColor = false;
    static const bool hasPartialUpdate = true;  // Controller IC does not support partial update, but it *does* support fast refresh.
    static const bool hasFastPartialUpdate = true;
    static const uint16_t power_on_time = 0;   // Undetermined, unused
    static const uint16_t power_off_time = 0;
    static const uint16_t full_refresh_time = 0;
    static const uint16_t partial_refresh_time = 0;
    // constructor
    GxEPD2_213_FC1(int16_t cs, int16_t dc, int16_t rst, int16_t busy, SPIClass &spi);
    // methods (virtual)
    void clearScreen(uint8_t value = 0xFF); // init controller memory and screen (default white)
    void writeScreenBuffer(uint8_t value = 0xFF); // init controller memory (default white)
    // write to controller memory, without screen refresh; x and w should be multiple of 8
    void writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void writeImageAgain(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void refresh(bool partial_update_mode = false); // screen refresh from controller memory to full screen
    void refresh(int16_t x, int16_t y, int16_t w, int16_t h); // screen refresh from controller memory, fast-refresh
    void powerOff(); // turns off generation of panel driving voltages, avoids screen fading over time
    void hibernate(); // For this display, no deep sleep, only power off. Preserves image memory for fast refresh

  // Unimplemented for meshtastic
  public:
    void writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                          int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false) {}
    void drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false) {}
    void drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                        int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false) {}
    void _setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {}  // (Normally private, moved here only so it is with the other unimplemented methods)

  private:
    void _writeScreenBuffer(uint8_t command, uint8_t value);
    void _writeImage(uint8_t command, const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void _writeImagePart(uint8_t command, const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                         int16_t x, int16_t y, int16_t w, int16_t h, bool invert = false, bool mirror_y = false, bool pgm = false);
    void _reset(); // Custom, uses _waitWhileBusy() instead of delay()
    void _Wake();
    void _PowerOn();
    void _PowerOff();
    void _Init_Full();
    void _Init_Part();
    void _Update_Full();
    void _Update_Part();

  private:
    bool _configured_for_fast = false;
    bool _configured_for_full = false;
    static const unsigned char lut_20_vcomDC_partial[56];
    static const unsigned char lut_21_ww_partial[56];
    static const unsigned char lut_22_bw_partial[56];
    static const unsigned char lut_23_wb_partial[56];
    static const unsigned char lut_24_bb_partial[56];
};

#endif
