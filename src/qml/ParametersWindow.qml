import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3

Window {
    id: parametersWindow
    visible: false
    title: qsTr("Parameters")
    minimumWidth: 300
    minimumHeight: 130
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    color: mainWindow.controlColor
    modality: Qt.ApplicationModal
    property real spacing: 20
    onVisibilityChanged: {
        if (visible) {
            lengthInput.parent.color = mainWindow.inputColor
            lengthInput.text = backEnd.getCalibrationLength()
        }
    }

    function apply() {
        if (!lengthInput.isValueValid()) {
            return false
        }
        backEnd.setCalibrationLength(lengthInput.getValue())
        return true
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
