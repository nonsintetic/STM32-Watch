# STM32-Watch
an STM32 based smart watch

# Hardware
At the moment I use a cheap STM32F103C8T6 development board from eBay. Look for 'STM32F103C8T6 development board' if you want to find a similar one. The setup also uses the Adafruit SHARP Memory Display Breakout as a display and an [HM-11 Bluetooth Module](http://wiki.seeedstudio.com/wiki/Bluetooth_V4.0_HM-11_BLE_Module) for communication with an Android based phone (the Android app doesn't exist yet, for testing I use the NRF Toolkit from Nordic).

# Connections
Left is the STM32 board pin, right is the module pin. I left out the power connections, generally connect everything to 3.3v and ground. The buttons have one pin connected to 3.3v and the other one is listed below, they use internal pull-downs.

**Display:**

A5 > CLK (spi clock)

A7 > DI (spi mosi)

A4 > CS (spi chip select)

**HM11 bluetooth module:**

A10 > UART_TX (serial)

A9 > UART_RX (serial)

**3 Buttons:**

B8 (button 1 = menu button)

B9 (button 2 = left)

B10 (button 3 = right)

**Battery level pin (optional)**

A1 (connect to a voltage divider if you have a battery that goes over 3.3volts when charged fully, I use two 1k resistors for my li-po)
