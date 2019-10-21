<!----- Conversion time: 3.646 seconds.


Using this Markdown file:

1. Cut and paste this output into your source file.
2. See the notes and action items below regarding this conversion run.
3. Check the rendered output (headings, lists, code blocks, tables) for proper
   formatting and use a linkchecker before you publish this page.

Conversion notes:

* Docs to Markdown version 1.0β17
* Mon Oct 21 2019 16:19:30 GMT-0700 (PDT)
* Source doc: https://docs.google.com/open?id=1TkMEhGpZMDMVQgV45Pd8Kv3XPtj0hTOHslndL_6t-Ao
* This is a partial selection. Check to make sure intra-doc links work.

WARNING:
You have 3 H1 headings. You may want to use the "H1 -> H2" option to demote all headings by one level.

----->


<p style="color: red; font-weight: bold">>>>>>  gd2md-html alert:  ERRORs: 0; WARNINGs: 1; ALERTS: 0.</p>
<ul style="color: red; font-weight: bold"><li>See top comment block for details on ERRORs and WARNINGs. <li>In the converted Markdown or HTML, search for inline alerts that start with >>>>>  gd2md-html alert:  for specific instances that need correction.</ul>

<p style="color: red; font-weight: bold">Links to alert messages:</p>
<p style="color: red; font-weight: bold">>>>>> PLEASE check and correct alert issues and delete this message and the inline alerts.<hr></p>



# Week 1


## Video 1: Networks, the Internet, and Units

We will analyze networks and the Internet in terms of...



*   Transmission technologies (data and packet transmission)
*   Communication protocols
*   Internetworking (universal service over a network of networks)
*   Internet applications (programs that use the Internet)

_Ethernet_ is a networking standard for local networking.

_TCP/IP_ is a protocol for internetworking.

The growth of networking heralded an era of distributed networking.

Network standards were proposed and documented in Requests for Comments (RFCs). This became customary starting in 1969.[^1]

The Internet Engineering Task Force (IETF) also grew from this effort to standardize and collaborate on the growth of networking and internetworking.[^2]

Discusses units of measure.


## Video 2: Overview of Networking

Protocols define…



1. format and order of messages sent and received among network entities
2. actions taken on message transmission and receipt

Describes three aspects of an inter-network:



1. Network edge: clients and servers (hosts) that initiate requests or provide services
    1. client/server model
    2. peer-to-peer (no server acting as intermediary or service provider)
2. Network core: network infrastructure (outers, maybe switches)
3. Physical media: communication links in the form of copper or fiber


### 


### TCP Service

A connection-oriented service that has…



*   reliable, in-order, byte-stream data
*   flow control
*   congestion control


### UDP Service

A connectionless service



*   faster than TCP
*   no congestion control, no flow control, no handshakes


## Video 3A: Circuit Switching and Packet Switching

How does data get from one network over to another? There are two different models for this:


### Circuit Switching

A physical/electronic link is made between two hosts (think of phone exchanges). We create a connection via agreement between the two endpoints and that stays in place until one of them hangs up. The circuit is a dedicated resource: no other entity aside from the two parties in communication, can use the circuit.

Cons: charged on per-minutes basis; idle resources are wasted because they aren't shared with an entity that could make use of them

Links used within circuits can be divided up among users/clients/hosts using two different methods:



*   FDM: Frequency Division Multiplexing
*   TDM: Time Division Multiplexing


### Packet Switching

Data is broken up into packets that are layered with routing information and contain a payload. These packets travel on shared media/infrastructure.

Aggregate demand for resources can exceed available bandwidth. Aggregate demand can also cause congestion, which results in packet queues.

Packets originating from the same host and going to the same destination might take different routes to arrive at said destination; each packet travels independently. They can be retransmitted if lost (though this depends upon the protocol in use).

Destination must reassemble packets in correct order.

Implementations are considerably more complex than a circuit-switching network. Hosts/entities on packet-switching networks must be capable of the following functions:



*   Packet construction
    *   encoding/wrapping data payload
*   Packet transmission
    *   sending a packet to its destination
*   Packet interpretation
    *   unpacking/decoding data in a packet at its destination
    *   acknowledge receipt (depending upon protocol)


#### Routing and Forwarding

In a packet switching network, routers in the network core play a key role in ensuring delivery of data. They determine what packets should be sent to which routers (routing) and then transmit these packets onward (forwarding).

