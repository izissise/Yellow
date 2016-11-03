import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2

import Lib 1.0 as Lib

Rectangle {
    id: root;
    width: 1024;
    height: 768;
    property alias text2Text: text2.text
    property alias myModel: listview1.model
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
        x: 8
        y: 212
        width: 1008; height: 126

        model: myModel
        delegate: Rectangle {
            height: 25
            width: 100
            Text { text: modelData }
        }
    }

    ListModel {
        id: dataModel2
        ListElement {
            title: "Package 1"
            id_: "2433434D3"
            comment: "SUPER LIST IN VIEW"
        }
        ListElement {
            title: "Package 2"
            id_: "2345Y6U789"
            comment: "declaree dans windows qml"

        }
        ListElement {
            title: "pacakge 3"
            id_: "2345678987654321"
            comment: "a remplir depuis le code cpp"

        }
        ListElement {
            title: "pacakge 4"
            id_: "23456rttrt87654321"
            comment: "COMMENTER LA LIGNE model: dataModel2 dans Table View, "

        }
        ListElement {
            title: "pacakge 5"
            id_: "2345eregvcdd4GF4f87654321"
            comment: "ainsi que l'object ListModel comportant ces chose la, "

        }
        ListElement {
            title: "pacakge 6"
            id_: "zxkcr2E33D"
            comment: "puis creer l'object correponsdant a cette liste dans le cpp, "

        }
        ListElement {
            title: "pacakge 7"
            id_: "2e5TRGFDScdd4GF4f87654321"
            comment: "puis set la value 'dataModel'   ->"

        }

        ListElement {
            title: "pacakge 8"
            id_: "2e5TRGFDfcecdvf652fEFC1"
            comment:     "QStringList dataList; dataList.append(some_data); object->setProperty(\"dataModel\", QVariant::fromValue(dataList)); "
        }



    }

    TableView {
        id: mTableView
        x: 8
        y: 352
        width: 987
        height: 235
        TableViewColumn {
            role: "title"
            title: "Title"
            width: 120
        }
        TableViewColumn {
            role: "id_"
            title: "ID"
            width: 80
        }
        TableViewColumn {
            role: "comment"
            title: "Commentaire"
            width: 580
        }
        model: dataModel2
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
