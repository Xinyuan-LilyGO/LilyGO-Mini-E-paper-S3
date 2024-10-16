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

#include "GxEPD2_213_FC1.h"

GxEPD2_213_FC1::GxEPD2_213_FC1(int16_t cs, int16_t dc, int16_t rst, int16_t busy, SPIClass &spi) :
  GxEPD2_EPD(cs, dc, rst, busy, LOW, 6000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate, spi)
{
}

// Generic: clear display memory (one buffer only), using specified command
void GxEPD2_213_FC1::_writeScreenBuffer(uint8_t command, uint8_t value) {
  _writeCommand(command); // set current
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(value);
  }
  yield();  // Allegedly: keeps ESP32 and ESP8266 WDT happy
}

// Clear display: memory + screen image
void GxEPD2_213_FC1::clearScreen(uint8_t value)
{
  _Wake();
  // _Init_Full();
  _writeScreenBuffer(0x13, value); // Clear "NEW" (red) mem
  _writeScreenBuffer(0x10, value); // Clear "OLD" (black) mem
  refresh(false); // Full refresh
  _PowerOff();
  _initial_write = false;
  _initial_refresh = false;
}

// Clear display's memory - no refresh
void GxEPD2_213_FC1::writeScreenBuffer(uint8_t value)
{
  _Wake();
  _writeScreenBuffer(0x13, value); // Clear "NEW" (red) mem
  _writeScreenBuffer(0x10, value); // Clear "OLD" (black) mem
  _initial_write = false;
}

// Generic: write image to memory, using specified command
void GxEPD2_213_FC1::_writeImage(uint8_t command, const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  // Mostly redundant - controller IC does not support partial update, only fast-refresh
  // Potentially still used for the "mirror" code? Untested
  int16_t wb = (w + 7) / 8; // width bytes, bitmaps are padded
  x -= x % 8; // byte boundary
  w = wb * 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;

  // Controller doesn't support partial refresh, so we'll only allow fullscreen images, just incase
  if (w < WIDTH || h < HEIGHT)
    return;

  _writeCommand(command);

  //uint8_t extras[h1] = {0};
  
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < wb; j++)
    {
      uint8_t data;
      // use wb, h of bitmap for index!
      int16_t idx = mirror_y ? j + dx / 8 + ((h - 1 - (i + dy))) * wb : j + dx / 8 + (i + dy) * wb;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&bitmap[idx]);
#else
        data = bitmap[idx];
#endif
      }
      else
      {
        data = bitmap[idx];
      }
      if (invert) data = ~data;

      _writeData(data);
    }
  }  
  yield();  // Allegedly: keeps ESP32 and ESP8266 WDT happy
}

// Write image to "NEW" (red) memory
void GxEPD2_213_FC1::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (_initial_refresh) clearScreen();  // Screen image is unknown at startup: make sure it is clear.

  _Wake();
  // _Init_Part();
 
  _writeImage(0x13, bitmap, x, y, w, h, invert, mirror_y, pgm); // Run using the generic method, passing the command for write "NEW" mem
}

// Write image to "OLD" (black) memory. Used for determining which pixels mode during fast refresh
void GxEPD2_213_FC1::writeImageAgain(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm) {
  // Pass-down
  _writeImage(0x10, bitmap, x, y, w, h, invert, mirror_y, pgm); // Run using the generic method, passing the command for write "OLD" mem
}

// Update the display image
void GxEPD2_213_FC1::refresh(bool partial_update_mode)
{
  if (partial_update_mode) refresh(0, 0, WIDTH, HEIGHT);  // If requested, run a fast refresh instead
  else
  {
    _Update_Full();
    _initial_refresh = false; // initial full update done
  }
}

// Update the display image, using fast refresh
void GxEPD2_213_FC1::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  if (_initial_refresh) return refresh(false); // initial update needs be full update
  _Update_Part();
}

// Custom reset function - less delay()
void GxEPD2_213_FC1::_reset() {
  digitalWrite(_rst, LOW);
  pinMode(_rst, OUTPUT);
  delay(_reset_duration);
  pinMode(_rst, INPUT_PULLUP);
  _waitWhileBusy("_reset", 200);  // "200ms" not used, actually reads busy pin

  _hibernating = false;
  _configured_for_full = false;
  _configured_for_fast = false;
}

// End the update process (NOT to be confused with hibernate)
// ZinggJM notes: turns off generation of panel driving voltages, avoids screen fading over time
void GxEPD2_213_FC1::powerOff()
{
  _PowerOff();
}

// No hibernate for this display, only power off. Preserves image memory for fast refresh.
void GxEPD2_213_FC1::hibernate()
{
  _PowerOff();
}

void GxEPD2_213_FC1::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_213_FC1::_PowerOff()
{
  if (_power_is_on)
  {
    _writeCommand(0x02);
    _waitWhileBusy("_PowerOff", power_off_time);
  }
  _power_is_on = false;
}

// Common setup for both full refresh and fast refresh
void GxEPD2_213_FC1::_Wake()
{
  // Note: no hibernation for meshtastic code, because display memory is lost (quirk of controller IC?)
  if (_hibernating) 
    return _reset();

  // At start-up: rescue display from any weird states
  if (_initial_refresh)
    return _reset();
}

