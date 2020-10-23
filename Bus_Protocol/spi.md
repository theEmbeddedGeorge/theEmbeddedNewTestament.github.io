[Serial Peripheral Interface basics] (https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all)

## Key points

1.  SPI is a synchronous data bus which means it use separate lines for data and clock. Four data lines: 
    * __MOSI__: Master to Slave Data Line (Master out slave in)
    * __MISO__: Slave to Master Data Line (Master in slave out)
    * __SS__: Slave Select
    * __SCK__: Clock

![SPi Data Transfer](./Bus_Protocol\SPI_data_line.png)

2. Sample timing could be the rising (low to high) or falling (high to low) edge of the clock signal; the datasheet will specify which one to use. 
   
3. One reason that SPI is so popular is that the receiving hardware can be a simple shift register. This is a much simpler (and cheaper!) piece of hardware than the full-up UART (Universal Asynchronous Receiver / Transmitter) that asynchronous serial requires.
   
4. In SPI, only one side generates the clock signal (usually called CLK or SCK for Serial ClocK). The side that generates the clock is called the "master", and the other side is called the "slave". Only __ONE MASTER__ but __MANY SLAVE__ allowed.

5. Because the master always generates the clock signal, it must know in advance __when__ a slave needs to return data and __how much data__ will be returned. This is very different than asynchronous serial, where random amounts of data can be sent in either direction at any time. 
   
6. SPI is "full duplex" (has separate send and receive lines), and, thus, in certain situations, you can transmit and receive data __at the same time__ (for example, requesting a new sensor reading while retrieving the data from the previous one). Your device's datasheet will tell you if this is possible. (_Half duplex three-line SPI is also possible_)
   
7. SS signal is usually __HIGH__ when the slave is not selected, when data needs to be transfer, SS line will be driven __LOW__, this is called active low logic.

![SPi Data Transfer with SS](./Bus_Protocol\SPI_data_line_SS.png)

8. Only __ONE__ slave can be selected at a time, meaning one SS line driven low while others stays high at a time.

## Parameters to tune for SPI device programming

There are couple of parameters you would like to check in the datasheet and set properly before implementing any SPI routine:

1. __Endianness__: end data with the most-significant bit (MSB) first, or least-significant bit (LSB) first.
2. __Sampling timing__: The slave will read the data on either the rising edge or the falling edge of the clock pulse.
3. __Data Rate__: SPI can operate at extremely high speeds (millions of bytes per second), which may be too fast for some devices.

## Tips and tricks

- Because of the high speed signals, SPI should only be used to send data over short distances (up to a few feet). If you need to send data further than that, lower the clock speed, and consider using specialized driver chips.
- If things aren't working the way you think they should, a logic analyzer is a very helpful tool.

### Advantages of SPI:
- It's faster than asynchronous serial
- The receive hardware can be a simple shift register
- It supports multiple slaves

### Disadvantages of SPI:
- It requires more signal lines (wires) than other communications methods
- The communications must be well-defined in advance (you can't send random amounts of data whenever you want)
- The master must control all communications (slaves can't talk directly to each other)
- It usually requires separate SS lines to each slave, which can be problematic if numerous slaves are needed.




