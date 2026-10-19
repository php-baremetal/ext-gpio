# ext-gpio

Digital pin I/O for the PHP that runs on your board: make a pin an input or an output, drive it high or low,
read its level. Four global functions, `gpio_mode()`, `gpio_write()`, `gpio_read()` and `gpio_available()`,
and two constants, `GPIO_INPUT` and `GPIO_OUTPUT`.

ext-gpio is platform-neutral: it requires the virtual `php-baremetal/gpio-hal`, and conductor adds the HAL for
your SDK platform (`php-baremetal/esp32-gpio-hal` on ESP32).

## Install

In a project created with `phpbm init`:

```bash
composer require php-baremetal/ext-gpio
```

The examples pause with `delay()`, which comes from ext-sys:

```bash
composer require php-baremetal/ext-sys
```

## Example

An LED with a 330 ohm resistor between GPIO 2 and GND, in the `init-loop` runtime:

```php
<?php
const LED = 2;

function setup(): void
{
    gpio_mode(LED, GPIO_OUTPUT);
}

function loop(int $tick): void
{
    gpio_write(LED, $tick % 2); // on for odd ticks, off for even ones
    delay(500);                 // ext-sys
}
```

## Documentation

- [Overview](docs/overview.md): requirements, install, a first project, notes.
- [PHP API](docs/usage/php-api.md): every function and constant.
- [Blink an LED](docs/recipes/blink-an-led.md) and [Read a button](docs/recipes/read-a-button.md)

## License

MIT.
