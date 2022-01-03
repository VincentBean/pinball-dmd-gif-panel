# Pinball DMD Clock


## Building

### SdFat

Change the `SPI_DRIVER_SELECT` from `0` to `2`.

File: `.pio/libdeps/wemos_d1_mini32/SdFat/src/SdFatConfig.h`

```
#define SPI_DRIVER_SELECT 0
```

TODO: Use a patch for this