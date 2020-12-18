#include <stdint.h>
#include <errno.h>

#include <stdio.h>

#include "dac161s997.h"
#include "mock_spi.h"

int main (void)
{
    uint32_t status = 0;
    dac161s997_dev_t dev0 = {.cs_num = 0};
    dac161s997_dev_t dev1 = {.cs_num = 1};
    puts("dac161s997_init(&dev0)");
    while (dac161s997_init(&dev0) != 0);
    puts("dac161s997_init(&dev1)");
    while (dac161s997_init(&dev1) != 0);
    puts("dac161s997_set_output(&dev0, 1000)");
    dac161s997_set_output(&dev0, 1000);
    puts("dac161s997_set_alarm(&dev0, DAC161S997_ALARM_LOW_FAIL)");
    dac161s997_set_alarm(&dev0, DAC161S997_ALARM_LOW_FAIL);
    puts("dac161s997_set_alarm(&dev0, DAC161S997_ALARM_HIGH_FAIL)");
    dac161s997_set_alarm(&dev0, DAC161S997_ALARM_HIGH_FAIL);
    puts("dac161s997_get_status(&dev0, &status)");
    dac161s997_get_status(&dev0, &status);
    printf("status: 0x%X", status);
    return 0;
}