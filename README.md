<p align="center">
<table>
<td>
<h1>Helper utility for:</h2>
</td>

<td>
<a href="https://github.com/HerrNamenlos123/JTAG_Interface">
<p align="right"><img src="assets/logo.svg" alt="JTAG_Interface logo" height="100px"></p>
</a>
</td>
</table>
</p>

# Byte reverser 💻

When developing FPGA programs for the Arduino MKR Vidor 4000 it is a great question how to upload the bitstream to the FPGA part of the Arduino.  

This application takes care of it to make development as smooth for you as possible.

## How to get

On Windows, you can just download the latest release:

https://github.com/HerrNamenlos123/TechnicalSketcher/releases/latest/download/TechnicalSketcher-Installer-x64.msi

Currently the GUI is Windows-only and 32 bit for maximum compatibility. The Linux GUI might be coming in the future, please let me know if you want it, so i know if there is interest!

For now, Linux users can use the CLI tool directly, for that just build the utility in the subfolder `bytereverse`, then just use it like any other command line utility. You will find out how to use it.

The CLI-tool can be built on any platform:
```
cd bytereverse
mkdir build
cd build
cmake ..
cmake --build .
```

## Why do we need this?

Intel Quartus generates an output file `MKRVIDOR4000.ttf` or similar, that we need to upload to the Arduino's FPGA, but it is not in the right format yet, it is bit-reversed.  

This utility converts the file to the right format and writes it to a `FPGA_Bitstream.h` file, which is then included in your Arduino program. 

## What does it do?

When you start the application, a tray icon will appear. Right-clicking it will open the options UI where you can create a profile and choose the input and output files. After that, you can left-click the tray icon to immediately convert the file.

<p align="center"><table>
<td valign="bottom"><img src="assets/bytereverse_work.png" alt="ByteReverser file conversion UI"></td>
<td valign="bottom"><img src="assets/bytereverse_options.png" alt="ByteReverser options UI"></td>
</table></p>

This immensely speeds up your workflow, as you only need to build the Quartus project, click once in the taskbar and can immediately upload the Arduino project, no more file copying.

There is also an instant feedback feature, where you can write me any messages if you think I could improve something. There is a Telegram bot under the hood, which directly sends me your message. Please do make use of it and tell me of any bugs or improvements I could make.
