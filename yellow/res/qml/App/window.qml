import QtQuick 2.6

import Lib 1.0 as Lib

Rectangle {
    id: root;
    width: 1024;
    height: 768;
    property alias text2Text: text2.text

    color: "#282C34"

    Column {
        anchors.centerIn: parent;

        Lib.Icon {
            anchors.horizontalCenter: parent.horizontalCenter;
            type: types.fa_headphones;
            pointSize: 80;
        }

        Lib.Label {
            font.pointSize: 24;
            anchors.horizontalCenter: parent.horizontalCenter;
            text: Qt.application.name + " " + Qt.application.version
        }

        Lib.Label {
            font.pointSize: 12;
            anchors.horizontalCenter: parent.horizontalCenter;
            text: Qt.application.organization + " " + Qt.application.domain
        }
    }

    Text {
        id: text1
        x: 407
        y: 206
        width: 166
        height: 15
        color: "#ffffff"
        text: qsTr("HELLO")
        font.pixelSize: 12
    }
    Text {
        id: text2
        x: 607
        y: 406
        width: 129
        height: 15
        color: "#ffffff"
        font.pixelSize: 12
    }

}
