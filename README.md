# ADC sample

```
BOARD="nucleo_f756zg"
west build -b $BOARD -p always adc_dt
```

* Enabling ADC shell

adc@40012000

3 ADC, 16 shared ext channels (datasheet)

Analog-to-digital converter (ADC)
RM0385
Page 458

0x4001 2000 - 0x4001 23FF ADC1 - ADC2 - ADC3

uart:~$ devmem 40012008
0x401
0b1 = ADON: ADC on


Note: Once the DAC channelx is enabled, the corresponding GPIO pin (PA4 or PA5) is
automatically connected to the analog converter output (DAC_OUTx). In order to avoid
parasitic consumption, the PA4 or PA5 pin should first be configured to analog (AIN).

ADC_CR1 (offset 0x04)
Bits 4:0 AWDCH[4:0]: Analog watchdog channel select bits
(chann 0 to 31)

Analog GPIOx
0x40020000
- Input: 0b00

devmem 40020000 32

Chan 2 and 3: analog mode (0b11)

Channel 3 is pin A0. WHY?
Because pins in STM32 Nucleo do not match logical pins.
CN9 - A0 is logical PA3.
