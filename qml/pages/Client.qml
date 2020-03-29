import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.eu.tgcm.minidsp_remote 1.0

Page {

    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    property string state: "unconnected"

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")
                onClicked: pushSettings()
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column
            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Minidsp settings")
            }

            Slider {
                label: qsTr("Volume")
                value: theClient.device.volume
                minimumValue: -255
                maximumValue: 0
                stepSize: 1
                onValueChanged: {
                    theClient.setVolume(value);
                }
                width: page.width
                valueText: (value / 2) + " dB"
                id: volumeSlider
                visible: isConnected()
            }
            Row {
                spacing: Theme.paddingMedium
                anchors.leftMargin: Theme.paddingLarge
                anchors.left:parent.left
                Label {
                    text: qsTr("Volume step")
                    anchors.verticalCenter: parent.verticalCenter
                }
                IconButton {
                    icon.source: "image://theme/icon-m-remove"
                    onClicked: volumeSlider.value -= 1
                    id: silentBtn
                    anchors.verticalCenter: parent.verticalCenter
                }
                IconButton {
                    icon.source: "image://theme/icon-m-add"
                    onClicked: volumeSlider.value += 1
                    id: spkonBtn
                    anchors.verticalCenter: parent.verticalCenter
                }
                visible: isConnected()
            }
            TextSwitch {
                checked: theClient.device.muted
                text: qsTr("Mute")
                automaticCheck: false
                onClicked: {
                    theClient.setMute(!checked)
                    checked = !checked
                }
                visible: isConnected()
            }

            ComboBox {
                label: qsTr("HDMI Input")
                anchors.left: parent.left
                anchors.right: parent.right
                menu: ContextMenu {
                    MenuItem { text: "HDMI1" }
                    MenuItem { text: "HDMI2" }
                }
                onValueChanged: {
                    theClient.setHdmiInput(currentIndex + 1)
                }
                id: hdmiInputCb
                visible: isConnected()
            }
            ComboBox {
                label: qsTr("Equalizer preset")
                anchors.left: parent.left
                anchors.right: parent.right
                menu: ContextMenu {
                    MenuItem { text: qsTr("Preset 1") }
                    MenuItem { text: qsTr("Preset 2") }
                    MenuItem { text: qsTr("Preset 3") }
                    MenuItem { text: qsTr("Preset 4") }
                }
                onValueChanged: {
                    theClient.setEqualizerPreset(currentIndex + 1)
                }
                id: presetInputCb
                visible: isConnected()
            }

            // conneting
            Label {
                text: qsTr("Connecting, please wait");
                anchors.horizontalCenter: parent.horizontalCenter
                visible: isConnecting()
            }

            BusyIndicator {
                size: BusyIndicatorSize.Large
                running: theClient.connectionStatus !== 2
                anchors.horizontalCenter: parent.horizontalCenter
                visible: isConnecting()
            }

            // Connection errer
            Label {
                text: qsTr("Impossible to connect to %1").arg(theClient.address);
                anchors.horizontalCenter: parent.horizontalCenter
                visible: isConnectionError()
            }
            Button {
                text: qsTr("Retry")
                onClicked: theClient.connectToDevice()
                visible: isConnectionError();
                anchors.horizontalCenter: parent.horizontalCenter
            }

            // No device
            Label {
                text: qsTr("No device set");
                anchors.horizontalCenter: parent.horizontalCenter
                visible: noDeviceConfigured();
            }

            Button {
                text: qsTr("Add device")
                onClicked: pushSettings()
                visible: noDeviceConfigured();
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
    Component.onCompleted: {
        theClient.connectToDevice();
    }
    Connections {
        target: theClient.device
        onCurrentHdmiInputChanged : hdmiInputCb.currentIndex = theClient.device.currentHdmiInput - 1
    }
    Connections {
        target: theClient.device
        onCurrentPresetChanged: presetInputCb.currentIndex = theClient.device.currentPreset - 1
    }

    function pushSettings()
    {
        pageStack.push(Qt.resolvedUrl("Settings.qml"))
    }

    function isConnected()
    {
        return theClient.connectionStatus === 2;
    }

    function isConnecting()
    {
        return theClient.address != "" &&
                (theClient.connectionStatus === 0 || theClient.connectionStatus === 1)
    }

    function isConnectionError()
    {
        return theClient.address != "" && theClient.connectionStatus === -1;
    }

    function noDeviceConfigured()
    {
        return theClient.address === "";
    }
}
