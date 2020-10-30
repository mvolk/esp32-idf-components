# ESP32 ESP-IDF Prototyping Components

This repository draws together a variety of components for the ESP-IDF
development platform and ESP32 MCU. Each of these components lives
independently in its own repository, and it is from those repositories
that you should clone these components into your project(s).

This repository offers examples, intra-component integration tests, and
a central location for browsing the complete collection of ESP32
components I have to offer.

## TFT Test

The TFT Test is designed to assist in the verification of TFT configuration
settings. It displays two alternating modes, each for five seconds at a time.

In the first mode, the main screen should be black with a small (9x9) red
square 1 pixel from the upper left corner, a large (19x19) white square 1
pixel from the upper right corner, a small (9x9) blue square 1 pixel from the
lower right corner, a large (19x19) green square 1 pixel from the lower
left corner, and a large (19x19) red square in the center.

In the second mode, the main screen should be white. There should be 1 pixel
white space separating the four squares at each corner from the edges of the
screen. The dimensions of the squares should remain unchanged, but the
previously blue square should become red, the previously red squares should
become blue, and the previously white square should become black. The green
square should remain green.

![Mode 1 Example](https://github.com/mvolk/esp32-idf-components/blob/master/docs/tft_test_mode_1.png)

![Mode 2 Example](https://github.com/mvolk/esp32-idf-components/blob/master/docs/tft_test_mode_2.png)

There are several key things to watch for:

1. If there are any irregularities, such as "white noise" or streaks,
   then it's likely that the an offset is needed and/or that x and y need
   to be exchanged (effectively rotating the area being rendered by 90
   degrees).
2. If the center square is red when the background is white, and blue
   when the background is black, invert the RGB/BGR configuration.
3. If the upper left square color doesn't match the center square color,
   adjust x/y exchange and mirroring so that the two squares do match.
4. If the upper right square isn't white against black, and black against
   white, adjust x/y exchange and mirroring so that it is.
5. If the squares that should be green are reddish or blueish, ensure
   that the byte order of 2-byte 16-bit values is harmonized. If you
   store a 16-byte value in ESP32 memory, the least significant byte
   is counterintuitively ordered _before_ the most significant byte.
   If transmitted as a byte array, this will cause a deterministic
   but non-obvious mapping of expected color to actual color.
6. If colors aren't correct, and both BGR/RGB and byte order have been
   rules out as causal factors, adjust the gamma configuration.
