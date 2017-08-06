Road To RIoT Jakarta
====================

These code are shown during my talk about Introduction to RIoT Hardware Kits & ESP32 Programming at BINUS University, Jakarta.

## ESP-IDF with Arduino

The project `oled_bmp_queue` requires Arduino for ESP32 to be present inside the components directory. To make it happen, inside the `oled_bmp_queue` directory run this command:

```bash
git clone https://github.com/espressif/arduino-esp32 components/arduino-esp32
```

Once you've done that, you can build and flash the code.


## Building and flashing the Code

Each directory is a project that uses ESP-IDF expect the `oled_bmp` one, it uses PlatformIO as the development tool + Arduino as the framewrork.

To build and flash the code, run this command:

```
make flash
```

or alternatively you can provide `-jN` flag where `N` is the number of task that will be run during the build process.

For the PlatformIO project, run using this command:

```
platformio run -t upload
```

