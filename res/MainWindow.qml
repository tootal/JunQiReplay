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

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            spacing: 0
            TextField {
                placeholderText: qsTr("Folder Path")
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.preferredWidth: 80
            }

            Button {
                icon.source: "qrc:/refresh.png"
            }
            TextField {
                placeholderText: qsTr("Filter Name")
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.preferredWidth: 80
            }
        }

        Component {
            id: tableDelegate
            Rectangle {
                implicitWidth: 100
                implicitHeight: 50
                border.width: 1
                color: background ?? "white"

                Text {
                    text: display ?? ""
                    anchors.centerIn: parent
                }
            }

        }

        HorizontalHeaderView {
            syncView: tableView
            boundsBehavior: Flickable.StopAtBounds
            delegate: tableDelegate
        }

        TableView {
            id: tableView
            Layout.preferredHeight: 800
            Layout.preferredWidth: 900
            columnSpacing: 0
            rowSpacing: 0
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            model: ReplayTableModel {}
            delegate: tableDelegate
            columnWidthProvider: function (column) {
                switch (column) {
                case 0:
                    return 30;
                case 1:
                    return 150;
                default:
                    return -1;
                }
            }
        }
    }
}