However, these two actions aren't the only functions performed by the routers. Routers also are involved in…



*   Queuing
*   Route discovery
*   Traffic/congestion control
*   Retransmission of lost packets
*   QoS
*   etc


## Video 3B: Packet-switched vs. Circuit-switched networks


### Statistical Multiplexing

Transmission medium is shared _on demand_: if one host stops transmitting, another is provided that host's bandwidth (this is not the case with a circuit-switched network).

Utilization = average percentage bandwidth used over a period of time

Using probability, we can provide a greater number of users with equivalent amounts of bandwidth. So a circuit-switched network that serves 10 people could be equivalent to a packet-switched network of 35 users, each of which is active only 10% of the time.

Packet-switching is not good for time-sensitive applications. Applications like video and audio need bandwidth guarantees and other technologies to ensure their proper functioning on a packet-switched network.


## Video 4: Network Structure and Performance


### Access Networks

Access networks are managed by ISPs to provide Internet access to residences. They typically lead to regional ISPs (eg. Atlantic Broadband) and traffic will then flow upward to larger, Tier 1 ISPs such as AT&T and Sprint. Sometimes this traffic will be routed through IXPs (Internet Exchange Points).

Modems (modulate-demodulators): translate digital to analog and vice versa


### Network Performance Metrics


#### Throughput

Defined as the rate (bits/sec) at which bits are actually being transferred between sender/receiver. "Instantaneous throughput" is the rate at any given point in time. "Average throughput" is measured over a longer period of time.

Note that this is an end-to-end measurement that is more than just simply transmission rate. It takes queuing and congestion into account.


#### End-to-end delay (nodal delay)

Usually measured in milliseconds and seconds. There are four sources of packet delay on a packet-switched network:



1. nodal processing: constructing and deconstructing the packets
2. queueing delay: time waiting at router for transmission, which depends upon congestion
3. transmission delay (equal to _L/R_, where _L_ is the packet length in bits and _R_ is the link bandwidth, measured in bits per second). This is the physical capacity of the device to manipulate the medium of the link in order to transmit data
4. propagation delay: essentially the speed of light, but varies minutely per medium (copper, fiber optic wire, wireless). Defined as _d/s_, where _d_ is the length of the physical link (often in meters) and _s_ is the propagation speed of the medium (often approximately 2.5 x 10<sup>8</sup> m/s)

d<sub>nodal</sub> = d<sub>proc</sub> + d<sub>queue</sub> + d<sub>trans</sub> + d<sub>prop</sub>

It's important to bear in mind that, before a packet can be transmitted onward (forwarded) all of its bits must be assembled/accumulated at the network entity (a router, for example). We call this process of gathering related bits and passing them on "store and forward."


#### Packet Loss

When packets queue, buffers on the router or switch fill up. When the buffer overflows, packets at the end of the queue fall off.


## Book Notes: Sections 1.1 - 1.4


### Section 1.2

Physical media of transmission fall into two categories: _guided_ (copper wire, fiber optic cable) and _unguided_ (electro-magnetic waves in the atmosphere and in orbit).

UTP (unshielded, twisted-pair) ethernet cable is the most common physical medium. It is often used inside buildings for local area networks.

The set of standards surrounding optical fiber networks are known as OC (optical carrier) standards, and are denoted OC-n, where n is a multiple of the base rate of 51.84 Mbit/s. For example, an OC-12 link would be capable of a data rate of 12 x 51.84 Mbits/s. 


### Section 1.3

Packet switches are part of the internet core and typically consist of routers and link-layer switches. Most packet switches use _store-and-forward transmission_: the packet switch must receive the entire packet before it can begin to transmit the first bit of the packet onto the outbound link.

The end-to-end transmission delay between a source and a destination is a multiple of the transmission delay and the number of links involved (assuming a universal transmission rate). For example, if there were three routers between a source and destination, there would be a delay of 4(L/R).

Each router has a _forwarding table_ that maps destination addresses to the router's outbound links.

Points of presence (PoPs) is a group of co-located routers in a provider's network where client ISPs can connect into the provider's network. This is sometimes done by leasing a direct, high-speed link from a third party to connect a client router with a router of the provider. 

When an ISP chooses to _multi-home_, it connects to multiple, up-stream providers for the sake of redundancy and, perhaps, more efficient routing.

