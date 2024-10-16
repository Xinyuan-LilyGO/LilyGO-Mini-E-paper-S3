
<h1 align = "center">🌟LilyGO Mini Epaper🌟</h1>


--------------------------------------
## 1️⃣Product

| Product(PinMap)              | SOC         | Flash | PSRAM     | Resolution | Size      | Driver       |
| ---------------------------- | ----------- | ----- | --------- | ---------- | --------- | ------------ |
| [Mini-E-Paper-S3][1]         | ESP32-FN4R2 | 4MB   | 2MB(QSPI) | 128x80     | 1.02 Inch | GDGDEW0102T4 |

[1]: https://www.lilygo.cc

<h2 align = "left">Quick start ⚡:</h2>

## 2️⃣ PlatformIO Quick Start (Recommended)

1. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/)
2. Search for the `PlatformIO` plugin in the `VisualStudioCode` extension and install it.
3. After the installation is complete, you need to restart `VisualStudioCode`
4. After restarting `VisualStudioCode`, select `File` in the upper left corner of `VisualStudioCode` -> `Open Folder` -> select the `LilyGO-Mini-Epaper` directory
5. Wait for the installation of third-party dependent libraries to complete
6. Click on the `platformio.ini` file, and in the `platformio` column
7. Select the board name you want to use in `default_envs` and uncomment it.
8. Uncomment one of the lines `src_dir = xxxx` to make sure only one line works
9. Click the (✔) symbol in the lower left corner to compile
10. Connect the board to the computer USB
11. Click (→) to upload firmware
12. Click (plug symbol) to monitor serial output
13. If it cannot be written, or the USB device keeps flashing, please check the **FAQ** below


# :four: FAQ

1. `Mini-E-Paper-S3` uses USB as the JTAG upload port. When printing serial port information on USB_CDC_ON_BOOT configuration needs to be turned on.
2. If the port cannot be found when uploading the program or the USB has been used for other functions, the port does not appear.
Please enter the upload mode manually.
   1. Connect the board via the USB cable (`Mini-E-Paper-ESP32 PICO`  requires an external U2T to discover the port)
   2. Press and hold the BOOT button , While still pressing the BOOT button, press RST
      ![BOOT](./images/download.jpg)
   3. Release the RST
   4. Release the BOOT button
   5. Upload sketch
3. If the above is invalid, burn the [binary file](./firmware/README.MD)  to check whether the hardware is normal


