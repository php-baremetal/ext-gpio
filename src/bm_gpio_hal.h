/*
   +----------------------------------------------------------------------+
   | php-baremetal: ext-gpio                                              |
   +----------------------------------------------------------------------+
   | Copyright (c) 2026 Gianfrancesco Aurecchia                           |
   +----------------------------------------------------------------------+
   | This source file is subject to the MIT license, that is bundled with |
   | this package in the file LICENSE.                                    |
   +----------------------------------------------------------------------+
   | Authors: Gianfrancesco Aurecchia <gianfri@aurecchia.com>             |
   +----------------------------------------------------------------------+
*/

/*
 * Neutral GPIO HAL. The gpio extension uses only these; an arch package implements them
 * (esp32-gpio-hal over driver/gpio). A different family ships its own implementation.
 */
#pragma once

/* mode: 1 = output (driven, still readable back); 0 = input. */
void bm_gpio_hal_mode(int pin, int output);
void bm_gpio_hal_write(int pin, int level);
int  bm_gpio_hal_read(int pin);
