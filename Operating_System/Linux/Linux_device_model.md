## Linux Device Model

### Small Piece of the Device model

![Small piece](../images/small_piece_device_model.png)

### Architecture Diagram

![Architecture](../images/linux_device_model_architecture.jpg)

### Example

#### I2C Charger Example

![Example1](../images/linux_device_model_example.png)

![Example2](../images/linux_device_model_example_2.png)

![Example3](../images/linux_device_model_example_3.png)

#### Who calls i2c_new_device?

![Example4](../images/linux_device_model_example_4.png)

### Device Tree dtsi file

![dtsi](../images/dtsi_example.png)

![dtsi2](../images/dtsi_example_2.png)

![dtsi_parse](../images/dtsi_parse.png)

Device node building Process:

    of_platform_populate
        -> of_platform_bus_create
            -> of_platform_device_create_pdata
                -> of_device_add
		            ->device_add
                        ->bus_probe_device


### Reference

_Linux Device Drivers, 3rd Edition_

http://www.slideshare.net/jserv/linux-discovery

http://m.blog.chinaunix.net/uid-29955651-id-5095220.html

http://www.cs.fsu.edu/~baker/devices/notes/ch14.html#(1)


