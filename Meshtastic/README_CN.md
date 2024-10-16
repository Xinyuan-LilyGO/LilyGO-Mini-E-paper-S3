<h1 align = "center">LilyGO-Mini-E-Paper-S3</h1>

## **[English](./README.md) | 中文**

## :one: 描述 🎁
这是`LilyGO-Mini-E-Paper-S3`和`Lora`拓展版搭建的硬件平台，适配`Meshtastic`软件平台，您可以使用`PlatformIO IDE`来编译他。如何使用`Meshtastic`软件平台，请参考[官方文档](https://meshtastic.org/)。

## :two: Product 🎯

| Product(PinMap)              | SOC         | Flash | PSRAM     | Resolution | Size      | Driver       |
| ---------------------------- | ----------- | ----- | --------- | ---------- | --------- | ------------ |
| [Mini-E-Paper-S3][1]         | ESP32-FN4R2 | 4MB   | 2MB(QSPI) | 128x80     | 1.02 Inch | GDGDEW0102T4 |

[1]: https://www.lilygo.cc

## :three: Lora 拓展版🎯
Lora拓展版内置了SX1262 Lora模块、PCF85063 RTC模块和一个红外发射器MMBT3904。

## :four: 预览
--------------------------------------

## :five: PlatformIO 快速开始

<font color="green"> 提示：下面一个是安装的流程，更详细的安装，可以使用浏览器搜索 PlatformIO 的安装教程；例如：[PlatformIO 环境安装](https://zhuanlan.zhihu.com/p/509527710)</font>

1. 安装 [VScode]((https://code.visualstudio.com/)) 和 [Python](https://www.python.org/)，并且克隆或下载此项目；
2. 在 VScode 的扩展中搜索 PlatformIO 的插件，然后安装它；
3. 在 PlatformIO 插件安装完成后，需要重新启动 VScode，<font color="red">然后用 VScode 打开`Meshtastic`文件夹；</font>
<img src="/images/1.png"></img>
<img src="/images/2.png"></img>

4. 打开此工程后，PlatformIO 会自动的下载需要的三方库和依赖，第一次这个过程比较长，情耐心等待；
5. 当所有的依赖安装后，可以打开 `platformio.ini` 配置文件，并取消`default_envs = mini_e-paper`注释,将其他`default_envs = xxx`注释上，选择后按下 `ctrl+s` 保存 .ini 配置文件；
6. 点击 VScode 下面的 :ballot_box_with_check: 编译工程，按住`BOOT_BUTTON`按键然后插上 USB 在 VScode 下面选择 COM 口；
7. 最后点击 :arrow_right: 按键将程序下载到 Flash 中；



## :six: 引脚总览
|   BUTTON      | ESP32S3引脚 |
|:-------------:| :----------: |
|   BOOT_BUTTON         |  IO0    |  
|   NEXT_PAGE_BUTTON    |  IO3    |

|   LoRA SX1262  | ESP32S3引脚 |
|:-------------:| :----------: |
|   LORA_DIO1    |  IO5        |  
|   LORA_MISO    |  IO6        |
|   LORA_CS      |  IO7        |
|   LORA_SCK     |  IO8        |
|   LORA_MOSI    |  IO17       |

|  GDGDEW0102T4 |  ESP32S3引脚 |
|:-------------:| :----------: |
|PIN_EINK_MOSI  |    IO15      |
|PIN_EINK_SCLK  |    IO14      |
|PIN_EINK_CS    |    IO13      |
|PIN_EINK_DC    |    IO12      |
|PIN_EINK_RES   |    IO11      |
|PIN_EINK_BUSY  |    IO10      |


| PCF8563 RTC Module | ESP32S3引脚  |
| :------------------: |:----------:|
| I2C_SDA            | IO18         |
| I2C_SCL            | IO9          |

|   MMBT3904       | ESP32S3引脚  |
|:-------------:   | :----------: |
|  INFRARED_INPUT  |  IO21        | 

# 固件烧录
打开`tools`文件夹，解压并安装`flash_download_tool_3.9.3`烧录软件，打开`flash_download_tool_3.9.3.exe`,按照如图所示对`Mini-E-Paper-S3`进行软件烧录。<font color="red"> 烧录对应的文件在本目录下的`firmware`文件夹。<font>
<img src="/images/disposition.png" alt="disposition" style="width: 30%; min-width: 100px; display: block; margin: auto;">
<img src="/images/burn.png" alt="burn" style="width: 70%; min-width: 100px; display: block; margin: auto;">

# Meshtastic Firmware

![GitHub release downloads](https://img.shields.io/github/downloads/meshtastic/firmware/total)
[![CI](https://img.shields.io/github/actions/workflow/status/meshtastic/firmware/main_matrix.yml?branch=master&label=actions&logo=github&color=yellow)](https://github.com/meshtastic/firmware/actions/workflows/ci.yml)
[![CLA assistant](https://cla-assistant.io/readme/badge/meshtastic/firmware)](https://cla-assistant.io/meshtastic/firmware)
[![Fiscal Contributors](https://opencollective.com/meshtastic/tiers/badge.svg?label=Fiscal%20Contributors&color=deeppink)](https://opencollective.com/meshtastic/)
[![Vercel](https://img.shields.io/static/v1?label=Powered%20by&message=Vercel&style=flat&logo=vercel&color=000000)](https://vercel.com?utm_source=meshtastic&utm_campaign=oss)

## Overview

This repository contains the device firmware for the Meshtastic project.

- **[Building Instructions](https://meshtastic.org/docs/development/firmware/build)**
- **[Flashing Instructions](https://meshtastic.org/docs/getting-started/flashing-firmware/)**

## Stats

![Alt](https://repobeats.axiom.co/api/embed/a92f097d9197ae853e780ec53d7d126e545629ab.svg "Repobeats analytics image")


