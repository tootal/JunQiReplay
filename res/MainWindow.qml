import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform as Platform
import Qt.labs.qmlmodels

import JunQiReplay

ApplicationWindow {
    id: window
    width: 1000
    height: 600
    minimumHeight: height
    maximumHeight: height
    minimumWidth: width
    maximumWidth: width

    visible: true
    title: qsTr("JunQiReplay")

    property MainWindowViewModel viewModel: MainWindowViewModel{}

    Component.onCompleted: {
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2
    }

    Platform.MenuBar {
        Platform.Menu {
            title: qsTr("&File")

            Platform.MenuItem {
                text: qsTr("&Open Folder")
                onTriggered: openDialog.open()
            }
        }

        Platform.Menu {
            title: qsTr("&Help")

            Platform.MenuItem {
                text: qsTr("&About")
                onTriggered: aboutDialog.open()
            }
            Platform.MenuItem {
                text: qsTr("About &Qt")
                onTriggered: aboutQtDialog.open()
            }
        }
    }

    Platform.FolderDialog {
        id: openDialog
        folder: Platform.StandardPaths.standardLocations(Platform.StandardPaths.PicturesLocation)[0]
    }

    AboutDialog {
        id: aboutDialog
        text: qsTr("An application for QQGame JunQi Replay Manager.")
    }

    AboutDialog {
        id: aboutQtDialog
        icon {
            source: "qrc:/qt-logo.svg"
            width: 60
            height: 44
        }
        text: qsTr("This program uses Qt version %1. \nQt is a C++ toolkit for cross-platform application development.").arg(viewModel.qtVersion)
    }

    Component {
        id: tableDelegate
        Rectangle {
            implicitWidth: 180
            implicitHeight: 30
            border.width: 1
            border.color: "lightgray"
            color: background ?? "white"

            Text {
                text: display ?? ""
                color: foreground ?? "black"
                anchors.centerIn: parent
                font: theFont ?? Application.font
            }
        }

    }

    Item {
        anchors.fill: parent

        Row {
            id: operateRow
            width: parent.width
            height: 20

            TextField {
                width: (parent.width - parent.height) / 2
                height: parent.height
                placeholderText: qsTr("Folder Path")
            }

            Button {
                width: parent.height
                height: parent.height
                icon.source: "qrc:/refresh.png"
            }

            TextField {
                width: (parent.width - parent.height) / 2
                height: parent.height
                placeholderText: qsTr("Filter Name")
            }
        }

        HorizontalHeaderView {
            id: tableHeader
            anchors {
                top: operateRow.bottom
                left: parent.left
                right: parent.right
            }

            syncView: tableView
            boundsBehavior: Flickable.StopAtBounds
            delegate: tableDelegate
        }

        TableView {
            id: tableView
            anchors {
                top: tableHeader.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            columnSpacing: 0
            rowSpacing: 0
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            ScrollBar.vertical: ScrollBar {}

            model: ReplayTableModel {
                dirName: "C:\\Users\\tootal\\workspace\\JunQiReplay\\test"
            }
            delegate: tableDelegate
            columnWidthProvider: function (column) {
                switch (column) {
                case 0:
                    return 30;
                case 1:
                    return 140;
                case 6:
                    return 40;
                case 7:
                    return 70;
                default:
                    return -1;
                }
            }
        }
    }
}
