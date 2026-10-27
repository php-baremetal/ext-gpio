---
eyebrow: 'ext-gpio · Recipe'
lede:    'Blink SOS in Morse code on an external LED: one output pin, gpio_write() and delay().'
see_also:
  - { href: '../usage/php-api.md', meta: 'PHP API' }
  - { href: './read-a-button.md', meta: 'Recipe' }
  - { href: 'package:php-baremetal/ext-sys', meta: 'Extension', label: 'ext-sys' }
prev: { label: 'Functions and constants', href: '../usage/php-api.md' }
next: { label: 'Read a button', href: './read-a-button.md' }
---

# Blink an LED

## Wiring

An LED and a 330 ohm resistor in series between GPIO 2 and GND, on a Waveshare ESP32-S3-Zero (N8R8). The long
leg of the LED goes towards GPIO 2. Any free GPIO works; change `LED` to match.

<!-- @callout variant="note" title="The on-board LED" -->
The RGB LED on the Waveshare ESP32-S3-Zero is an addressable WS2812, not a plain LED: `gpio_write()` cannot
light it. Use ext-led for it.
<!-- @endcallout -->

## The project

<!-- @code-block language="json" label="composer.json" -->
```json
{
  "name": "acme/blink-sos",
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
      "sdk": "esp32@^2.0",
      "board": "waveshare-esp32-s3-zero-n8r8",
      "runtime": "init-loop",
      "storage": "embedded"
    }
  }
}
```
<!-- @endcode-block -->

## The script

<!-- @code-block language="php" label="project-src/index.php" -->
```php
<?php
const LED = 2;
const UNIT = 200;   // ms; a dot is one unit, a dash three

function pulse(int $units): void
{
    gpio_write(LED, 1);
    delay(UNIT * $units);
    gpio_write(LED, 0);
    delay(UNIT);        // gap between symbols
}

function setup(): void
{
    gpio_mode(LED, GPIO_OUTPUT);
    gpio_write(LED, 0);
    echo "blinking SOS on GPIO " . LED . "\n";
}

function loop(int $tick): void
{
    foreach ([1, 1, 1, 3, 3, 3, 1, 1, 1] as $units) {   // S O S
        pulse($units);
    }
    delay(UNIT * 6);    // longer gap before repeating
}
```
<!-- @endcode-block -->

## How it works

- `setup()` runs once: the pin becomes an output and starts low.
- `loop()` runs again as soon as it returns (the runtime's `loop-interval` option defaults to 0); the timing
  comes entirely from `delay()`, which suspends the PHP task instead of spinning.
- With the `power-save` option on, the board can drop into light sleep during each `delay()`.
