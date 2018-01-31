libsigrok-pcsu1000
------------------

This is a try to port the Velleman PCSU1000 Digital oscilloscope to Sigrok.

**Roadmap**

  - load the firmware to the FPGA (WIP)
  - understand the commands sent to the FPGA (WIP)
  - understand and parse the data we get from the FPGA (TODO)
  - display the data in Sigrok (TODO)
  - merge with Sigrok project (TODO)

**Firmware loading**
The firmware file `pcsu1000.bit` is extracted from the [Velleman library](https://www.velleman.eu/downloads/files/downloads/pcsu1000_dll_package.zip).
The FPGA is configured in Serial slave mode using the BitBang mode of the FTDI FT245BL. It uses a 74HC367 Hex buffer (4A-4Y and 6A-6Y unused) 

  - `PROG_B`: FT245BL `D1` pin (221R) (74HC367 1A-1Y i.e. pin2-pin3)
  - `CCLCK`: FT245BL `D0` pin (221R) (74HC367 5A-5Y i.e. pin12-pin11)
  - `INIT_B`: FT245BL `D3` pin (input) (74HC367 3A-3Y i.e. pin6-pin7)
  - `DIN`: FT245BL `D2` pin (74HC367 2A-2Y i.e. pin4-pin5)
  - `DONE`: drives `1OE` of 74HC367 (1 Output Enable is disabled when `DONE` is high)
  - `HSWAP_EN`: unplugged
  - `M2`, `M1`, `M0`: unplugged on the board (default serial slave mode)
