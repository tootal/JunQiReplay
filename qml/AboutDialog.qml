import QtQuick
import QtQuick.Controls

Window {
    id: root
    title: qsTr("JunQiReplay")
    visible: false
    maximumHeight: 120
    minimumHeight: 120
    maximumWidth: 400
    minimumWidth: 400
    modality: Qt.WindowModal
    flags: Qt.Dialog

    property alias text: dialogText.text
    property AboutDialogIcon icon: AboutDialogIcon {}
    readonly property int buttonAreaHeight: 40

    function open() {
        root.show()
    }

    Item {
        height: parent.height - buttonAreaHeight
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        Image {
            id: dialogImage
            source: icon.source
            width: icon.width
            height: icon.height

            anchors {
                left: parent.left
                leftMargin: 10
                verticalCenter: parent.verticalCenter
            }
        }

        Text {
            id: dialogText
            text: "About dialog text content"
            wrapMode: Text.WordWrap

            anchors {
                left: dialogImage.right
                leftMargin: 10
                right: parent.right
                rightMargin: 10
                verticalCenter: parent.verticalCenter
            }
        }
    }

    Rectangle {
        height: buttonAreaHeight
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        color: "#f0f0f0"

        Button {
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 10
            }

            width: 64
            height: 24
            text: "OK"
            onClicked: root.close()
        }
    }
}