ISPs at the same level of the internet hierarchy often choose to _peer_, or connect to each other settlement-free (ie. without cost). This saves them paying a provider ISP for bandwidth used communicating with one another. Another option for interconnectivity is an Internet Exchange Point (IXP). An IXP is a meeting point where multiple ISPs can peer together. An IXP is typically in a stand-alone building with its own switches.


### Section 1.4

Packet delay (_total nodal delay_) in packet-switched networks is a function of several factors that occur sequentially as a packet travels through a network device:



*   nodal processing delay: this is the time taken to examine the packet's header and determine where to direct the packet, as well as detect and correct bit-level errors that occurred in previous transmission
*   queuing delay: this delay depends to a large degree on the number of packets already in the queue, waiting to be transmitted; if the queue is empty, queuing delay is zero and the process of transmitting the packet is begun immediately
*   transmission delay: this is denoted by _L/R_, where _L_ is the length of the packet in bits and _R_ is the transmission rate of the link in bits per some unit of time (typically a second)
*   propagation delay: the propagation speed depends upon the physical length of the link (_d_) and the speed at which a signal travels through the medium (typically it approaches the speed of light); calculated as _d/s_


#### Queuing Delay and Packet Loss

Queuing delay is characterized according to three factors: the rate at which traffic arrives at the queue, the transmission rate of the link, and the nature of the arriving traffic, that is, whether the traffic arrives periodically or arrives in bursts. The ration at which traffic accumulates relative to the rate of transmission is known as traffic intensity and it's described by the following formula: _La/R_, where _L_ is the length of a packet in bits (here assumed to be a constant), _a_ is the number of packets per second that accumulate in the queue, and _R_ is the transmission rate. Networks and the devices that comprise them must not allow the traffic intensity to exceed 1.

Packet loss occurs when a packet arrives at a router or switch and the queue is full.


#### End-to-End Delay

End-to-end delay is found by multiplying the number of nodes (sans the initial host) times the processing, transmission, and propagation delays.


    _d<sub>end - end</sub> = N (d<sub>proc</sub> + d<sub>trans</sub> + d<sub>prop</sub>)_


#### Other Forms of Delay

VOIP involves the packetization of audio data, and there is a delay involved with this process known as **packetization delay**. Other forms of delay are incurred due to protocols themselves delaying the transmission of data in order to accommodate other hosts on a shared link.


#### Throughput

One way of approximating network performance is by assessing the instantaneous throughput of a given link or set of links. Throughput is transmission rate, in aggregate. In a network with many links between a given client and server, the potential throughput is limited by the slowest link: _min{R<sub>1</sub>, R<sub>2</sub>, R<sub>3</sub>...R<sub>N</sub>}_. This slowest link is referred to as the **bottleneck link**. In a shared network, with shared links, the transmission rate of a link that is common to many routes and will be a fraction of However, this is only a gross approximation of transfer time, as it does not account for an of the delays previously mentioned.


# Week 2


## Video 5: Access Networks and Physical Media


### Guided Media


#### Twisted Pair Copper Wire

Categories 3, 5 and 6, each with their own rate of communication. Subject to EM interference.

The most common form of physical medium. Typically consist of four pairs of wires, each with a pin/connector in a cable termination (RJ-45, _RJ_ stands for "registered jack"). Pins 1 and 2 are for transmission; 3 and 6 are for receiving; pins 4, 5, 7, and 8 are not used for data.

The wiring of connectors can be done in _connector_ configuration or _crossover_. The connector configuration is for connecting a computer to a network device, whereas crossover is used for PC-to-PC connections.


#### Coaxial

Shielded and jacketed. Fewer issues with interference due to insulation and shielding.

Two types: FDM (frequency division multiplexing), which is used for multi-channel broadband. These frequencies or channels can then support multiple users with statistical multiplexing. The second type of traffic is baseband, which uses a small part of the spectrum and sends only one signal at a time.


#### Fiber Optic Cable

Glass fiber carrying light pulses, with each pulse representing a bit. This is capable of extremely high rates of transmission on the order of tens or hundreds of Gbps. It also has an extremely low error rate, because of a lack of interference (not susceptible to EM/RF interference).

Channels are divided by prisms. This is informally known as "color division multiplexing."


### Unguided Media

Affected by the physical and electro-magnetic environment.