// Load config for full refresh. Called only by _InitDisplay()
void GxEPD2_213_FC1::_Init_Full()
{
  if (_configured_for_full) return; // If already configured, abort

    // Soft reset (via panel setting register)
  _writeCommand(0x00);
  _writeData(0); // [0] RST_N
  _waitWhileBusy("soft reset", 0);

  // Panel setting
  // [7:6] Display Res, [5] LUT, [4] BW / BWR [3] Scan Vert, [2] Shift Horiz, [1] Booster, [0] !Reset
  _writeCommand(0x00);
  _writeData(0b11 << 6 | 1 << 4 | 1 << 3 | 1 << 2 | 1 << 1 | 1 << 0);

  // VCOM and data interval setting
  // [7:6] Border, [5:4] Data polarity (default), [3:0] VCOM and Data interval (default)
  _writeCommand(0x50);
  _writeData(0b10 << 6 | 0b11 << 4 | 0b0111 << 0);

  _configured_for_full = true;
  _configured_for_fast = false;
}

// Load config for fast refresh. Called only by _InitDisplay()
void GxEPD2_213_FC1::_Init_Part()
{
  if (_configured_for_fast) return;  // If already configured, abort

  // Soft reset (via panel setting register)
  _writeCommand(0x00);
  _writeData(0); // [0] RST_N
  _waitWhileBusy("soft reset", 0);

  // Panel setting
  // [7:6] Display Res, [5] LUT, [4] BW / BWR [3] Scan Vert, [2] Shift Horiz, [1] Booster, [0] !Reset
  _writeCommand(0x00);
  _writeData( 0b11 << 6 | 1 << 5 | 1 << 4 | 1 << 3 | 1 << 2 | 1 << 1 | 1 << 0 );

  // VCOM and data interval setting
  // [7:6] Border, [5:4] Data polarity (default), [3:0] VCOM and Data interval (default)
  _writeCommand(0x50);
  _writeData( 0b11 << 6 | 0b01 << 4 | 0b0111 << 0 );

  // Load the various LUTs
  _writeCommand(0x20);  // VCOM
  for(uint8_t i=0;i < sizeof(lut_20_vcomDC_partial); i++) 
    _writeData(pgm_read_byte_near(lut_20_vcomDC_partial + i));
  
  _writeCommand(0x21);  // White -> White
  for(uint8_t i=0;i < sizeof(lut_21_ww_partial); i++) 
    _writeData(pgm_read_byte_near(lut_21_ww_partial + i));
  
  _writeCommand(0x22);  // Black -> White
  for(uint8_t i=0;i < sizeof(lut_22_bw_partial); i++) 
    _writeData(pgm_read_byte_near(lut_22_bw_partial + i));
  
  _writeCommand(0x23);  // White -> Black
  for(uint8_t i=0;i < sizeof(lut_23_wb_partial); i++) 
    _writeData(pgm_read_byte_near(lut_23_wb_partial + i));
  
  _writeCommand(0x24);  // Black -> Black
  for(uint8_t i=0;i < sizeof(lut_24_bb_partial); i++) 
    _writeData(pgm_read_byte_near(lut_24_bb_partial + i));

  _configured_for_fast = true;
  _configured_for_full = false;
}

void GxEPD2_213_FC1::_Update_Full()
{
  _Init_Full();
  _PowerOn();
  _writeCommand(0x12);
  _waitWhileBusy("_Update_Full", full_refresh_time);
}

void GxEPD2_213_FC1::_Update_Part()
{
  _Init_Part();
  _PowerOn();
  _writeCommand(0x12);
  _waitWhileBusy("_Update_Part", partial_refresh_time);
}

// Fast refresh waveform is unofficial (experimental?)
// https://github.com/todd-herbert/heltec-eink-modules/tree/v4.1.2/src/Displays/LCMEN2R13EFC1/LUTs

const unsigned char GxEPD2_213_FC1::lut_20_vcomDC_partial[] PROGMEM =
{
0x01, 0x06, 0x03, 0x02, 0x01, 0x01, 0x01,
0x01, 0x06, 0x02, 0x01, 0x01, 0x01, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char GxEPD2_213_FC1::lut_21_ww_partial[] PROGMEM =
{
  0x01, 0x06, 0x03, 0x02, 0x81, 0x01, 0x01,
  0x01, 0x06, 0x02, 0x01, 0x01, 0x01, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char GxEPD2_213_FC1::lut_22_bw_partial[] PROGMEM =
{
  0x01, 0x86, 0x83, 0x82, 0x01, 0x01, 0x01,
  0x01, 0x86, 0x82, 0x01, 0x01, 0x01, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char GxEPD2_213_FC1::lut_23_wb_partial[] PROGMEM =
{
  0x01, 0x46, 0x43, 0x02, 0x01, 0x01, 0x01,
  0x01, 0x46, 0x42, 0x01, 0x01, 0x01, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char GxEPD2_213_FC1::lut_24_bb_partial[] PROGMEM =
{
  0x01, 0x06, 0x03, 0x42, 0x41, 0x01, 0x01,
  0x01, 0x06, 0x02, 0x01, 0x01, 0x01, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};