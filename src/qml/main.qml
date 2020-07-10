import QtQuick 2.11
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.0 as Labs

//TODO: Ask for saving before closing
//TODO: Calculation method choosing
//FIXME: Rewrite global functions to signals and slots


ApplicationWindow {
    id: mainWindow
    visible: true
    title: qsTr("TRG Calculator")
    width: 1040
    height: 726
    minimumWidth: 640
    minimumHeight: 480
    property int menuBarHeight: 20
    property int blinkDuration: 1000
    property bool isBlinkAllowed: true
    property bool darkTheme: controls.isDark
    property string backgroundColor: mainWindow.darkTheme ? "#000000" : "#DDDDDD"
    property string controlColor: mainWindow.darkTheme ? "#222222" : "#BBBBBB"
    property string hControlColor: mainWindow.darkTheme ? "#666666" : "#888888"
    property string textColor: mainWindow.darkTheme ? "#AAAAAA" : "#222222"
    property string hTextColor: mainWindow.darkTheme ? "#FFFFFF" : "#000000"
    property string inputColor: mainWindow.darkTheme ? "#888888" : "#DDDDDD"
    property string version: "2.2.0"

    function syncBlink() {
        isBlinkAllowed = false
        isBlinkAllowed = true
    }

    background: Rectangle {
        anchors.fill: parent
        color: backgroundColor
    }

    Component.onCompleted: {
        //loading configs from backEnd
    }

    ParametersWindow {
        id: parametersWindow
    }

    Window {
        id: aboutWindow
        title: qsTr("About")
        flags: Qt.FramelessWindowHint
        visible: false
        minimumWidth: 380
        minimumHeight: 160
        maximumWidth: minimumWidth
        maximumHeight: minimumHeight
        color: mainWindow.controlColor
        modality: Qt.ApplicationModal
        Text {
            text: qsTr("TRG calculation program.\nVersion ")+mainWindow.version+
                  qsTr("\n\nCreated by Kirill Maksimov.\nCalculations methods provided by Regina Maximova.\nContacts: ")+
                  "maximovkirandr@gmail.com"

            color: mainWindow.textColor
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 30
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        CustomButton {
            text: qsTr("Ok")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: aboutWindow.close()
        }
    }

    Labs.FileDialog {
        id: openDialog
        folder: Labs.StandardPaths.writableLocation(Labs.StandardPaths.HomeLocation)
        fileMode: Labs.FileDialog.OpenFile
        title: qsTr("Please choose a file")
        nameFilters: [ qsTr("All files ")+"(*.trg *.jpg *.png *.bmp)",
                       qsTr("TRG files ")+"(*.trg)",
                       qsTr("Image files ")+"(*.jpg *.png *.bmp)"]

        onAccepted: {
            backEnd.openFile(file)
        }
    }

    Labs.FileDialog {
        id: saveDialog
        defaultSuffix: "trg"
        fileMode: Labs.FileDialog.SaveFile
        title: qsTr("Save file as...")
        nameFilters: [ qsTr("TRG files ")+"(*.trg)"]

        onAccepted: {
            backEnd.saveFile(file.toString().split("///").pop())
            message.send(qsTr("File saved"))
        }
    }

    menuBar: ControlsBar {id: controls}

    footer: Item {
        height: 20
        Text {
            id: pointMessage
            leftPadding: 10
            height: parent.height
            anchors.left: parent.left
            anchors.right: panelBtn.right
            font.bold: true
            color: mainWindow.textColor
            text: backendMsg
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            property string backendMsg: qsTr("Please open file")

            function loadBackendMsg() {
                text = Qt.binding(function(){return backendMsg})
            }

            Connections {
                target: backEnd
                onNewMsg: {
                    pointMessage.backendMsg = msg
                }
            }
        }

        CustomButton {
            id: panelBtn
            width: 140
            height: parent.height
            anchors.right: parent.right
            text: controls.panelSwitch.checked ? qsTr("Hide work panel") : qsTr("Show work panel")
            onReleased: controls.panelSwitch.trigger()
        }


    }

    Row {
        id: workRow
        anchors.fill: parent

        WorkZone {
            id: workZone
            height: parent.height
            width: workRow.width - rightPanel.width
        }

        Rectangle {
            height: parent.height
            width: 1
            color: mainWindow.controlColor
        }

        RightPanel {
            id: rightPanel
            height: parent.height
        }
    }

    Text {
        id: message
        anchors.centerIn: parent
        font.bold: true
        width: mainWindow.width-200
        font.pointSize: 100
        minimumPointSize: 10
        horizontalAlignment: Text.AlignHCenter
        fontSizeMode: Text.Fit
        color: "green"
        opacity: 0

        function fadeOut(text) {
            message.text = text
            msgFade.running = false
            message.opacity = 0
            msgFade.running = true
        }

        function send(text) {
            message.color = "green"
            fadeOut(text)
        }

        function sendErr(text) {
            message.color = "red"
            fadeOut(text)
        }

        Connections {
            target: backEnd
            onError: message.sendErr(errorMsg)
        }

        OpacityAnimator {
            id: msgFade
            easing.type: Easing.InCubic
            target: message
            from: 1
            to: 0
            duration: 1500
            running: false
        }
    }
}