Types include terrestrial microwave (directional and line-of-sight, 45 Mbps channels), LAN technologies like 802.11b/g/n (Wifi), wide-area technologies (cellular protocols like 3G, 4G, and LTE which typically are capable of hundreds of Kbps), and finally satellite technologies, which can communicate with geostationary (36000km) or low-Earth-orbit satellites (300-600km). Note that satellite communications are not always between terrestrial stations and satellites, but often involve hops between multiple satellites.


## Video 6: Protocols and Layering

Each layer in the network implements a service, processing packets internally, then passing the results to the layer above it (sending) or below it (receiving).

This layering model is also representative of how the Internet is structured. Clients at the edge of the network send packets that are encapsulated in an application-level layer. Core devices (routers and switches) deconstruct the packet to read routing and addressing information. Then, upon arriving at its destination at the network's edge, the packet is torn down, layer by layer, until the application data is revealed.


## Video 7: Network Security

DDoS Attacks

Packet Sniffing


## Video 8: Network Applications and Architecture

A socket consists of an IP address and a port number.

A connection is a pair of sockets, one on each host.

Client-Server

Peer-to-Peer (P2P)

Hybrid P2P

	P2P VOIP (eg. Skype)

	Instant Messaging (Keybase)


## Video 9: Protocols

There are public-domain and proprietary protocols.

Application Layer protocols need to consider what services, at lower levels in the network stack, they will take advantage of. Data integrity, bandwidth, timing, and security are all related to lower-level services. 


## Book Notes: Sections 1.5 - 1.8, 2.1


### Section 1.5 Protocol Layers and Their Service Models

Protocols belong to layers in the Internet and ISO models of networks. Each layer offers a service to the layer above it (this perspective on a layer is known as the **service model**). Taken together, the layers and their protocols form a **protocol stack**. 


#### Application Layer

Protocols like HTTP, DNS, FTP, and SSH all belong to the application layer. The information exchanged by hosts on a network that are communicating with each other via an application-level protocol is referred to in the text as a **message**. 


#### Transport Layer

There are two transport layer protocols used on the internet: UDP and TCP. TCP is a connection-oriented protocol that does much to guarantee the receipt of packets. It has facilities for flow-control and anti-congestion.

UDP, on the other hand is connectionless and asynchronous. It offers no guarantee of reliability, no flow control and no congestion control.

The book refers to transport-layer packets as **segments**.


#### Network Layer

The network layer deals with **datagrams**: packets that are addressed for delivery. The Internet transport-layer protocol (TCP or UDP) in a source host passes a transport-layer segment and a destination address to the network layer, just as you would give the postal service a letter with a destination address. The network layer then provides the service of delivering the segment to the transport layer in the destination host.

The IP protocol is part  of this layer. All Internet components that have a network layer must run the IP protocol.

Many routing protocols are also present in this layer. Among them, RIP, OSPF, and BGP. 


#### Link Layer

The services provided by the link layer depend upon the link layer protocol employed over the link. Some link-layer protocols provide reliable delivery and some don't. Examples of link-layer protocols are Wifi, Ethernet, PPP, and DOCSIS.

As datagrams typically need to traverse several links to travel from source to destination, a datagram may be handled by different link-layer protocols at different links along its route. For example, a datagram may be handled by Ethernet on one link and by PPP on the next link. The network layer will receive a different service from each of the different link-layer protocols. 

The book refers to link-layer packets as **frames**.


#### Physical Layer

The protocols in this layer are again link dependent and further depend on the actual transmission medium of the link (for example, twisted-pair copper wire, single-mode fiber optics). For example, Ethernet has many physical-layer protocols: one for twisted-pair copper wire, another for coaxial cable, another for fiber, and so on.


### Section 1.6 Network Security

**botnet**: a network of compromised devices under the control of a nefarious actor

**virus**: a piece of malware that requires some form of user interaction to infect the user's device

**worms**: malware that can compromise a device without any user interaction

**packet-sniffing**: capturing traffic and inspecting its payload

**IP-spoofing**: injecting packets into the network with false addresses


#### Dedenial-of-service (DoS) attacks

