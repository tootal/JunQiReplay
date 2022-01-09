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

    Platform.MessageDialog {
        id: aboutDialog
    }

    Platform.MessageDialog {
        id: aboutQtDialog
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
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

        TableView {
            Layout.preferredHeight: 800
            Layout.preferredWidth: 900
            columnSpacing: 1
            rowSpacing: 1
            clip: true

            model: TableModel {
                TableModelColumn { display: "name" }
                TableModelColumn { display: "color" }

                rows: [
                    {
                        "name": "cat",
                        "color": "black"
                    },
                    {
                        "name": "dog",
                        "color": "brown"
                    },
                    {
                        "name": "bird",
                        "color": "white"
                    }
                ]
            }

            delegate: Rectangle {
                implicitWidth: 100
                implicitHeight: 50
                border.width: 1

                Text {
                    text: display
                    anchors.centerIn: parent
                }
            }
        }

    }
}
