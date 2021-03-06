import QtQuick 2.11
import QtQuick.Controls 2.4

Rectangle {
    id: resultsZone
    color: mainWindow.backgroundColor
    property real animationDuration: 100
    property real spacingValue: 10
    property real spacing: width > spacingValue ? spacingValue : 0

    function expand() {
        collapseAnimation.stop()
        expandAnimation.start()
    }

    function collapse() {
        expandAnimation.stop()
        collapseAnimation.start()
    }

    PropertyAnimation {
        id: expandAnimation
        target: resultsZone
        property: "width"
        to: 450
        duration: resultsZone.animationDuration
    }

    PropertyAnimation {
        id: collapseAnimation
        target: resultsZone
        property: "width"
        to: 0
        duration: resultsZone.animationDuration
    }

    Rectangle {
        id: resultTitle
        anchors.left: parent.left
        anchors.right: parent.right
        height: 30
        color: mainWindow.backgroundColor

        Row {
            width: parent.width
            anchors.leftMargin: resultsZone.spacing
            anchors.rightMargin: resultsZone.spacing
            anchors.verticalCenter: parent.verticalCenter
            spacing: resultsZone.spacingValue

            Text {
                width: parent.width/3
                horizontalAlignment: Text.AlignHCenter
                color: mainWindow.textColor
                clip: true
                font.bold: true
                text: qsTr("Name")
            }

            Text {
                width: parent.width/3
                horizontalAlignment: Text.AlignHCenter
                color: mainWindow.textColor
                clip: true
                font.bold: true
                text: qsTr("Value")
            }

            Text {
                width: parent.width/3
                horizontalAlignment: Text.AlignHCenter
                color: mainWindow.textColor
                clip: true
                font.bold: true
                text: qsTr("Reference value")
            }
        }
    }

    ListView {
        id: resultsList
        width: parent.width
//        anchors.topMargin: resultsZone.spacing
        anchors.top: resultTitle.bottom
        anchors.bottom: parent.bottom
        orientation: Qt.Vertical
        spacing: resultsZone.spacingValue

        delegate: Row {
            id: resultLine
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: resultsZone.spacing
            anchors.rightMargin: resultsZone.spacing
            spacing: resultsZone.spacingValue

            Text {
                id: nameText
                width: parent.width/3
                clip: true
                color: mainWindow.textColor
                text: name
            }

            Text {
                id: valueText
                width: parent.width/3
                horizontalAlignment: Text.AlignHCenter
                clip: true
                color: mainWindow.textColor
                text: value
            }

            Text {
                id: referenceText
                width: parent.width/3
                horizontalAlignment: Text.AlignHCenter
                clip: true
                color: mainWindow.textColor
                text: reference
            }
        }

        model: ListModel {
            id: results
        }

        ScrollBar.vertical: ScrollBar {
               active: true
               visible: parent.width !== 0
        }

        Connections {
            target: backEnd
            onResultUpdated: {
                for (var i = 0; i < results.count; ++i)
                {
                    if (results.get(i).name === resultName)
                    {
                        results.get(i).reference = resultReference
                        results.get(i).value = resultValue
                        return
                    }
                }
                results.append({name: resultName, value: resultValue, reference: resultReference})
                console.log("Result \""+resultName+"\" added")
            }

            onClearTable: {
                results.clear()
            }
        }

        CustomButton {
            id: storeResultsBtn
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            text: qsTr("Store results")
            enabled: workZone.isReady

            //TODO: Custom store path
            onClicked: {
                workZone.saveData()
            }
        }
    }
}