An attack that focuses on preventing a server from effectively offering a service. This type of attack can be broken down into three different categories:


        _Vulnerability attacks_: carefully crafted messages are aimed at a vulnerable application or operating system running on a targeted host


        _Bandwidth flooding_: the attacker sends a deluge of packets to the targeted host, often from many different (thousands) of computers (a botnet). This latter situation is called a DDoS


        _Connection flooding_: the attacker establishes a large number of half-open or fully open TCP connections at the target host


### Section 2.1 Network Application Architecture

Introduces **client-server** and **P2P** architectures and mentions data centers within the context of services and millions of clients (that is, meeting demand). Also mentions hybrid solutions that mix both architectures.

**Sockets** are software interfaces to the network services at the transport layer provided by the network stack. Processes communicate via sockets by way of ports. Each port has a unique number, from 0 to 65535 (though some ports cannot be used).[^3] 

Application developers choose a transport layer protocol depending upon the needs of their application and the services offered by the two protocols available (UDP and TCP). We can broadly classify the possible services along four dimensions: reliable data transfer, throughput, timing, and security.


#### Reliable Data Transfer

A guarantee that the data sent by one end of the application is delivered correctly and completely to the other end of the application. Such a guarantee is not needed by **loss-tolerant applications**, such as multimedia applications.


#### Reserved Throughput

Applications that have throughput requirements are said to be **bandwidth-sensitive** applications. For these applications, it is helpful to ascertain what level of throughput can be guaranteed by the underlying transport protocol, as throughput will vary given that intermediate links are shared by multiple hosts. Such a guarantee is not as important to applications that do not have specific throughput requirements, known as **bandwidth** **elastic applications**.


#### Guaranteed Timing

Real-time applications (multi-player gaming, video conferencing, etc) suffer when latency exceeds a certain level. A transport protocol capable of ensuring that a packet or packets arrived at a certain time after they were sent would be preferable to one that couldn't offer such a guarantee.


#### Security

In the sending host, a transport protocol can encrypt all data transmitted by the sending process, and in the receiving host, the transport-layer protocol can decrypt the data before delivering the data to the receiving process. Such a service would provide confidentiality between the two processes, even if the data is somehow observed between sending and receiving processes. A transport protocol can also provide other security services in addition to confidentiality, including data integrity and end-point authentication.

Of these four service characteristics, TCP can guarantee only one: reliable data transfer. There are aspects of the protocol that aid in attempting to reserve throughput (congestion control, indirectly) but there's no guarantee that a certain bandwidth will be reserved for an application. 


#### Application-Layer Protocols

What does an application layer protocol define?



*   the type of messages exchanged (request and response, for example)
*   syntax of message types (the fields in the message and how they are delineated)
*   semantics: the meaning of the information in the fields
*   sequence of communication (when is a certain type of message sent, and what is the appropriate response)

Some application layer protocols are public (published in RFCs), others are private and proprietary (Skype, for example).


# Week 3


## Video 10: HTTP



*   Client is more complicated than the server, as the client has to render the server's response.
*   Uses the TCP protocol at the transport layer.
*   HTTP, itself, is stateless: no information is saved.
*   An HTTP request is sent after a three-way, TCP handshake.


### Non-persistent HTTP



1. TCP SYN, then ACK (1 x RTT)
    1. HTTP client initiates a TCP connection to server
    2. Server ACK-nowledges the request and accepts the connection
2. Client sends an HTTP request with a URL for a resource to server
3. Server responds with an HTTP response message, and _closes the connection_
4. Client renders response, if applicable
5. Repeat steps 1-4 for each referenced resource in web page

Total time: 2 x RTT + File Transfer time

Browsers often open parallel TCP connections to fetch referenced objects in this model.


### Persistent HTTP

Server leaves connection open after sending initial, HTTP response.

Subsequent messages sent over same TCP connection.

Theoretically, possible that total time for retrieving a webpage and all associated resources could be 1 x RTT + transfer times


### HTTP Request Syntax and Semantics

Starts with Request Line that has the following format: _method URL version_ (eg. `GET /path/to/file.html HTTP/1.1\r\n`)

Common Header Lines:



*   Host
*   Usert-Agent
*   Accept, Accept-Language, Accept-Encoding, Accept-Charset
*   Keep-Alive (specifies time to persist connection)
*   Connection: (value of _keep-alive_ asks to keep connection open)

Common Status Codes



*   505 (HTTP version not supported)
*   404 (Resource not found)


## Video 11: Cookies and Caching in HTTP

