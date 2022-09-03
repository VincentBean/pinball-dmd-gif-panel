# Pinball DMD Clock


## Building

### SdFat

Change the `SPI_DRIVER_SELECT` from `0` to `2`.

File: `.pio/libdeps/wemos_d1_mini32/SdFat/src/SdFatConfig.h`

```
#define SPI_DRIVER_SELECT 0
```

TODO: Use a patch for this





## Resources / Similar projects

Here is a list of resources and similar projects.

### GIFS


### Open Source Projects

[DMD clock](https://gitlab.com/modernhackerspace/dmdclock)

[Teensy2dmd](https://github.com/gi1mic/teensy2dmd)

[Matrix library, lots of information about supported panels](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA)
