# qcoap
CoAP for Qt

Now it's a work in progress!

Below is a planned features list:

1. Requesting / Settings a resource
  * Blocking call (waitForComplete())
  ```
  CoapEndpoint endpoint;
  endpoint.bind(QHostAddress::Any, CoapEndpoint::Client);
  CoapRequest req("coap://coap.me/test");
  req.get();
  req.waitForComplete(5000);
 ```
  * Lambda
  ```
  req.onComplete([](){});
  ```
  * Qt Quick callback function (onComplete(function(){})
  ```
  CoapEndpoint {
   address: "0.0.0.0"
   mode: CoapEndpoint.Client
  }
  Button {
   onClicked: {
    Coap.Request req("coap://coap.me/test")
    req.get().onComplete(function() {
    })
   }
  }
  ```
2. Observing a resource
  * Slot invocation on a resource change
  * Lambda
  * Virtual signal
3. Creating a simple resource
  * Publish Q_INVOKABLE method as a resource
  * Publish slot as a resource
  * Custom get/post/put/delete handlers
4. Creating a resource based on a property
5. Binding remote resource to local property
  * Simple (only observe remote resource)
  * Two-way (observe and fire updates)
6. File transfering
  * Create a resource that accepts incoming files
  * Creata a resource that serves files
7. Discovery
  * Multicast based
  * DNS based
8. Doing a multicast requests
9. Securing via certificates
  * Automatic certificate provision
10. Running many endpoints on one host (CoAP-CoAP proxy)
  * Adopt existing (maybe smcp)
  * Create own
11. HTTP(s)-CoAP proxy (configure existing)
