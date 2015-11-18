import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import CoAP 1.0

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true

    NodesWatcher {
        id: watcher
    }

    CoapExchange {
        id: exchange
        onCompleted: {
            console.log("Signal completed")
        }
    }

    Row {
        anchors.centerIn: parent
        spacing: 10

        Label {
            text: exchange.status
        }

        Button {
            onClicked: {
                exchange.onCompleted(function(){console.log("JS completed")})
                exchange.uri = "coap://127.0.0.1/"
                exchange.get()
            }

            text: "GET"
        }


    }

    Component.onCompleted: {
        watcher.watch("coap://127.0.0.1")
        watcher.watch("coap://134.102.218.18")
    }


}
