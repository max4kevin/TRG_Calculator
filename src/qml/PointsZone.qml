import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    id: pointsZone

    ListView {
        id: resultsList
        width: parent.width
//        anchors.topMargin: rightPanel.spacing
        anchors.top: resultTitle.bottom
        anchors.bottom: parent.bottom
        orientation: Qt.Vertical
        spacing: rightPanel.spacingValue

        delegate: Row {
            id: resultLine
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: rightPanel.spacing
            anchors.rightMargin: rightPanel.spacing
            spacing: rightPanel.spacingValue

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
