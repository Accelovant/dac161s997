#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "dac161s997.h"
#include "mock_spi.h"

int _safeInput(int *number)
{
    long a;
    char buf[1024];
    char *endptr;

    if (!fgets(buf, 1024, stdin)) {
        return 0;
    }

    errno = 0;
    a = strtol(buf, &endptr, 10);
    if (errno == ERANGE) {
        return 0;
    }
    if (endptr == buf) {
        return 0;
    }
    if (*endptr && *endptr != '\n') {
        return 0;
    }
    if (a > INT_MAX || a < INT_MIN) {
        return 0;
    }
    *number = (int) a;
    return 1;
}

int dac161s997_spi_xfer(dac161s997_dev_t *dev, uint8_t* tx_buf,
		uint8_t* rx_buf, size_t size) {
	int err = 0;
    int n;

    printf("\ndac161s997_spi_xfer(dev=%d, size=%d)\n", dev->cs_num, (int)size);
    for (size_t i = 0; i < size; i++) {
        printf("\ntx byte = 0x%02X\n", tx_buf[i]);
        printf("Enter rx byte: ");
        if (!_safeInput(&n)) {
            puts("Not a valid byte, 0 is assumed");
            n = 0;
        }
        else {
            rx_buf[i] = n;
        }
        printf("rx byte = 0x%02X\n", n);
    }
    puts("\nEnter error code (invalid numbers will return 0): ");
    if (!_safeInput(&n)) {
        return 0;
    }
	return err;
}