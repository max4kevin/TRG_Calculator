import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12

//TODO: warning message about calibration

Item {
    id: resultsZone

    Rectangle {
        id: resultTitle
        width: parent.width
        height: 30
        color: mainWindow.backgroundColor

        Row {
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter
            spacing: 0
            height: parent.height

            Text {
                width: parent.width*3/5
                height: parent.height
                horizontalAlignment:  Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: mainWindow.textColor
                clip: true
                font.bold: true
                text: qsTr("Description")
//                wrapMode: Text.WordWrap
            }

            Rectangle {
                width: 1
                height: parent.height
                color: mainWindow.controlColor
            }

            Text {
                width: parent.width/5
                height: parent.height
                horizontalAlignment:  Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: mainWindow.textColor
                clip: true
                font.bold: true
                text: qsTr("Value")
            }

            Rectangle {
                width: 1
                height: parent.height
                color: mainWindow.controlColor
            }

            Text {
                width: parent.width/5
                height: parent.height
                horizontalAlignment:  Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: mainWindow.textColor
                clip: true
                font.bold: true
                text: qsTr("Reference")
            }
        }
    }

    ListView {
        id: resultsList
        width: parent.width
//        anchors.topMargin: rightPanel.spacing
        anchors.top: resultTitle.bottom
        anchors.bottom: storeResultsBtn.top
        orientation: Qt.Vertical
        spacing: 0
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        delegate: Row {
            id: resultLine
            anchors.left: parent.left
            anchors.right: parent.right
            height: 30
            spacing: 0
            property string name: name

            Text {
                id: descriptionText
                width: parent.width*3/5
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                leftPadding: rightPanel.spacing
                clip: true
                color: mainWindow.textColor
                text: description
            }

            Rectangle {
                width: 1
                height: parent.height
                color: mainWindow.controlColor
            }

            Text {
                id: valueText
                width: parent.width/5
                height: parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true
                color: mainWindow.textColor
                text: value
            }

            Rectangle {
                width: 1
                height: parent.height
                color: mainWindow.controlColor
            }

            Text {
                id: referenceText
                width: parent.width/5
                height: parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true
                color: mainWindow.textColor
                text: reference
            }
        }

        model: ListModel {
            id: results
        }

        ScrollBar.vertical: CustomScrollBar {
               visible: parent.width !== 0
        }

        Connections {
            target: backEnd
            onResultAdded: {
                for (var i = 0; i < results.count; ++i) {
                    if (results.get(i).name === resultName) {
                        results.setProperty(i, "description", description)
                        results.setProperty(i, "reference", resultReference)
                        return
                    }
                }
                results.append({name: resultName, description: description, value: "", reference: resultReference})
                console.log("Result \""+resultName+"\" added")
            }

            onResultUpdated: {
                for (var i = 0; i < results.count; ++i) {
                    if (results.get(i).name === resultName) {
                        results.get(i).reference = resultReference
                        results.get(i).value = resultValue
                        return
                    }
                }
                console.log("Error: Result \""+resultName+"\" not found in table")
            }

            onClearTables: {
                results.clear()
            }
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
