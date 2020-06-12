import QtQuick 2.11
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.0 as Labs

//TODO: Ask for saving before closing
//TODO: Custom data storing path
//TODO: Calculation method choosing
//TODO: Laguage choosing
//TODO: Settings data in resourses


ApplicationWindow {
    id: mainWindow
    visible: true
    title: qsTr("TRG Calculator")
    width: 1040
    height: 726
    minimumWidth: 640
    minimumHeight: 480
    property int menuBarHeight: 20
    property bool darkTheme: controls.isDark
    property string backgroundColor: mainWindow.darkTheme ? "#000000" : "#EEEEEE"
    property string controlColor: mainWindow.darkTheme ? "#222222" : "#CCCCCC"
    property string hControlColor: mainWindow.darkTheme ? "#888888" : "#888888"
    property string textColor: mainWindow.darkTheme ? "#AAAAAA" : "#222222"
    property string hTextColor: mainWindow.darkTheme ? "#FFFFFF" : "#000000"
    property string inputColor: mainWindow.darkTheme ? "#888888" : "#FFFFFF"
    property string version: "2.1.0"

    background: Rectangle {
        anchors.fill: parent
        color: backgroundColor
    }

    ParametersWindow {
        id: parametersWindow
    }

    Window {
        id: aboutWindow
        title: qsTr("About")
        flags: Qt.FramelessWindowHint
        visible: false
        minimumWidth: 200
        minimumHeight: 130
        maximumWidth: minimumWidth
        maximumHeight: minimumHeight
        color: mainWindow.controlColor
        modality: Qt.ApplicationModal
        Text {
            text: qsTr("TRG calculation program.\nVersion ")+mainWindow.version+qsTr("\nCreated by Kirill Maksimov")
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

    footer:
        //TODO: Fix text areas

        Text {
            id: pointMessage
            leftPadding: 10
            height: 20
            font.bold: true
            color: mainWindow.textColor
            text: qsTr("Please open file")
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Connections {
                target: backEnd
                onNewMsg: {
                    pointMessage.text = msg
                }
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
        font.pointSize: mainWindow.width/25
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
