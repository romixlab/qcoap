# qcoap
CoAP for Qt

Now it's a work in progress!

Below is a planned features list:

1. Requests
  * Lambda
  ```cpp
  CoapEndpoint endpoint(QHostAddress::Any); // client mode
  CoapExchange *exchange = new CoapExchange; // will be deleted by endpoint
  exchange->setTimeout(5000);
  exchange->onComplete([](CoapExchange &exchange){});
  exchange->get();
  ```
  * Qt Quick callback function (onComplete(function(){})
  ```qml
  CoapEndpoint {
      address: "0.0.0.0"
  }
  Button {
      onClicked: {
          var exchange = Coap.Exchange("coap://coap.me/test")
          exchange.get().onComplete(function(var exchange) {
          })
          //or
      onClicked: Coap.Exchange("coap://coap.me/test").get().onComplete(function(var exchange){})
      }
  }
  ```
2. Observing a resource
  * Slot invocation on a resource change
  ```cpp
  CoapEndpoint endpoint(QHostAddress::Any);
  CoapExchange *exchange = new CoapExchange;
  exchange->observe("coap://...", object, SLOT(QString,QPoint)) // types dispatched on the fly
  ```
  * Lambda
  ```cpp
  exchange->observe("coap://...", [](QVariant value){}) // lambda's with different arguments?
  ```
  * Dynamic signal
  ```cpp
  exchange->observe("coap://...", "mysignal(int,int,QString)")
  connect(exchange->dq(), SIGNAL(mysignal(int,int,QString)),
          myobject,  SLOT(someslot(int,int,QString))); // types dispatched on the fly
  // dq() returns DQObject - class for work with dynamic slots, signals, properties
  ```
  * QML
  ```qml
  Component.onCompleted: {
   var exchange = Coap.Exchange();
   exchange.observe("coap://...").onChanged(function(){})
  }
  ```
3. Creating a simple resource
  * Publish Q_INVOKABLE method or slot as a resource
  ```cpp
  CoapEndpoint endpoint(QHostAddress::Any, 5683); // server mode
  CoapResource resource("/");
  resource.onRequest(object, SLOT(status(QString))); // get, post, put supported
  // if only get/post/put is needed
  resource.onPut(object, SLOT(status(QString))); // get, post, put supported
  // or make a decision based on request info (source, type, etc)
  resource.onRequest(object, SLOT(status(QString,CoapExchange))); // get, post, put supported
  ```
  * Custom get/post/put/delete handlers
  ```cpp
  CoapResource resource("/custom");
  resource.onRequest([](CoapExchange *exchange){ /* handle everything */ });
  ```
  * QML
  ```qml
  Component.onCompleted: {
   var resource = Coap.Resource("/qmlresource");
   resource.onRequest(function(){})
  }
  // or
  CoapResource {
   path: "/"
   onRequest: ...
   CoapResource {
     path: "/test"
     bindProperty: someObject.property
   }
  }
  ```
4. Creating a resource based on a property
  ```cpp
  CoapResource *resource = new CoapResource("/lamp");
  resource.bindProperty(someqobject, "lamp"); // get, post, put, observe supported
  ```
  * QML
  ```qml
  Button {
   id: startButton
   checkable: true
   text: "Start"
   Component.onCompleted: {
    var resource = Coap.Resource("/start")
    resource.bindProperty(startButton.checked)
   }
  }
  ```
5. Binding remote resource to local property
  * Simple (only observe remote resource)
  ```cpp
  CoapExchange *exchange = new CoapExchange("coap://vs0.inf.ethz.ch:5683/obs");
  exchange->bindProperty(someqobject, "clock");
   // when remote resource changed, clock property will be updated
   // type will be automatically casted to property type (QTime in this case)
  ```
  * Two-way (observe and fire updates)
  ```cpp
  CoapExchange exchange("coap://vs0.inf.ethz.ch:5683/obs");
  exchange.bindProperty(object, "lamp", CoapResource::TwoWay);
   // first, get request will be made and lamp property will be setted
   // next remote resource and property will be observed
   // when property will change, put request will be made
   // when resource will change, property will be updated
   // endless loop of setting property - sending put - receiving update - setting property - etc is taken care of
  ```
  * QML
  ```qml
  Led {
   id: led
   Component.onCompleted: {
    var exchange = Coap.Exchange()
    exchange.bindProperty("coap://.../observable", led.on)
    // or
    exchange.bindProperty("coap://.../observable", led.on, CoapResource.TwoWay)
   }
  }
  ```
6. File transfering
  * Create a resource that accepts incoming files
  ```cpp
  CoapResource *resource = new CoapResource("/filesink");
  resource->acceptFiles([](fileName, fileSize, CoapRequestInfo info){/*decide what to do with the file*/});
  ```
  * Creata a resource that serves files
  ```cpp
  CoapResource *resource = new CoapResource("/file");
  resource->bindFile("./somefile");
  // or
  resource->bindDirectory("./files"); // every file in files dir will be published as /file/some/path/in/files_dir
  ```
  * Downloading a file from resource
  ```cpp
  CoapExchange *exchange = new CoapExchange("coap://host/file");
  exchange->get(); // normal processing, see p.1
  // or
  exchange->get(CoapExchnage::File); // automatically save file contents to temporary file
  // or
  exchange->get(CoapExchange::File, [](CoapExchange &exchange){}); // decide download file or not based on its size,name, etc
  ```
  * Monitoring download progress (or any other exchange)
  ```cpp
  CoapExchange *exchange = new CoapExchange("some url");
  exchange->monitor([](CoapExchange &info){});
  // or
  exchange->monitor(object, SLOT(onExchangeStatusChanged(CoapExchange*)));
  ```
7. Discovery
  * Multicast based
  ```cpp
  CoapDiscoverer discoverer;
  discoverer.onEndpointFound(object, SLOT(endpointFound(CoapEndpointInfo)));
  discoverer.findAll();
  // or
  discoverer.findLocal(); // on this machine
  // or
  CoapEndpointInfo info;
  info.setName("lamp-*");
  info.setGroup("floor1, basement");
  discoverer.findMatch(info);
  ```
  @note If more than one endpoint is launched on one machine, bad thing will happen with multicast(: So proxy is needed here, see p.10
  * DNS based
  ```cpp
  CoapDiscoverer discoverer(CoapDiscoverer::DNS);
  // same API, if there is no DNS restrictions to implement them?
  ```
8. Doing a multicast requests
  ```cpp
  CoapExchange *exchange = new CoapExchange("coap://239.0.0.1/test");
  // same API, many answers
  CoapExchange *exchange = new CoapExchange("coap://[floor1]/test"); // make a request to every endpoint in floor1 group
  CoapExchange *exchange = new CoapExchange("coap://{lamp-*}/set?value=0"); // make a request to every lamp (regexp match)
  CoapExchange *exchange = new CoapExchange("coap://[floor1]{lamp-*}/set?value=0"); // group and name match
  ```
9. Securing via certificates
  * Manual way
  ```cpp
  CoapEndpoint endpoint;
  endpoint.setCertificate("path/to/certificate");
  endpoint.setKeyPassphrase("abcd");
  qDebug() << endpoint.encryptionStatus(); // CoapEncryptionStatus(Encrypted, Static/Provision mode, etc)
  ```
  * Automatic provision
  ```cpp
  CoapEndpoint endpoint;
  CoapNodeInfo info;
  info.setName("very-important-node");
  info.setGroups(QStringList() << "floor1" << "kitchen");
  endpoint.setInfo(info);
  endpoint.receiveCertificate();
  ```
  After receiveCertificate() is called, presence of certificate is checked, if there is no certificate, multicast request to group "provision" is maded, then admin will decide issue the certificate to specific endpoint or not. If certificate exists it will be loaded. What to do with keyPassphase?
10. Running many endpoints on one host (CoAP-CoAP proxy)
  If there is more than one multicast listener on one interface, only one of them will receive multicast packets (while everyone can still send them). So transparent CoAP-CoAP proxy is needed, that will be launched as a daemon.
CoapEndpoint in Client mode will check if there is such a proxy and pass every multicast request through it. C/C++ CoAP implementation is more preferable in this case, maybe smcp, libcoap or some other?
11. HTTPS-CoAP, WebSockets-CoAP proxy (configure existing)
Mobile clients that cannot run CoAP can use HTTPS instead (for example Tizen Wearable), appropriate proxy configuration must be provided (with discovery and multicast requests support)
Certificate provision must be supported.

If app using coap:// scheme is running on a mobile platform outside private networks, transparent WebSockets-CoAP proxy may be used, without changing any logic. Call CoapEndpoint::translateRequestsToHttps("wss://ext-addr");
