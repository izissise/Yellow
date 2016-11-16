import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

import Lib 1.0 as Lib

Rectangle {
    id: root;
    width: 1024;
    height: 768;
    property alias text2Text: text2.text
    property alias dataModel: mTableView.model
    property alias search: textEdit1.text
    property alias currentchoice: box.currentText

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
        id: text2
        x: 42
        y: 8
        width: 129
        height: 15
        color: "#ffffff"
        font.pixelSize: 12
    }

    ComboBox {
        id: box
        x: 634
        y: 48
        width: 126
        height: 22
        model: [ "Ethernet - Src", "Ethernet - Dst", "IP - Src", "IP - Dst", "TLL", "Version","Protocol","SrcPort" , "DstPort", "data", "Checksum"  ]
    }

//        ListModel {
//        id: dataModel2
////        ListElement {
////            EtSrc: "";
////            EtDst: "";
////            IpSrc: "";
////            IpDst: "";
////            IpTll: "";
////            IpV: "";
////            Protocol: "";
////            SrcPrt: "";
////            DstPrt: "";
////            data: "";
////            Checksum: "";
////        }
//       }

    TableView {
        id: mTableView
        x: 8
        y: 85
        width: 1008
        height: 653
        TableViewColumn {
            role: "EtSrc"
            title: "Ethernet - Src"
            width: 90
        }
        TableViewColumn {
            role: "EtDst"
            title: "Ethernet - Dst"
            width: 90
        }
        TableViewColumn {
            role: "IpSrc"
            title: "IP - Src"
            width: 90
        }
        TableViewColumn {
            role: "IpDst"
            title: "IP - Dst"
            width: 90
        }
        TableViewColumn {
            role: "IpTll"
            title: "TLL"
            width: 80
        }
        TableViewColumn {
            role: "Ipv"
            title: "Version"
            width: 90
        }
        TableViewColumn {
            role: "Protocol"
            title: "Protocol"
            width: 90
        }
        TableViewColumn {
            role: "SrcPrt"
            title: "SrcPort"
            width: 80
        }
        TableViewColumn {
            role: "DstPrt"
            title: "DstPort"
            width: 80
        }
        TableViewColumn {
            role: "data"
            title: "data"
            width: 80
        }
        TableViewColumn {
            role: "Checksum"
            title: "Checksum"
            width: 80
        }


        //model: dataModel2
    }

    Button {
        id: button1
        x: 8
        y: 48
        onClicked: _myClass.start("sniff")
        text: qsTr("start")
    }

    Button {
        id: button2
        x: 101
        y: 48
        onClicked: _myClass.stop("sniff")
        text: qsTr("stop")
    }

    Button {
        id: button3
        x: 200
        y: 48
        onClicked: _myClass.clear("sniff")
        text: qsTr("clear")
    }

    Button {
        id: button4
        x: 286
        y: 48
        onClicked: _myClass.clear_stop("sniff")
        text: qsTr("clear and stop")
    }



    Button {
        id: button5
        x: 893
        y: 48
        onClicked: _myClass.filter("sniff")
        text: qsTr("Filter")
    }

        TextInput {
            id: textEdit1//
            x: 769
            y: 48
            width: 116
            height: 22
            color: "#ffffff"
            font.pixelSize: 12
            text: qsTr("filtre")

        }




}
