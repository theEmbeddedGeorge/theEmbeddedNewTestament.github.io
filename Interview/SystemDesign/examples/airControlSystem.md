I am not sure about above but I'd probably work from top to down and try to organize as much info as I can however time is limit in basically refining a design.

## Players 

1. airplane in the sky
2. air traffic control unit on the ground

### Airplane -
It has many components, but important one here are two

- pilot - one who is displacing the plane or changing its position
- air traffic control unit - the one that communicates with air traffic control system on the ground
  
***Pilot -***
It has couple of actions

- register - sends initial message to new air traffic control on the ground

- deregister - send final message to old air traffic control on the ground

- steer - move plane (but not relevant to this i suppose)

***Air Traffic Control Communication -***
- updates its location - this happens either in loop continuously
- send message to air traffic control unit on ground this happens again periodically in loop
- receive messages from air traffic control unit and send those back to pilot - happens periodically in loop (messages are either simple pings or some message)

### On the ground -

***Controller***

- subscribes for registration / deregistration / urgent messages on global queue
- receive registration, send registration ack
- receive de-registration, send re-registration ack
- In response to registration spawn plane specific thread handling object
( one can discuss thread pool, database/data structure to maintain location and other info of plane)

***Worker:***

- subscribes to messages on plane's queue
- subscribes to messages on a global queue
- update location database in response to periodic location update
- notifies registration message and exit

***Communication:***
- A backend of radio. Implements message passing
- Receives and collects all messages and distributes them
- Messages are either registration, deregistration, urgent and location update
- Messages are put in different queues , each worker thread create its own queue when it registers for messages and subscribes for it.

***Control Unit on Plane***
- subscribes for urgent messages from controller
- periodically sends location update
- Urgent messages directed to Pilot class
- unsubscribe from controller

***Pilot***

- Sends registration, in response to ack, trigger Control Unit on Plane subscribe to new Controller
- Sends deregistration, in response to ack, trigger Control Unit on Plane unsubscribe from old Controller
- One can discuss message queue, subscription mechanism of IPC etc, DB, interfaces to GUI (if that counts)