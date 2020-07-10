import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import Qt.labs.platform 1.0 as Labs

Window {
    id: parametersWindow
    visible: false
    title: qsTr("Parameters")
    minimumWidth: 440
    minimumHeight: 200
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    color: mainWindow.controlColor
    modality: Qt.ApplicationModal
    property real spacing: 20
    onVisibilityChanged: {
        if (visible) {
            lengthInput.parent.color = mainWindow.inputColor
            lengthInput.text = backEnd.getCalibrationLength()
            savePathInput.parent.color = mainWindow.inputColor
            var saveDir = backEnd.getConfig("saveDir")
            if (saveDir !== "default") {
                savePathInput.text = saveDir
            }
            var lang = backEnd.getConfig("lang")
            if (lang === "ru_RU") {
                control.currentIndex = 1
            }
            else {
                control.currentIndex = 0
            }
        }
    }

    function apply() {
        if (!lengthInput.isValueValid() || !savePathInput.isValueValid()) {
            return false
        }

        backEnd.setCalibrationLength(lengthInput.getValue())
        var dir = savePathInput.text === "" ? "default" : savePathInput.text
        backEnd.setConfig("saveDir", dir)
        switch (control.currentIndex) {
        case 1:
            backEnd.setLanguage("ru_RU")
            control.currentIndex = 1
            break
        default:
            backEnd.setLanguage("en_EN")
            control.currentIndex = 0
            break
        }
        workZone.sendSelectionMsg()

        return true
    }

    Labs.FolderDialog {
        id: savePathDialog
        folder: Labs.StandardPaths.writableLocation(Labs.StandardPaths.HomeLocation)
        title: qsTr("Please choose data saving directory")
        onAccepted: {
            savePathInput.text = folder.toString().split("///").pop()
        }
    }

    Rectangle {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: buttons.top
        anchors.margins: parametersWindow.spacing

        color: mainWindow.controlColor
        border.color: mainWindow.hControlColor

        Column {
            anchors.fill: parent
            spacing: parametersWindow.spacing/2
            anchors.topMargin: parametersWindow.spacing
            anchors.bottomMargin: parametersWindow.spacing

            Item {
                height: mainWindow.menuBarHeight
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: parametersWindow.spacing
                anchors.rightMargin: parametersWindow.spacing
                Text {
                    height: parent.height
                    anchors.left: parent.left
                    color: mainWindow.textColor
                    text: qsTr("Calibration length (mm):")
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle {
                    width: 40
                    height: parent.height
                    anchors.right: parent.right
                    border.color: "#000000"

                    TextInput {
                        id: lengthInput
                        anchors.centerIn: parent
                        clip: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        width: parent.width
                        text: backEnd.getCalibrationLength()
                        color: "#000000"
                        validator: RegExpValidator { regExp : /[0-9]+\.[0-9]+/ }
                        onTextEdited: {
                            if (isValueValid()) {
                                parent.color = mainWindow.inputColor
                            }
                            else {
                                parent.color = "red"
                            }
                        }

                        function isValueValid(){
                            return ((text !== "") && (parseFloat(text) !== 0))
                        }

                        function getValue() {
                            return parseFloat(text)
                        }
                    }
                }
            }

            Item {
                height: mainWindow.menuBarHeight
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: parametersWindow.spacing
                anchors.rightMargin: parametersWindow.spacing
                Text {
                    height: parent.height
                    anchors.left: parent.left
                    color: mainWindow.textColor
                    text: qsTr("Data saving path:")
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle {
                    width: 200
                    height: parent.height
                    anchors.right: openBtn.left
                    border.color: "#000000"

                    TextInput {
                        id: savePathInput
                        anchors.centerIn: parent
                        clip: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        width: parent.width
                        color: "#000000"
                        onTextEdited: {
                            if (isValueValid()) {
                                parent.color = mainWindow.inputColor
                            }
                            else {
                                parent.color = "red"
                            }
                        }

                        function isValueValid(){
                            return (backEnd.isDirectoryValid(text) || text === "")
                        }
                    }
                }

                CustomButton {
                    id: openBtn
                    text: "..."
                    width: 20
                    anchors.right: parent.right
                    onClicked: savePathDialog.open()
                }
            }

            Item {
                height: mainWindow.menuBarHeight
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: parametersWindow.spacing
                anchors.rightMargin: parametersWindow.spacing

                Text {
                    height: parent.height
                    anchors.left: parent.left
                    color: mainWindow.textColor
                    text: qsTr("Language:")
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                //FIXME: Customizing refactoring
                ComboBox {
                    id: control
                    width: 120
                    height: parent.height
                    anchors.right: parent.right
                    model: [qsTr("English"), qsTr("Russian")]

                    delegate: ItemDelegate {
                        width: control.width
                        height: 20
                        contentItem: Text {
                            text: modelData
                            color: "#000000"
                            font: control.font
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        highlighted: control.highlightedIndex === index
                        background: Rectangle {
                            color: parent.highlighted ? mainWindow.controlColor : "transparent"
                            border.color: parent.highlighted ?"#000000" : "transparent"
                        }
                    }

                    indicator: Canvas {
                        id: canvas
                        x: control.width - width - control.rightPadding
                        y: control.topPadding + (control.availableHeight - height) / 2
                        width: 8
                        height: 5
                        contextType: "2d"

                        onPaint: {
                            context.reset();
                            context.moveTo(0, 0);
                            context.lineTo(width, 0);
                            context.lineTo(width / 2, height);
                            context.closePath();
                            context.fillStyle = "#000000";
                            context.fill();
                        }
                    }

                    contentItem: Text {
                        leftPadding: 0
                        rightPadding: control.indicator.width + control.spacing

                        text: control.displayText
                        font: control.font
                        color: "#000000"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    background: Rectangle {
                        implicitWidth: 120
                        implicitHeight: 20
                        color: control.pressed ? mainWindow.controlColor : mainWindow.inputColor
                        border.color: "#000000"
                    }

                    popup: Popup {
                        y: control.height - 1
                        width: control.width
                        implicitHeight: contentItem.implicitHeight
                        padding: 0

                        contentItem: ListView {
                            implicitHeight: contentHeight
                            model: control.popup.visible ? control.delegateModel : null
                            currentIndex: control.highlightedIndex
                            interactive: false
                        }

                        background: Rectangle {
                            color: mainWindow.inputColor
                            border.color: "#000000"
                        }
                    }
                }
            }
        }
    }

    Row {
        id: buttons
        height: 30
        spacing: 10
        anchors.bottom: parent.bottom
        anchors.rightMargin: spacing
        anchors.right: parent.right

        CustomButton {
            text: qsTr("Ok")
            onClicked: {
                if (parametersWindow.apply()) {
                    parametersWindow.close()
                }
            }
        }

        CustomButton {
            id: cancelBtn
            text: qsTr("Cancel")
            onClicked: parametersWindow.close()
        }

        CustomButton {
            id: applyBtn
            text: qsTr("Apply")
            onClicked: {
                parametersWindow.apply()
            }
        }
    }
}
