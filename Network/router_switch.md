
## Router vs Switch

```Router is network layer device while switches are link layer devices```

Some packet switches, called link-layer switches (examined
in Chapter 5), base their forwarding decision on values in the fields of the linklayer frame; switches are thus referred to as link-layer (layer 2) devices. Other packet switches, called routers, base their forwarding decision on the value in the networklayer field. Routers are thus network-layer (layer 3) devices. but must also implement layer 2 protocols as well, since layer 3 devices require the services of layer 2 to implement their (layer 3) functionality.

## Network layer

![network layer](images/network_layers.png)

The role of the network layer is thus deceptively simple—to move packets from a sending host to a receiving host. To do so, two important network-layer functions can be identified:

- ***Forwarding***. Forwarding refers to the router-local action of transferring a packet from an input link
interface to the appropriate output link interface.

When a packet arrives at a router’s input link, the router must move the packet to the appropriate output link. For example, a packet arriving from Host H1 to Router R1 must be forwarded to the next router on a path to H2.

- ***Routing***. Routing refers to the network-wide process that determines the end-to-end paths that packets take from source to destination.

The network layer must determine the route or path taken by packets as they flow from a sender to a receiver. The algorithms that calculate these paths are referred to as routing algorithms. A routing algorithm would determine, for example, the path along which packets flow from H1 to H2.

### Forwarding table

```Every router has a forwarding table.```

A router forwards a packet by examining the value of a field in the arriving packet’s header, and then using this header value to index into the router’s forwarding table. The value stored in the forwarding table entry for that header indicates the router’s outgoing link interface to which that packet is to be forwarded. Depending on the network-layer protocol, the header value could be the destination address of the packet or an indication of the connection to which the packet belongs.

![routing](images/routing.png)

## Reference 

