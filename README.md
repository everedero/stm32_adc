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

## Pinout
### ADC Nucleo pin name

| ADC name     | Logical pin | Nucleo pin name |
|--------------|-------------|-----------------|
| ADC123\_IN0  | PA0         | D32             |
| ADC123\_IN1  | PA1         |                 |
| ADC123\_IN2  | PA2         |                 |
| ADC123\_IN3  | PA3         | A0              |
| ADC12\_IN4   | PA4         | D24             |
| ADC12\_IN5   | PA5         | D13             |
| ADC12\_IN6   | PA6         | D12             |
| ADC12\_IN7   | PA7         | D11             |
| ADC12\_IN8   | PB0         | D33             |
| ADC12\_IN9   | PB1         | A6              |
| ADC123\_IN10 | PC0         | A1              |
| ADC123\_IN11 | PC1         |                 |
| ADC123\_IN12 | PC2         | A7              |
| ADC123\_IN13 | PC3         | A2              |
| ADC12\_IN14  | PC4         |                 |
| ADC12\_IN15  | PC5         |                 |
| ADC3\_IN4    | PF6         |                 |
| ADC3\_IN5    | PF7         | D62             |
| ADC3\_IN6    | PF8         | D61             |
| ADC3\_IN7    | PF9         | D63             |
| ADC3\_IN8    | PF10        | A5              |
| ADC3\_IN9    | PF3         | A3              |
| ADC3\_IN14   | PF4         | A8              |
| ADC3\_IN15   | PF5         | A4              |

### Nucleo ADC pinout
<!--
                                                                         CN7

                                                                       +-------+
                                                                   D16 | 1 | 2 | D15
                                                                       |   |   |
                                                                   D17 | 3 | 4 | D14
                                                                       |   |   |
                                                                   D18 | 5 | 6 | AREF
                    CN8                                                |   |   |
                                                                   D19 | 7 | 8 | GND
                  +-------+                                            |   |   |
               NC | 1 | 2 | D43                                    D20 | 9 | 10| D13 (ADC12_IN5)
                  |   |   |                                            |   |   |
            IOREF | 3 | 4 | D44                                    D21 | 11| 12| D12 (ADC12_IN6)
                  |   |   |                                            |   |   |
            RESET | 5 | 6 | D45                                    D22 | 13| 14| D11 (ADC12_IN7)
                  |   |   |                                            |   |   |
              3V3 | 7 | 8 | D46                                    D23 | 15| 16| D10
                  |   |   |                                            |   |   |
               5V | 9 | 10| D47                        (ADC12_IN4) D24 | 17| 18| D9
                  |   |   |                                            |   |   |
              GND | 11| 12| D48                                    D25 | 19| 20| D8
                  |   |   |                                            |   |   |
              GND | 13| 14| D49                                        +-------+
                  |   |   |
              VIN | 15| 16| D50                                        +-------+
                  |   |   |                                      AVDD  | 1 | 2 | D7
                  +-------+                                            |   |   |
                                                                 AGND  | 3 | 4 | D6
                  +-------+                                            |   |   |
 (ADC123_IN3) A0  | 1 | 2 | D51                                   GND  | 5 | 6 | D5
                  |   |   |                                            |   |   |
(ADC123_IN10) A1  | 3 | 4 | D52                        (ADC12_IN9) A6  | 7 | 8 | D4
                  |   |   |                                            |   |   |
(ADC123_IN13) A2  | 5 | 6 | D53                      (ADC123_IN12) A7  | 9 | 10| D3
                  |   |   |                                            |   |   |
   (ADC3_IN9) A3  | 7 | 8 | D54                        (ADC3_IN14) A8  | 11| 12| D2
                  |   |   |                                            |   |   |
  (ADC3_IN15) A4  | 9 | 10| D55                                    D26 | 13| 14| D1
                  |   |   |                                            |   |   |
   (ADC3_IN8) A5  | 11| 12| GND                                    D27 | 15| 16| D0
                  |   |   |                                            |   |   |
              D72 | 13| 14| D56                                    GND | 17| 18| D42
                  |   |   |                                            |   |   |
              D71 | 15| 16| D57                                    D28 | 19| 20| D41
                  |   |   |                                            |   |   |
              D70 | 17| 18| D58                                    D29 | 21| 22| GND
                  |   |   |                                            |   |   |
              D69 | 19| 20| D59                                    D30 | 23| 24| D40
                  |   |   |                                            |   |   |
              D68 | 21| 22| D60                                    D31 | 25| 26| D39
                  |   |   |                                            |   |   |
              GND | 23| 24| D61 (ADC3_IN6)                         GND | 27| 28| D38
                  |   |   |                                            |   |   |
              D67 | 25| 26| D62 (ADC3_IN5)            (ADC123_IN0) D32 | 29| 30| D37
                  |   |   |                                            |   |   |
              D66 | 27| 28| D63 (ADC3_IN7)             (ADC12_IN8) D33 | 31| 32| D36
                  |   |   |                                            |   |   |
              D65 | 29| 30| D64                                    D34 | 33| 34| D35
                  |   |   |                                            |   |   |
                  +-------+                                            +-------+

                  CN9                                                  CN10
-->
![Kroki generated PlantUML](https://kroki.io/ditaa/svg/eNrNmMFu2zAMhu9-Ch1nFANMUpTkoxGlRS4e0A257kX68CXdxKGBJFVbqZsB3irp8--f_JU6V-nZzbHrKu318PPteaixYYbgXhxIoVQGrkX5cq46lFE2Iym_UPr_lDLJZiylmk7P-8fuuhfSP8YcZTMVVHGf5tzd8VgVynlnPeapiBIH-WMlhUE_ObkfU94B_j3M3Hd3Dv868eGXfDrrN-_LiPUVAaRQifFCHBoTP-9_7_8Y72XPZcT6QYCkvBLDhTg2JnaOjmRsmH0oI9ZFwFJBiYfWlI6P1oY-3tpgVc73Qqm2gSiVZNHYGlJ62DrPpzIp1S0wSlfqm6VvgjybzY818u7WYcfDbH3CQ9XDyraajjk7O_hi80H78WfSz2JnXWgA-dYbJM1BvZuGjSgMBXudINfxxvXNemGEQSBhIwrju20_yqLgNhOtKaQqiVtR6B7isghlUXR2oFGTttcj6SQKbURhf0_JBVIG6JScHWjYAnI9j-U8vxGFC9MzbNKzqZJJINmKsrRECWS0g7B5Xua4uVJwUaqfouGclx7bU8ImHmKZlMkGpof2lIOVhQtjXW2M4hLEW7f7upRhtLJwUa5n0ldDsQkul4H2vgzJyJLDUEa5xJTYBNUm9E0XuVWWAGv_h_6d7kGxCapNKLXXMlpZAq6U3F8PHknUTDoYUGxCS-7E9pTByhJopYz99QhPSqmZT2ITUptQaE_JVpZQ9kuT9FZCYhNSmxA3_xX02Uvg5TrdXftHyOg-_Ozkfta9AsRwInw=)
