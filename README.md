# RP2040 I2C Detect (Arduino)

This sketch turns a Raspberry Pi Pico (or any RP2040 board) into an I²C bus scanner that prints results in the same grid format as Linux `i2cdetect` from the i2c-tools package. It is handy when you need to confirm wiring, spot missing pull-ups, or double-check device addresses without leaving the Arduino IDE.

## Features

- Mimics the classic `i2cdetect` table layout, including address grid and summary line
- Automatically scans the bus every three seconds (adjustable)
- Configurable first/last address range and I²C pin assignments
- Works with the standard Arduino `Wire` library on RP2040 boards

## Hardware

- Raspberry Pi Pico / Pico W / other RP2040 board
- Two 4.7 kΩ (typical) pull-up resistors to 3.3 V on SDA and SCL
- Target I²C devices wired to:
  - `SDA` → GP12
  - `SCL` → GP13

> **Note:** The RP2040’s internal pull-ups are too weak for most I²C setups. Use external resistors unless you are doing a very short, low-speed test on the bench.

If your board exposes different default pins, change the `Wire.setSDA()` and `Wire.setSCL()` calls in the sketch to match your wiring.

## Getting Started

1. Install the **Arduino Mbed OS RP2040** core (Tools → Board → Boards Manager → search for “rp2040”).
2. Open `i2cdetect.ino` in the Arduino IDE.
3. Select your RP2040 board and its serial port.
4. Click **Upload**.
5. Open the Serial Monitor at `115200` baud.

You should see the banner followed by the 8×16 address grid. The sketch rescans automatically; you can reset the board or press the IDE’s Serial Monitor reset to trigger the initial scan again.

## Configuration

| Constant | Default | Description |
|----------|---------|-------------|
| `kScanIntervalMs` | `3000` | Milliseconds between scans. Reduce for faster updates or increase to ease bus traffic. |
| `kFirstUsable` | `0x03` | Lowest address to probe; mirrors Linux `i2cdetect`. |
| `kLastUsable` | `0x77` | Highest address to probe. |

To scan a different I²C instance or pin pair, change the pin numbers passed to `Wire.setSDA()` and `Wire.setSCL()` before `Wire.begin()`.

## Sample Output

```
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- 11 -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- 3b -- 3d -- 3f
40: -- -- -- -- -- -- -- -- -- -- 4a -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- 73 -- -- -- -- -- -- -- -- -- -- -- --
Found 6 devices at: 0x11 0x3b 0x3d 0x3f 0x4a 0x73
```

Addresses shown are examples; your output will reflect the devices connected to the bus.

## Troubleshooting

- **Nothing appears in the grid:** Confirm SDA/SCL wiring, power, and ensure you have proper pull-ups.
- **Only `--` entries:** Double-check that the bus voltage matches your devices (3.3 V) and that they are powered on.
- **Spurious addresses:** Long wires or weak pull-ups can cause noise; lower scan frequency or improve the hardware setup.

## License

This project is released into the public domain. Modify and use it however you like.
