
```The Serial Peripheral Interface (SPI) is a synchronous serial communication interface specification used for short-distance communication, primarily in embedded systems. ```

## Key points

1.  Serial Peripheral Interface (SPI) is a synchronous data bus which means it use separate lines for data and clock. Four data lines: 
    * __MOSI__: Master to Slave Data Line (Master out slave in)
    * __MISO__: Slave to Master Data Line (Master in slave out)
    * __SS__: Slave Select
    * __SCK__: Clock

![SPi Data Transfer](./images/SPI_data_line.png)

2. Sample timing could be the rising (low to high) or falling (high to low) edge of the clock signal; the datasheet will specify which one to use. 
   
3. One reason that SPI is so popular is that the receiving hardware can be a simple shift register. This is a much simpler (and cheaper!) piece of hardware than the full-up UART (Universal Asynchronous Receiver / Transmitter) that asynchronous serial requires.

![shift register](https://i2.wp.com/aticleworld.com/wp-content/uploads/2019/11/SPIProtocolGIF.gif?resize=730%2C162&ssl=1)
   
4. In SPI, only one side generates the clock signal (usually called CLK or SCK for Serial ClocK). The side that generates the clock is called the "master", and the other side is called the "slave". Only __ONE MASTER__ but __MANY SLAVE__ allowed.
If the slave needs to send a response back to the master, the master will continue to generate a __prearranged__ number of clock cycles.

5. Because the master always generates the clock signal, it must know in advance __when__ a slave needs to return data and __how much data__ will be returned. This is very different than asynchronous serial, where random amounts of data can be sent in either direction at any time. 
   
6. SPI is "full duplex" (has separate send and receive lines), and, thus, in certain situations, you can transmit and receive data __at the same time__ (for example, requesting a new sensor reading while retrieving the data from the previous one). Your device's datasheet will tell you if this is possible. (_Half duplex three-line SPI is also possible_)
   
7. SS signal is usually __HIGH__ when the slave is not selected, when data needs to be transfer, SS line will be driven __LOW__, this is called active low logic.

![SPi Data Transfer with SS](./images/SPI_data_line_SS.png)

1. Only __ONE__ slave can be selected at a time, meaning one SS line driven low while others stays high at a time.

## Parameters to tune for SPI device programming

There are couple of parameters you would like to check in the datasheet and set properly before implementing any SPI routine:

1. __Endianness__: end data with the most-significant bit (MSB) first, or least-significant bit (LSB) first.
2. __Sampling timing__: The slave will read the data on either the rising edge or the falling edge of the clock pulse.
3. __Data Rate__: SPI can operate at extremely high speeds (millions of bytes per second), which may be too fast for some devices.

## Tips and tricks

- Because of the high speed signals, SPI should only be used to send data over short distances (up to a few feet). If you need to send data further than that, lower the clock speed, and consider using specialized driver chips.
- If things aren't working the way you think they should, a logic analyzer is a very helpful tool.

## SPI Bus 3-Wire and Multi-IO Configurations
In addition to the standard 4-wire configuration, the SPI interface has been extended to include a variety of IO standards including 3-wire for reduced pin count and dual or quad I/O for higher throughput.

In 3-wire mode, MOSI and MISO lines are combined to a single bidirectional data line as shown in Figure 3. Transactions are half-duplex to allow for bidirectional communication. Reducing the number of data lines and operating in half-duplex mode also decreases maximum possible throughput; many 3-wire devices have low performance requirements and are instead designed with low pin count in mind.

![3-wire](https://www.corelis.com/wp-content/uploads/2017/05/1-31.jpg)

Multi I/O variants such as dual I/O and quad I/O add additional data lines to the standard for increased throughput. Components that utilize multi I/O modes can rival the read speed of parallel devices while still offering reduced pin counts. This performance increase enables random access and direct program execution from flash memory (execute-in-place).

Quad I/O devices can, for example, offer up to 4 times the performance of a standard 4-wire SPI interface when communicating with a high speed device. Figure 4 shows an example of a single quad IO slave configuration.

![quad-io](https://www.corelis.com/wp-content/uploads/2017/05/2-41.jpg)

## SPI speed
There is no standard set of speeds defined for the SPI protocol. Typical bus speeds are in the **50 MHz** range. Since there is no overhead added by the protocol such as addressing and flow control, the throughput that can be achieved using SPI mirrors the clock frequency. For a 50 MHz SPI line, the throughput is 50 Mbps.

## SPI data transfer example
### Simple SPI write
Most SPI flash memories have a write status register command that writes one or two bytes of data, as shown in Figure 6. To write to the status register, the SPI host first enables the slave select line for the current device. The master then outputs the appropriate instruction followed by two data bytes that define the intended status register contents. Since the transaction does not need to return any data, the slave device keeps the MISO line in a high impedance state and the master masks any incoming data. Finally, slave select is de-asserted to complete the transaction.

![SPI write](https://www.corelis.com/wp-content/uploads/2017/05/1-51.jpg)

Using the Corelis SPI Exerciser command language, this transaction can be accomplished with the following code, where Data Byte 2 is “55” and Data Byte 1 is “AA”. For brevity we are using the short version of commands; the command script language supports both full commands such as “write, read” as well as abbreviated versions “wt, rd”. For example, the code for a write transaction below activates slave select, performs a write operation, then deactivates slave select:

    sson		// Activate slave select
    wt 01 55 AA	// Write instruction 01h and data bytes 55h, AAh
    ssoff		// Deactivate slave select

### Simple SPI read
A status register read transaction would be similar to the write transaction, but now takes advantage of data returned from the slave as shown in Figure 7. After sending the read status register instruction, the slave begins transmitting data on the MISO line at a rate of one byte per eight clock cycles. The host receives the bitstream and completes the transaction by de-asserting SS#.    

![SPI read](https://www.corelis.com/wp-content/uploads/2017/05/1-61.jpg)

This sequence is really a single-byte write followed by a two byte read; it can be considered a three-byte combined write/read command or a single byte write and two-byte read. We can create this transaction with the Corelis SPI Exerciser debugger command sequence:

    sson		// Activate slave select
    wt 05	// Write instruction 05h
    rd 2		// Read two data bytes
    ssoff		// Deactivate slave select


## Advantages of SPI:
- Full duplex communication in the default version of this protocol
- Push-pull drivers (as opposed to open drain) provide good signal integrity and high speed
- Higher throughput than I²C or SMBus. Not limited to any maximum clock speed, enabling potentially high speed
- Complete protocol flexibility for the bits transferred
    - Not limited to 8-bit words
    - Arbitrary choice of message size, content, and purpose
- Extremely simple hardware interfacing
    - Typically lower power requirements than I²C or SMBus due to less circuitry (including pull up resistors)
    - No arbitration or associated failure modes - unlike CAN-bus
    - Slaves use the master's clock and do not need precision oscillators
    - Slaves do not need a unique address – unlike I²C or GPIB or SCSI
    - Transceivers are not needed - unlike CAN-bus
- Uses only four pins on IC packages, and wires in board layouts or connectors, much fewer than parallel interfaces
- At most one unique bus signal per device (chip select); all others are shared
- Signals are unidirectional allowing for easy galvanic isolation
- Simple software implementation

## Disadvantages of SPI:
- Requires more pins on IC packages than I²C, even in the three-wire variant
- No in-band addressing; out-of-band chip select signals are required on shared buses
- Extensibility severely reduced when multiple slaves using different SPI Modes are required. Access is slowed down when master frequently needs to reinitialize in different modes.
- No hardware flow control by the slave (but the master can delay the next clock edge to slow the transfer rate)
- No hardware slave acknowledgment (the master could be transmitting to nowhere and not know it)
Typically supports only one master device (depends on device's hardware implementation)
- No error-checking protocol is defined
Without a formal standard, validating conformance is not possible
Only handles short distances compared to RS-232, RS-485, or CAN-bus. (Its distance can be extended with the use of transceivers like RS-422.)
- Opto-isolators in the signal path limit the clock speed for MISO transfer because of the added delays between clock and data
Many existing variations, making it difficult to find development tools like host adapters that support those variations
- SPI does not support hot swapping (dynamically adding nodes).
Interrupts must either be implemented with out-of-band signals or be faked by using periodic polling similarly to USB 1.1 and 2.0.
- Some variants like dual SPI, quad SPI, and three-wire serial buses defined below are half-duplex.

## Application
SPI is used to talk to a variety of peripherals, such as

  - Sensors: temperature, pressure, ADC, touchscreens, video game controllers
  - Control devices: audio codecs, digital potentiometers, DAC
  - Camera lenses: Canon EF lens mount
  - Communications: Ethernet, USB, USART, CAN, IEEE 802.15.4, IEEE 802.11, handheld video games
  - Memory: flash and EEPROM
  - Real-time clocks
  - LCD, sometimes even for managing image data
  - Any MMC or SD card (including SDIO variant[6])

## Example of bit-banging the master protocol
```c
/*
 * Simultaneously transmit and receive a byte on the SPI.
 *
 * Polarity and phase are assumed to be both 0, i.e.:
 *   - input data is captured on rising edge of SCLK.
 *   - output data is propagated on falling edge of SCLK.
 *
 * Returns the received byte.
 */
uint8_t SPI_transfer_byte(uint8_t byte_out)
{
    uint8_t byte_in = 0;
    uint8_t bit;

    for (bit = 0x80; bit; bit >>= 1) {
        /* Shift-out a bit to the MOSI line */
        write_MOSI((byte_out & bit) ? HIGH : LOW);

        /* Delay for at least the peer's setup time */
        delay(SPI_SCLK_LOW_TIME);

        /* Pull the clock line high */
        write_SCLK(HIGH);

        /* Shift-in a bit from the MISO line */
        if (read_MISO() == HIGH)
            byte_in |= bit;

        /* Delay for at least the peer's hold time */
        delay(SPI_SCLK_HIGH_TIME);

        /* Pull the clock line low */
        write_SCLK(LOW);
    }

    return byte_in;
}
```
## Example : Bit banking for sending a byte on an SPI bus.
```c
// transmit byte serially, MSB first
void send_8bit_serial_data(unsigned char data)
{
   int i;

   // select device (active low)
   output_low(SD_CS);

   // send bits 7..0
   for (i = 0; i < 8; i++)
   {
       // consider leftmost bit
       // set line high if bit is 1, low if bit is 0
       if (data & 0x80)
           output_high(SD_DI);
       else
           output_low(SD_DI);

       // pulse clock to indicate that bit value should be read
       output_low(SD_CLK);
       output_high(SD_CLK);

       // shift byte left so next bit will be leftmost
       data <<= 1;
   }

   // deselect device
   output_high(SD_CS);
}
```

## Reference 
[Serial Peripheral Interface basics](https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all)

https://www.corelis.com/education/tutorials/spi-tutorial/

https://en.wikipedia.org/wiki/Serial_Peripheral_Interface