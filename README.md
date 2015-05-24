# qcoap
CoAP for Qt

Now it's a work in progress! only PDU encoding and decoding are supported (and untested yet)
Plans are:
* PDU encoding/decoding with custom options and content format support
* CoapNode class which well be talking to one remote CoAP node, work with sockets, 
process retransmissions and other stuff
* Local nodes discovery (ideaylly with many nodes per IP address support (multicast restrictions), maybe daemon that will provide discovery service for local virtual nodes)
* Qt Quick plugin (write UI for CoAP devices using qml only)
* Dynamic QObject and properties (bind signal with any arguments to CoAP observable resource, bind CoAP GET to slot call (with any arguments too), export properties, bind remote properties to local)
* File transfer (for Over The Air Update, UI elements (for example: node returning a QML, representing an editor or viewer of special type))
