/*
   +----------------------------------------------------------------------+
   | php-baremetal: ext-gpio                                              |
   +----------------------------------------------------------------------+
   | Copyright (c) 2026 Gianfrancesco Aurecchia                           |
   +----------------------------------------------------------------------+
   | This source file is subject to the MIT license, that is bundled with |
   | this package in the file LICENSE.                                    |
   +----------------------------------------------------------------------+
   | Authors: Gianfrancesco Aurecchia <gianfri@php-baremetal.com>         |
   +----------------------------------------------------------------------+
*/

/*
 * gpio extension: lets a script drive the board's pins directly.
 */
#ifndef PHP_GPIO_H
#define PHP_GPIO_H

#include "php.h"

extern zend_module_entry gpio_module_entry;
#define phpext_gpio_ptr (&gpio_module_entry)

#endif /* PHP_GPIO_H */
