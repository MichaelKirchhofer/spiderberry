// the adc sample
// read values from adc0 and output them on dac0

// adc is i2c slave 0x48
// PCF 8591 8 bit ADC/DAC

void doI2CScan();

void checkDAC();

// adc ADS7830 is slave address 0x48
// 8 bit ADC
// channel 0 is battery2 (1/3 divided)
// channel 4 is battery1 (1/3 divided)
void checkBattery();