HTTP is stateless, so how do we keep a history of client connections and state?


### Cookies

When client makes a request, server responds with a response that has a Set-cookie header, along with an ID generated by the server. The browser then creates a cookie and allows the server to record certain details regarding the client-server interaction there. This is a benefit to the server-side, as it allows it to offload storage and data retrieval on the client.


#### Caching and Proxies

Proxy servers can diminish the amount of traffic on an access link by caching data. Cached data is time-stamped, so that origin servers can be queried and a comparison made between content on the server and content in the proxy's cache. This is the basis for a conditional GET.


#### Conditional GET

Two scenarios are possible: cached content is stale, or it's not. In both, the client (a proxy server)  is sending an `If-modified-since` header.

If cached content is not stale, the client's request is responded to with a status code of `304` (`Not Modified`). If it is, client is sent the data with a response of `200`. 

To calculate the benefit of having a proxy, we must know how frequently requests will hit (and not miss), because the number of misses will correlate to the amount of bandwidth used with the proxy in place.


## Video 12: FTP and Email


### FTP

FTP (file transfer protocol) is a client-server protocol that uses two TCP connections: the server listens on port 21 for control messages, and on port 20 for data. The control connection remains open and active even after data has been sent or received. 

FTP commands are short and basic; return codes are numeric, akin to HTTP.


### Email

Mail protocols can be extended with MIME extensions such that they can accommodate messages with something other than 7-bit ASCII.

SMTP: Protocol for sending mail; TCP; default port 25; messages must be in 7-bit ASCII; SMTP servers communicate with each other in order to relay mail through the core of the internet

IMAP: manipulation of stored messages on server (successor to IMAP; stateful)

POP3: download email to local boxes and either delete or keep messages on the server (stateless)

HTTP: webmail


## Video 13: DNS

The DNS system is basically a system of hierarchical, distributed databases that map names to IP addresses.

There are 13 root name servers worldwide. 

DNS records are kept in resource record (RR) format:


```
    (name, value, type, ttl)
```


A resource record has many different types: 



*   _A_ indicates that the RR entry pertains to a name.
*   _MX_ indicates that the RR entry's value is the name of a mailserver associated with an A record
*   CNAME means that the value field is an alias for the name field

DNS protocol messages:



*   All multi-byte numeric values must be in big-endian order (network order)


## Book Notes: Sections 2.2 - 2.4, 2.7


### Section 2.2.1 The Web and HTTP

HTTP is a **stateless protocol**: the protocol itself does not maintain a record of the state of the client or a history of requests and responses.

HTTP runs over TCP, using a client-server model, with the server always listening on port 80 (or 443). The protocol is capable of establishing both persistent and non-persistent connections.


#### HTTP with Non-persistent Connections

A socket on both the client and server are created for each request of an object: every page, every referenced image, every referenced script causes the server and client to create and tear down a connection. These separate connections can be made serially, or in parallel (simultaneously) depending upon how the client is configured.

It takes 2xRTT (Round-Trip Time), plus the transmission time involved in sending the requested object to the client, to send a single resource between client and server. The first round trip is taken up by the first two parts of the three-way, TCP handshake (`SYN` -> `SYNACK`). The second round-trip involves the client sending its actual HTTP request, plus a TCP segment with its SYN bit set to zero. The final trip is the server's HTTP response.


#### HTTP with Persistent Connections

Version 1.1 of the HTTP protocol allows for persistent connections. Essentially, the server keeps the TCP connection alive rather than closing it after the first request is honored. The connection is closed by the server after a configurable timeout interval.


### Section 2.2.3 HTTP Message Format

There are two types of HTTP messages: requests and responses.


#### Requests

An HTTP request is comprised of a **request line**, followed by **header lines**. The request line has three fields: the method field, the URL field, and the HTTP version field. For example:


```
    GET /somedir/page.html HTTP/1.1
```


The Host header line is used by Web proxy caches. The Connection header is used to indicate whether the client wants to maintain an open connection. 


#### Responses


### Section 2.2.5 The Web and HTTP


### 2.4 DNS


<!-- Footnotes themselves at the bottom. -->
## Notes

[^1]:
     http://www.rfc-editor.org

[^2]:
     http://www.ietf.org

[^3]:
     Needs to be confirmed through further reading.


<!-- Docs to Markdown version 1.0β17 -->

