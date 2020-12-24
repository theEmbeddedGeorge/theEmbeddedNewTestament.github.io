```The Inter-Integrated Circuit (I2C) Protocol is a protocol intended to allow multiple "peripheral" digital integrated circuits ("chips") to communicate with one or more "controller" chips.```

## Intro

The Inter-Integrated Circuit (I2C) Protocol is a protocol intended to allow multiple "peripheral" digital integrated circuits ("chips") to communicate with one or more "controller" chips. Like the Serial Peripheral Interface (SPI), it is only intended for short distance communications within a single device. Like Asynchronous Serial Interfaces (such as RS-232 or UARTs), it only requires two signal wires to exchange information.

![I2C](https://cdn.sparkfun.com/assets/learn_tutorials/8/2/I2C-Block-Diagram.jpg)

## Why Use I2C?
### What's Wrong with Serial UART Ports?
Because serial ports are asynchronous (no clock data is transmitted), devices using them must agree ahead of time on a data rate. The two devices must also have clocks that are close to the same rate, and will remain so--excessive differences between clock rates on either end will cause garbled data.

Asynchronous serial ports require hardware overhead--the UART at either end is relatively complex and difficult to accurately implement in software if necessary. At least one start and stop bit is a part of each frame of data, meaning that 10 bits of transmission time are required for each 8 bits of data sent, which eats into the data rate.

Another core fault in asynchronous serial ports is that they are inherently suited to communications between two, and only two, devices. While it is possible to connect multiple devices to a single serial port, bus contention (where two devices attempt to drive the same line at the same time) is always an issue and must be dealt with carefully to prevent damage to the devices in question, usually through external hardware.

Finally, data rate is an issue. While there is no theoretical limit to asynchronous serial communications, most UART devices only support a certain set of fixed baud rates, and the highest of these is usually around 230400 bits per second.

In summary:

- Asynchronous serial communication require both side have the same clock rate and there must not be any excessive difference clock rates or it will cause garbled data.
- Only point to point communication, no multiple load support.
- Hardware is complex and hence more expensive.
- Uart also eats bit transmitted: out of 10 bits send, at least 2 bits is not data.
- Data rate is slow.

### What's Wrong with SPI?
The most obvious drawback of SPI is the number of pins required. Connecting a single controller [1] to a single peripheral [1] with an SPI bus requires four lines; each additional peripheral device requires one additional chip select I/O pin on the controller. The rapid proliferation of pin connections makes it undesirable in situations where lots of devices must be connected to one controller. Also, the large number of connections for each device can make routing signals more difficult in tight PCB layout situations.

SPI only allows one controller on the bus, but it does support an arbitrary number of peripherals (subject only to the drive capability of the devices connected to the bus and the number of chip select pins available).

SPI is good for high data rate full-duplex (simultaneous sending and receiving of data) connections, supporting clock rates upwards of 10MHz (and thus, 10 million bits per second) for some devices, and the speed scales nicely. The hardware at either end is usually a very simple shift register, allowing easy implementation in software.

In summary:

- Too many pins are used. Every addtional slave devices will require addtional SS wire.
- Only one master/controller allowed.
- Not support hot swap.

### I2C, the best of two worlds
I2C requires a mere two wires, like asynchronous serial, but those two wires can support up to 1008 peripheral devices. Also, unlike SPI, I2C can support a multi-controller system, allowing more than one controller [1] to communicate with all peripheral [1] devices on the bus (although the controller devices can't talk to each other over the bus and must take turns using the bus lines).

Data rates fall between asynchronous serial and SPI; most I2C devices can communicate at 100kHz or 400kHz. There is some overhead with I2C; for every 8 bits of data to be sent, one extra bit of meta data (the "ACK/NACK" bit, which we'll discuss later) must be transmitted.

The hardware required to implement I2C is more complex than SPI, but less than asynchronous serial. It can be fairly trivially implemented in software.

In summary:

- Only require two wires.
- Multi-controller system, allow multiple master and multiple slaves. If address is 7-bits, it allows 128 slave devices.
- Speed can be fast with different modes, only 1 bit is wasted (ACK/NACK).
- Hardware complxity is more than SPI but less than Uart.

## I2C hardware

```I2C bus drivers are "open drain", meaning that they can pull the corresponding signal line low, but cannot drive it high```

Each I2C bus consists of two signals: SCL and SDA. SCL is the clock signal, and SDA is the data signal. The clock signal is always generated by the current bus controller; some peripheral devices may force the clock low at times to delay the controller sending more data (or to require more time to prepare data before the controller attempts to clock it out). This is called "clock stretching" and is described on the protocol page.

Unlike UART or SPI connections, the I2C bus drivers are ***"open drain"***, meaning that they can pull the corresponding signal line low, but cannot drive it high. Thus, there can be no bus contention where one device is trying to drive the line high while another tries to pull it low, eliminating the potential for damage to the drivers or excessive power dissipation in the system. Each signal line has a ***pull-up resistor*** on it, to restore the signal to high when no device is asserting it low.

![I2C diagram](https://cdn.sparkfun.com/assets/learn_tutorials/8/2/I2C_Schematic.jpg)

Resistor selection varies with devices on the bus, but a good rule of thumb is to start with 4.7kΩ resistor and adjust down if necessary. I2C is a fairly robust protocol, and can be used with short runs of wire (2-3m). For long runs, or systems with lots of devices, smaller resistors are better.

    Rule of thump pull-up resistor choice:   4.7kΩ

Here is a good link about how to choose capacitance and pull-upresistance value and the reason behind it:

[I2C typical setup](https://www.i2c-bus.org/i2c-primer/typical-i2c-bus-setup/)

## Clock, Stretching, Arbitration
### Clock Generation
The SCL clock is always generated by the I2C master. The specification requires minimum periods for the low and high phases of the clock signal. Hence, the actual clock rate may be lower than the nominal clock rate e.g. in I2C buses with large rise times due to high capacitances.

### Clock Stretching
I2C devices can slow down communication by stretching SCL: During an SCL low phase, any I2C device on the bus may additionally hold down SCL to prevent it from rising again, enabling it to slow down the SCL clock rate or to stop I2C communication for a while. This is also referred to as clock synchronization.

Note: The I2C specification does not specify any timeout conditions for clock stretching, i.e. any device can hold down SCL *as long as it likes*.

### Arbitration
Several I2C multi-masters can be connected to the same I2C bus and operate concurrently. By constantly monitoring SDA and SCL for start and stop conditions, they can determine whether the bus is currently idle or not. If the bus is busy, masters delay pending I2C transfers until a stop condition indicates that the bus is free again.

However, it may happen that two masters start a transfer at the same time. During the transfer, the masters constantly monitor SDA and SCL. If one of them detects that SDA is low when it should actually be high, it assumes that another master is active and immediately stops its transfer. This process is called arbitration.

## Protocol
```Two kinds of frames: address frame and data frame.```

Messages are broken up into two types of frame: an ***address frame***, where the controller indicates the peripheral to which the message is being sent, and one or more ***data frames***, which are 8-bit data messages passed from controller to peripheral or vice versa. Data is placed on the SDA line after **SCL goes low, and is sampled after the SCL line goes high**. The time between clock edge and data read/write is defined by the devices on the bus and will vary from chip to chip.

![protocol](https://cdn.sparkfun.com/assets/learn_tutorials/8/2/I2C_Basic_Address_and_Data_Frames.jpg)

Data is sampled when SCL is high, so SDA line must be stable when SCL is HIGH. SDA is allowed to change when SCL is low:

![I2C sampling](images/I2C_sampling.png)

### Start and Stop Condition
- All transactions begin with a **START (S)** and are terminated by a **STOP (P)**
- A **HIGH to LOW** transition on the SDA line while SCL is **HIGH** defines a START condition.
- A **LOW to HIGH** transition on the SDA line while SCL is HIGH defines a **STOP** condition.

![I2C start & stop](images/I2C_start_stop.png)

START and STOP conditions are always generated by the master. The bus is considered to be busy after the START condition. The bus is considered to be free again a certain time after the STOP condition.

The bus stays busy if a repeated START (Sr) is generated instead of a STOP condition. In this respect, the START (S) and repeated START (Sr) conditions are functionally identical.

### Byte format
```Every byte put on the SDA line must be eight bits long.```

The number of bytes that can be transmitted per transfer is unrestricted. Each byte must be followed by an Acknowledge bit. Data is transferred with the Most Significant Bit (**MSB**) first (see Figure 6). If a slave cannot receive or transmit another complete byte of data until it has performed some other function, for example servicing an internal interrupt, it can hold the clock line SCL LOW to force the master into a wait state ***(clock stretching)***. Data transfer then continues when the slave is ready for another byte of data and releases clock line SCL.

![Byte format](images/I2C_byte_format.png)

### Acknowledge (ACK) and Not Acknowledge (NACK)
```The acknowledge takes place after every byte.```

The acknowledge bit allows the receiver to signal the transmitter that the byte was successfully received and another byte may be sent. The master generates all clock pulses, including the acknowledge ninth clock pulse.

The Acknowledge signal is defined as follows: **the transmitter releases the SDA line during the acknowledge clock pulse so the receiver can pull the SDA line LOW and it remains stable LOW during the HIGH period of this clock pulse**. Set-up and hold times must also be taken into account.

```When SDA remains HIGH during this ninth clock pulse, this is defined as the Not Acknowledge signal.```

The master can then generate either a STOP condition to abort the transfer, or a repeated START condition to start a new transfer. There are five conditions that lead to the generation of a NACK:

1. No receiver is present on the bus with the transmitted address so there is no device to respond with an acknowledge.
2. The receiver is unable to receive or transmit because it is performing some real-time
function and is not ready to start communication with the master.
3. During the transfer, the receiver gets data or commands that it does not understand.
4. During the transfer, the receiver cannot receive any more data bytes.
5. A master-receiver must signal the end of the transfer to the slave transmitter.

According to condition five, NACK is sometimes send at the end of transmission (slave not drive SDA line low to send ACK) and master will send stop condition to end the transmission.

### Clock synchronization

```A synchronized SCL clock is generated with its LOW period determined by the master with the longest clock LOW period, and its HIGH period determined by the one with the shortest clock HIGH period.```

Two masters can begin transmitting on a free bus at the same time and there must be a method for deciding which takes control of the bus and complete its transmission. This is done by clock synchronization and arbitration. **In single master systems, clock synchronization and arbitration are not needed.**

Clock synchronization is performed using the wired-AND connection of I2C interfaces to the SCL line. This means that a HIGH to LOW transition on the SCL line causes the masters concerned to start counting off their LOW period and, once a master clock has gone LOW, it holds the SCL line in that state until the clock HIGH state is reached (see Figure 7). However, if another clock is still within its LOW period, the LOW to HIGH transition of this clock may not change the state of the SCL line. The SCL line is therefore held LOW by the master with the longest LOW period. Masters with shorter LOW periods
enter a HIGH wait-state during this time.

![Clock Sync](images/I2C_clock_sync.png)

When all masters concerned have counted off their LOW period, the clock line is released and goes HIGH. There is then no difference between the master clocks and the state of the SCL line, and all the masters start counting their HIGH periods. The first master to complete its HIGH period pulls the SCL line LOW again.

### Arbitration

```Arbitration, like synchronization, refers to a portion of the protocol required only if more than one master is used in the system.```

Slaves are not involved in the arbitration procedure. A master may start a transfer only if the bus is free. Two masters may generate a START condition within the minimum hold time (tHD;STA) of the START condition which results in a valid START condition on the bus. Arbitration is then required to determine which master will complete its transmission.

![i2c Arbitration](images/I2C_arbitration.png)

### Clock stretching
```Clock stretching pauses a transaction by holding the SCL line LOW. The transaction cannot continue until the line is released HIGH again.```

Clock stretching is optional and in fact, most slave devices do not include an SCL driver so they are unable to stretch the clock.

A device may be able to receive bytes of data at a fast rate, but needs more time to store a received byte or prepare another byte to be transmitted. Slaves can then hold the SCL line LOW after reception and acknowledgment of a byte to force the master into a wait state until the slave is ready for the next byte transfer in a type of handshake procedure 

### Addressing

```After the START condition (S), a slave address is sent. This address is seven bits long followed by an eighth bit which is a data direction bit (R/W) — a ‘zero’ indicates a transmission (WRITE), a ‘one’ indicates a request for data (READ) (refer to Figure 10).```

A data transfer is always terminated by a **STOP condition (P) generated by the master**. However, if a master still wishes to communicate on the bus, it can generate a **repeated START condition (Sr) and address another slave without first generating a STOP condition**. Various combinations of read/write formats are then possible within such a transfer.

![I2C data transfer](images/I2C_data_transfer.png)

![First byte](images/I2C_1st_byte.png)

Possible transfer formats:
- ***Master-transmitter transmits to slave-receiver.*** The transfer direction is not changed. The slave receiver acknowledges each byte.

![format1](images/I2C_transfer_figure_1.png)

- ***Master reads slave immediately after first byte.*** At the moment of the first acknowledge, the master-transmitter becomes a master-receiver and the slave-receiver becomes a slave-transmitter. This first acknowledge is still generated **by the slave**. The **master generates subsequent acknowledges**. The **STOP condition is generated by the master**, which sends a **not-acknowledge (A) just before the STOP condition**.

![format2](images/I2C_transfer_figure_2.png)

- ***Combined format.*** During a change of direction within a transfer, the START condition and the slave address are both repeated, but with the R/W bit reversed. If a master-receiver sends a repeated START condition, it sends a not-acknowledge (A) just before the repeated START condition.

![format3](images/I2C_transfer_figure_3.png)

### 10-bit addressing
```The 10-bit slave address is formed from the first two bytes following a START condition (S) or a repeated START condition (Sr).```

The first seven bits of the first byte are the combination 1111 0XX of which the last two bits (XX) are the two Most-Significant Bits (MSB) of the 10-bit address; the eighth bit of the first byte is the R/W bit that determines the direction of the message.

![10-bit-transfer](images/I2C_10bit_address_transfer.png)

## Reference
[I2C Spec NXP](https://www.nxp.com/docs/en/user-guide/UM10204.pdf))

https://learn.sparkfun.com/tutorials/i2c/all

https://www.edn.com/design-calculations-for-robust-i2c-communications/

https://www.cnblogs.com/Tangledice/p/7622794.html

https://www.i2c-bus.org/

