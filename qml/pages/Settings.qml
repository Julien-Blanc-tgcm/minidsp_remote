import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    Column {
        id: listView
        anchors.fill: parent
        PageHeader {
            title: qsTr("Settings")
        }
        TextField {
            label: qsTr("Device address")
            text: configAddress.value;
            placeholderText: qsTr("Enter the address of the device")
            id:deviceAddr
            width: parent.width
        }
    }
    onStatusChanged: {
        if(status === PageStatus.Deactivating)
        {
            configAddress.value = deviceAddr.text;
            configAddress.sync();
        }
    }
}
