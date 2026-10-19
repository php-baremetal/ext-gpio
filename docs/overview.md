---
eyebrow: 'Package · Extension'
lede:    'Digital pin I/O from PHP: gpio_mode(), gpio_write(), gpio_read(). Neutral extension; conductor adds the GPIO HAL for your platform.'
see_also:
  - { href: './usage/php-api.md', meta: 'PHP API' }
  - { href: './recipes/blink-an-led.md', meta: 'Recipe' }
  - { href: 'package:php-baremetal/esp32-gpio-hal', meta: 'HAL', label: 'esp32-gpio-hal' }
  - { href: 'package:php-baremetal/ext-sys', meta: 'Extension', label: 'ext-sys' }
prev: { label: 'No previous page', href: '#' }
next: { label: 'Functions and constants', href: './usage/php-api.md' }
---

# GPIO

ext-gpio lets the PHP script on your board drive and read digital pins: switch an LED or a relay, read a button
or a jumper. A pin is either an input or an output; an output can still be read back.

## Requirements

| Requires | Why |
| --- | --- |
| `php-baremetal/gpio-hal` (virtual) | The platform side that touches the pins. On ESP32 it is `php-baremetal/esp32-gpio-hal` (platform `esp32`, over `esp_driver_gpio`). |

You never require the HAL yourself. Conductor reads the platform from `extra.baremetal.sdk` (`esp32@^1.0` gives
`esp32`) and adds `php-baremetal/esp32-gpio-hal` while Composer resolves. The extension's PHP module is `gpio`.

ext-gpio has no timing functions: the pauses in the examples (`delay()`, `sys_delay()`) come from
[ext-sys](package:php-baremetal/ext-sys).

## Install

<!-- @steps -->
- **Install conductor once per machine**

  `composer global require php-baremetal/conductor`

- **Create the project**

  `phpbm init my-blink` writes `composer.json` with conductor, the package registry, and the SDK, board, runtime
  and storage you pick.

- **Require the extensions**

  `composer require php-baremetal/ext-gpio php-baremetal/ext-sys`

- **Build and flash**

  `phpbm flash`, then `phpbm monitor` for the console.
<!-- @endsteps -->

The resulting `composer.json`, for a Waveshare ESP32-S3-Zero (N8R8) with an LED on GPIO 2:

<!-- @code-block language="json" label="composer.json" -->
```json
{
  "name": "acme/my-blink",
  "type": "project",
  "repositories": [
    { "type": "composer", "url": "https://repository.php-baremetal.com", "canonical": true }
  ],
  "require": {
    "php-baremetal/conductor": "^1.0",
    "php-baremetal/ext-gpio": "^1.0",
    "php-baremetal/ext-sys": "^1.0"
  },
  "config": {
    "allow-plugins": { "php-baremetal/conductor": true },
    "platform": { "php": "8.5.11" }
  },
  "extra": {
    "baremetal": {
      "sdk": "esp32@^1.0",
      "board": "waveshare-esp32-s3-zero-n8r8",
      "runtime": "init-loop",
      "storage": "embedded"
    }
  }
}
```
<!-- @endcode-block -->

## A first script

The `init-loop` runtime runs the script, then `setup()` once, then `loop($tick)` repeatedly.

<!-- @code-block language="php" label="project-src/index.php" -->
```php
<?php
const LED = 2;

function setup(): void
{
    gpio_mode(LED, GPIO_OUTPUT);
}

function loop(int $tick): void
{
    gpio_write(LED, $tick % 2);
    delay(500);
}
```
<!-- @endcode-block -->

## PHP API at a glance

| Name | Kind | Meaning |
| --- | --- | --- |
| `gpio_mode(int $pin, int $mode): void` | function | Make a pin an input or an output; resets the pin first. |
| `gpio_write(int $pin, int $level): void` | function | Drive an output low (0) or high (any other value). |
| `gpio_read(int $pin): int` | function | The pin's level, 0 or 1; works on outputs too. |
| `gpio_available(): bool` | function | Always `true`: the extension is in the firmware. |
| `GPIO_INPUT`, `GPIO_OUTPUT` | constants | 0 and 1, the values for `$mode`. |

No classes, no exceptions. Details in [Functions and constants](./usage/php-api.md).

## Dependencies

ext-gpio requires only the virtual `php-baremetal/gpio-hal`, provided on ESP32 by
[esp32-gpio-hal](package:php-baremetal/esp32-gpio-hal). It provides and registers nothing.

## Notes

- `$pin` is the chip's GPIO number, not a header position. It is not range-checked: a pin the chip does not
  have raises no PHP error: `gpio_mode()` and `gpio_write()` do nothing (the ESP-IDF driver logs an error on the
  console) and `gpio_read()` returns a meaningless value.
- `gpio_mode()` resets the pin every time, which also turns its internal pull-up on. An input therefore reads 1
  when nothing drives it, and a button to GND needs no external resistor. There is no way to select pull-down,
  open-drain or drive strength.
- Configure each pin once (in `setup()` or at the top of the script), not in every `loop()`.
- Pins used by the board itself (flash and PSRAM, the USB port, an on-board peripheral) must not be
  reconfigured; check the board's pinout.
- No edge interrupts, PWM or analog input here. For a callback on a falling edge use `watch_gpio()` from
  [ext-events](package:php-baremetal/ext-events); for addressable LEDs, [ext-led](package:php-baremetal/ext-led).
- The calls run directly on the PHP task; nothing goes through the executor, and the `power-save` option does
  not change them.
