## Freertos Firmware Stack

### Firmware Stack Layer diagram

![Firmware stack](../images/FreeRTOS_firmware_stack.png)

- User code is able to access the same FreeRTOS API, regardless of the underlying hardware port implementation.
- FreeRTOS does __NOT__ prevent __User code__ from using vendor-supplied drivers, CMSIS or raw hardware register.