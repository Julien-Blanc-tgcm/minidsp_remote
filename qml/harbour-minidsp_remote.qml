import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Configuration 1.0
import harbour.eu.tgcm.minidsp_remote 1.0
import "pages"

ApplicationWindow
{
    ConfigurationGroup {
        path: "/eu/tgcm/minidsp"
        id: config
        ConfigurationValue {
            key: "address"
            defaultValue: ""
            id: configAddress
        }
    }

    NanoAVRClient {
        address: configAddress.value
        id: theClient
    }
    Connections {
        target: configAddress
        onValueChanged: {
            theClient.setAddress(configAddress.value)
            theClient.connectToDevice();
        }
    }

    initialPage: Component { Client { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
}
