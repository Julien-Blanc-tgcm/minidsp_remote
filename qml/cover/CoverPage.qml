import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.eu.tgcm.minidsp_remote 1.0

CoverBackground {
    Column {
        anchors.fill: parent
        anchors.topMargin: Theme.paddingMedium
        Label {
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
            text: theClient.device.muted ? qsTr("Muted") :
                                           qsTr("-%1 dB").arg(-(theClient.device.volume / 2))
            font.pixelSize: Theme.fontSizeLarge
            color: Theme.highlightColor
            visible: isConnected()
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("HDMI %1").arg(theClient.device.currentHdmiInput)
            visible: isConnected()
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Preset %1").arg(theClient.device.currentPreset)
            visible: isConnected()
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Not connected")
            visible: !isConnected()
        }
    }

    CoverActionList {
        id: coverAction
        enabled: isConnected()
        CoverAction {
            iconSource: "image://theme/icon-cover-previous"
            onTriggered: theClient.setVolume(theClient.device.volume - 4);
        }

        CoverAction {
            iconSource: theClient.device.muted ? "image://theme/icon-m-speaker-on" : "image://theme/icon-m-speaker-mute"
            onTriggered: theClient.setMute(!theClient.device.muted)
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: theClient.setVolume(theClient.device.volume + 4)
        }
    }
    CoverActionList {
        id: coverAction2
        enabled:!isConnected()
        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: theClient.connectToDevice()
        }
    }

    function isConnected()
    {
        return theClient.connectionStatus === 2;
    }
}
