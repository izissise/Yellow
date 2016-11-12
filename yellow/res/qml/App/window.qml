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
        x: 46
        y: 30
        width: 166
        height: 15
        color: "#ffffff"
        text: qsTr("HELLO")
        font.pixelSize: 12
    }
    Text {
        id: text2
        x: 304
        y: 30
        width: 129
        height: 15
        color: "#ffffff"
        font.pixelSize: 12
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
            role: "IpV"
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


}
