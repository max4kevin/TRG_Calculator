import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    id: pointsZone

    function deselectPoint() {
        pointsList.isSelected = false
    }

    function showNextPoint() {

    }

    function selectPoint(pointName) {
        for (var i = 0; i < points.count; ++i) {
            if (points.get(i).name === pointName) {
                pointsList.currentIndex = i
                pointsList.isSelected = true
                pointsList.positionViewAtIndex(i, ListView.Center)
                return
            }
        }
        console.log("Error: "+pointName+" not found in PointsList")
    }

    Connections {
        target: workZone
        onPointDeselected: deselectPoint()
    }

    ListView {
        id: pointsList
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: buttons.top
        orientation: Qt.Vertical
        spacing: rightPanel.spacingValue
        clip: true
        highlightMoveDuration: 0
        boundsBehavior: Flickable.StopAtBounds

        property bool isSelected: false

        //FIXME: Scrolling bug
        highlight: Rectangle {
            id: selection
            width: pointsList.width
            visible: true
            color: "transparent"
            border.color: mainWindow.hControlColor
            SequentialAnimation on color{
                running: pointsList.isSelected
                loops: Animation.Infinite
                onRunningChanged: {
                    if (!running)
                    {
                        selection.color = "transparent"
                    }
                }

                ColorAnimation { from: mainWindow.controlColor; to: mainWindow.hControlColor; duration: 1000 }
                ColorAnimation { from: mainWindow.hControlColor; to: mainWindow.controlColor;  duration: 1000 }
            }
        }

        delegate: MouseArea {
            id: pointRow
            height: 50
            anchors.left: parent.left
            anchors.right: parent.right
            property bool isReady: readyStatus

            onClicked: {
                if (pointsList.currentIndex === index) {
                    if (pointsList.isSelected) {
                        workZone.deselectPoint()
                        return
                    }
                }
                else {
                    pointsList.currentIndex = index
                }
                pointsList.isSelected = true
                workZone.selectPoint(nameText.text)
                if (pointRow.isReady) {
                    workZone.focusOnPoint(nameText.text)
                }
            }

            Row {
                id: pointStatus
                height: 20
                anchors.left: parent.left
                anchors.right: deleteBtn.left
                anchors.leftMargin: rightPanel.spacing
                anchors.rightMargin: rightPanel.spacing
                spacing: rightPanel.spacingValue

                Rectangle {
                     width: 10
                     height: width
                     color: pointRow.isReady? "green" : "red"
                     radius: width*0.5
                     anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: nameText
                    width: implicitWidth
                    height: 20
                    clip: true
                    color: (pointsList.currentIndex === index) && (pointsList.isSelected) ? mainWindow.hTextColor : mainWindow.textColor
                    text: name
                    verticalAlignment: Text.AlignVCenter
                }
            }

            CustomButton {
                id: deleteBtn
                height: 15
                width: 50
                text: qsTr("Delete") //TODO: Icon
                anchors.right: parent.right
                anchors.rightMargin: rightPanel.spacing
                anchors.leftMargin: rightPanel.spacing
                anchors.verticalCenter: pointStatus.verticalCenter
                onClicked: {
                    backEnd.removePoint(nameText.text)
                    message.send(nameText.text+qsTr(" point deleted"))
                }
            }

            //TODO: Property button (for choosing color, etc.)

            Text {
                id: pointDescript
                height: implicitHeight
                anchors.top: pointStatus.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: rightPanel.spacing
                anchors.rightMargin: rightPanel.spacing
                wrapMode: Text.WordWrap
                text: description
                clip: true
                color: nameText.color
                verticalAlignment: Text.AlignVCenter

            }
        }

        model: ListModel {
            id: points
        }

        ScrollBar.vertical: ScrollBar {
               active: true
               visible: parent.width !== 0
        }

        Connections {
            target: backEnd
            onPointAdded : {
                points.append({name: pointName, description: description, readyStatus: false})
                console.log("PointLine \""+pointName+"\" added")
            }

            onPointUpdated: {
                if (isVisible) {
                    for (var i = 0; i < points.count; ++i)
                    {
                        if (points.get(i).name === pointName)
                        {
                            points.get(i).readyStatus = status
                            return
                        }
                    }
                    console.log("Error: PointLine \""+pointName+"\" not found in table")
                }
            }
            onClearTables: {
                points.clear()
            }
        }
    }

    Row {
        id: buttons
        height: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        CustomButton {
            height: parent.height
            width: parent.width/2
            text: qsTr("Delete points")
            enabled: points.count > 0
            onClicked: {
                backEnd.clear()
                message.send(qsTr("Points deleted"))
            }
        }

        CustomButton {
            height: parent.height
            width: parent.width/2
            text: qsTr("Delete all points")
            enabled: points.count > 0
            onClicked: {
                backEnd.clearAll()
                message.send(qsTr("All points deleted"))
            }
        }
    }
}
