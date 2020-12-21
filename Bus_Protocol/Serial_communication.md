## What is Serial communication?
In serial communication, data is in the form of binary pulses. In other words, we can say Binary One represents a logic HIGH or 5 Volts, and zero represents a logic LOW or 0 Volts. Serial communication can take many forms depending on the type of transmission mode and data transfer. The transmission modes are classified as Simplex, Half Duplex, and Full Duplex. There will be a source (also known as a sender) and destination (also called a receiver) for each transmission mode.

### _Simplex method_
```The Simplex method is a one-way communication technique.```

Only one client (either the sender or receiver is active at a time). If a sender transmits, the receiver can only accept. Radio and Television transmission are the examples of simplex mode.

### _Half Duplex mode_
```In Half Duplex mode, both sender and receiver are active but not at a time```

i.e. if a sender transmits, the receiver can accept but cannot send and vice versa. A good example is an internet. If a client (laptop) sends a request for a web page, the web server processes the application and sends back the information.

### _Full Duplex mode_
```both sender and receiver can transmit and receive at the same time.```

The Full Duplex mode is widely used communication in the world. Here both sender and receiver can transmit and receive at the same time. An example is your smartphone.

![transfer mode](https://www.codrey.com/wp-content/uploads/2017/09/Transmission-Modes-Serial-Communication.png)


## Difference between Serial and Parallel communication
Serial communication sends only one bit at a time. so, these require fewer I/O (input-output) lines. Hence, occupying less space and more resistant to cross-talk. The main advantage of serial communication is, the cost of the entire embedded system becomes cheap and transmits the information over a long distance. Serial transfer is used in DCE (Data communication Equipment) devices like a modem.

In parallel communication, a chunk of data (8,16 or 32 bit) is sent at a time. So, each bit of data requires a separate physical I/O line. The advantage of parallel communication is it is fast but its drawback is it use more number of I/O (input-output) lines. Parallel transfer is used in PC (personal computer) for interconnecting CPU (central processing unit), RAM (random access memory), modems, audio, video and network hardware.

![Serial Communication](https://www.codrey.com/wp-content/uploads/2017/09/Serial-Communication.png)

![Parallel Communication](https://www.codrey.com/wp-content/uploads/2017/09/RS232-Parallel-Communication.png)

Here is a chart to compare serial and parallel communication:

![Serial vs Parallel](images/serial_vs_parallel.png)

## Clock Synchronization
For efficient working of serial devices, the clock is the primary source. Malfunction of the clock may lead to unexpected results. The clock signal is different for each serial device, and it is categorized as synchronous protocol and asynchronous protocol.

### _Synchronous serial interface_
```All the devices on Synchronous serial interface use the single CPU bus to share both clock and data.```

 Due to this fact, data transfer is faster. The advantage is there will be no mismatch in baud rate. Moreover, fewer I/O (input-output) lines are required to interface components. 
 
 Examples:

    I2C, SPI etc.

 
### _Asynchronous serial interface_
```The asynchronous interface does not have an external clock signal```

It relies on four parameters namely

    1. Baud rate control
    2. Data flow control
    3. Transmission and reception control
    4. Error control
   
Asynchronous protocols are suitable for stable communication. These are used for long distance applications. 

Examples:

    RS-232, RS-422, and RS-485.


Asynchronous Protocols | Synchronous Protocols
-----------------------|----------------------
To move around the information at a longer distance | On-board communication
For more reliable data transfer | Faster data rate
---|---


## Serial communication Configurations
They are baud rate, data bit selection (framing), start-stop bit, and parity.

![Serial Configuration](https://www.codrey.com/wp-content/uploads/2017/09/Rules-of-Serial-Communication.png)

### _Baud rate_
```Baud rate is the speed of transferring data from the transmitter to a receiver in the form of bits per second.```

Some of the standard baud rates are 1200, 2400, 4800, 9600, 57600.

### _Framing_
```Framing shows how many data bits you want to send from the host device (Laptop) to mobile (receiver).```

Is it 5, 6, 7, or 8 bits? Mostly many devices, 8 bits are preferred. After selecting the8-bit data chunk, endianness has to be agreed by the sender and receiver.

### _Synchronization_
```Transmitter appends synchronization bits (1 Start bit and 1 or 2 Stop bit) to the original data frame.```

Synchronization bits help the receiver to identify the start and end of the data transfer. This process is known as asynchronous data transfer.

### _Error Control_
```Data corruption may happen due to external noise at the receiver end. The only solution to get the stable output is to check the Parity.```

If the binary data contains an even number of 1’s it is known as even parity and the Parity bit is set to ‘1’. If the binary data include an odd number of 1’s, it is called odd parity, and now parity bit is set to ‘0’.

## Links
[Serial Peripheral Interface basics](https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all)

[Serial Communication Basics](https://www.codrey.com/embedded-systems/serial-communication-basics/)
