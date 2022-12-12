#pragma once

// simple lcd text output sample using Joy-IT Explorer 500
// uses 4 bit mode

// LCD has several pins:
// RS        PIN15_GPIO22
// STROBE    PIN23_GPIO11_SCKL
// D0        PIN16_GPIO23
// D1        PIN19_GPIO10_MOSI
// D2        PIN21_GPIO9_MISO
// D3        PIN22_GPIO25

// CAUTION: LCD conflicts with SPI modules

void doLCD();

void doLCD_I2C();
