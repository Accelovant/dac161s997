# DAC161S997 Driver
Platform independent driver for the [DAC161S997](https://www.ti.com/product/DAC161S997) 4/20mA driver chip.

This provides some basic functions to the driver and is implemented in a portable way.

## Driver API

The driver API is defined in the [dac161s997.h](include/dac161s997.h) file.

## Build System

### PlatformIO
The library can be imported with platformio using:
```
lib_deps = https://github.com/MrKevinWeiss/acc-dac161s997.git
```

### CMake
There is a CMake module that can be used similar to the [CMakeLists.txt](examples/basic_desktop/CMakeLists.txt).

## Driver Porting

Since the driver requires use of an SPI driver porting is required to use this driver properly.

### Selecting the device
The way to select the driver is a struct `dac161s997_dev_t`.
It must be defined by the user with arguments that will end up in the `dac161s997_spi_xfer` call.
This will most likely be a chip select but may be more complex.

A basic example using a GPIO:
```c
/* port header */
struct dac161s997_dev_t {
    gpio_t chip_select;
};
```

A more complex verion, say for a modular platform with muxed chip selects could be:
```c
/* port header */
struct dac161s997_dev_t {
    int board_select;
    int chip_select;
};
```

### Implementing the spi driver
The `dac161s997_spi_xfer` must be implemented by the user.
This allows a simple SPI driver to be used or a more complex one that may be using an RTOS.

A basic example:
```c
/* port c file */
error_t dac161s997_spi_xfer(dac161s997_dev_t *dev, uint8_t* tx_buf,
                            uint8_t* rx_buf, size_t size) {
    error_t err;
    write_gpio(dev.chip_select, GPIO_LOW);
    err =  spi_xfer(tx_buf, rx_buf, size);
    write_gpio(dev.chip_select, GPIO_HIGH);
    return err;
}
```

## Examples

A [basic example](examples/basic_desktop/) can be run on the desktop that mocks the spi data with user input.



