---
eyebrow: 'ext-gpio · PHP API'
lede:    'Four global functions and two constants. None throws for a bad pin number: pass a GPIO the chip has.'
see_also:
  - { href: '../recipes/blink-an-led.md', meta: 'Recipe' }
  - { href: '../recipes/read-a-button.md', meta: 'Recipe' }
  - { href: 'package:php-baremetal/esp32-gpio-hal', meta: 'HAL', label: 'esp32-gpio-hal' }
prev: { label: 'Overview', href: '../overview.md' }
next: { label: 'Blink an LED', href: '../recipes/blink-an-led.md' }
---

# Functions and constants

All names are global. Arguments are strict `int`: passing a non-numeric string throws `\TypeError`, as for any
PHP function.

## Constants

| Constant | Value | Meaning |
| --- | --- | --- |
| `GPIO_INPUT` | `0` | Input mode for `gpio_mode()`. |
| `GPIO_OUTPUT` | `1` | Output mode for `gpio_mode()`; the pin stays readable. |

## gpio_mode()

<!-- @method name="gpio_mode(int $pin, int $mode): void" returns="void" visibility="public" -->

<!-- @params -->
<!-- @param name="$pin" type="int" required -->
The chip's GPIO number.
<!-- @endparam -->
<!-- @param name="$mode" type="int" required -->
`GPIO_INPUT` (0) or `GPIO_OUTPUT`. Any value other than 0 means output.
<!-- @endparam -->
<!-- @endparams -->

Resets the pin to its default state, then sets the direction. On ESP32 the reset selects the GPIO function,
disconnects any peripheral from the pin and enables the internal pull-up; output mode is input-and-output, so
`gpio_read()` returns what the pin is driving. `gpio_mode()` does not set the output level: call `gpio_write()`
right after it to start from a known state.

## gpio_write()

<!-- @method name="gpio_write(int $pin, int $level): void" returns="void" visibility="public" -->

<!-- @params -->
<!-- @param name="$pin" type="int" required -->
An output pin.
<!-- @endparam -->
<!-- @param name="$level" type="int" required -->
0 for low; any other value for high.
<!-- @endparam -->
<!-- @endparams -->

Sets the output level. On a pin configured as an input it has no visible effect.

## gpio_read()

<!-- @method name="gpio_read(int $pin): int" returns="int" visibility="public" -->

The current level of the pin, 0 or 1. Works on inputs and on outputs (it reads back the driven level).

## gpio_available()

<!-- @method name="gpio_available(): bool" returns="bool" visibility="public" -->

Always `true`. When ext-gpio is not in the firmware the function does not exist, so test for the extension
with `function_exists('gpio_available')` or `extension_loaded('gpio')`.

## Example

<!-- @code-block language="php" label="project-src/index.php" -->
```php
<?php
gpio_mode(2, GPIO_OUTPUT);
gpio_write(2, 1);
echo 'GPIO 2 reads ', gpio_read(2), "\n";   // 1: an output reads back its level

gpio_mode(4, GPIO_INPUT);
echo 'GPIO 4 reads ', gpio_read(4), "\n";   // 1 when nothing pulls it low (pull-up)
```
<!-- @endcode-block -->
