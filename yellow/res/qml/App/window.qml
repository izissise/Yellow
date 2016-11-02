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
        x: 259
        y: 142
        width: 166
        height: 15
        color: "#ffffff"
        text: qsTr("HELLO")
        font.pixelSize: 12
    }
    Text {
        id: text2
        x: 419
        y: 142
        width: 129
        height: 15
        color: "#ffffff"
        font.pixelSize: 12
    }


    ListView {
        x: 8
        y: 223
        width: 1008; height: 522

        Component {
            id: datasDelegate
            Rectangle {
                id: wrapper
                width: 180
                height: dataInfo.height
                color: ListView.isCurrentItem ? "black" : "red"
                Text {
                    id: dataInfo
                    text: name + ": " + number
                    color: wrapper.ListView.isCurrentItem ? "red" : "black"
                }
            }
        }

        model: DataModel {}
        delegate: datasDelegate
        focus: true
    }

}
