import QtQuick 2.6

//import Lib 1.0 as Lib

Rectangle {
    id: root;
    width: 1024;
    height: 768;
    property alias text2Text: text2.text
    property alias myModel: listview1.model

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
        id: listview1
        x: -6
        y: 212
        width: 1016; height: 506

        model: myModel
        delegate: Rectangle {
            height: 25
            width: 100
            Text { text: modelData }
        }
    }

    /*ListView {
        width: 100; height: 100
        id: listview1
        x: 8
        y: 209

        model: myModel
        delegate: Rectangle {
            height: 25
            width: 100
            color: model.modelData.color
            Text { text: name }
        }
    } */
}
