---
eyebrow: 'ext-gpio · Recipe'
lede:    'Mirror a push button to an LED: one input with the internal pull-up, one output, polled every 20 ms.'
see_also:
  - { href: '../usage/php-api.md', meta: 'PHP API' }
  - { href: './blink-an-led.md', meta: 'Recipe' }
  - { href: 'package:php-baremetal/ext-events', meta: 'Extension', label: 'ext-events' }
prev: { label: 'Blink an LED', href: './blink-an-led.md' }
next: { label: 'No next page', href: '#' }
---

# Read a button

## Wiring

On a Waveshare ESP32-S3-Zero (N8R8):

| Part | Between |
| --- | --- |
| LED and 330 ohm resistor in series | GPIO 2 and GND |
| Push button | GPIO 4 and GND |

No resistor on the button: `gpio_mode()` leaves the internal pull-up on, so GPIO 4 reads 1 when the button is
open and 0 while it is pressed.

## The project

The same `composer.json` as [Blink an LED](./blink-an-led.md): `ext-gpio` and `ext-sys`, runtime `init-loop`.

## The script

<!-- @code-block language="php" label="project-src/index.php" -->
```php
<?php
const LED = 2;
const BUTTON = 4;

function setup(): void
{
    gpio_mode(LED, GPIO_OUTPUT);
    gpio_mode(BUTTON, GPIO_INPUT);
    echo "mirroring GPIO " . BUTTON . " to GPIO " . LED . "\n";
}

function loop(int $tick): void
{
    $pressed = gpio_read(BUTTON) === 0;   // pulled low when pressed
    gpio_write(LED, $pressed ? 1 : 0);
    delay(20);
}
```
<!-- @endcode-block -->

## How it works

- Polling every 20 ms is fast enough for a hand and slow enough to ride over most contact bounce.
- For a count of presses, compare with the previous reading and act on the 1 to 0 transition only.
- To react to presses without polling, use the `event-driven` runtime and `watch_gpio()` from ext-events, which
  raises an event on a debounced falling edge.
