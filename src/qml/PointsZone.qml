import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    id: pointsZone

    function deselectPoint() {
        pointsList.isSelected = false
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
        console.log(pointName+" not found in PointsList")
    }

    Connections {
        target: workZone
        onPointDeselected: deselectPoint()
    }

    ListView {
        id: pointsList
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        orientation: Qt.Vertical
        spacing: rightPanel.spacingValue
        clip: true
        highlightMoveDuration: 0
        boundsBehavior: Flickable.StopAtBounds

        property bool isSelected: false

        //TODO: Blink animation
        //TODO: Disabling selection on another tab
        //FIXME: Scrolling bug
        highlight: Rectangle {
            id: selection
            width: pointsList.width
            visible: pointsList.isSelected
            border.color: mainWindow.backgroundColor
            SequentialAnimation on color{
                running: pointsList.isSelected
                loops: Animation.Infinite
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
                workZone.focusOnPoint(nameText.text)
            }

            Row {
                id: pointStatus
                height: 20
                anchors.left: parent.left
                anchors.right: parent.right
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
                    width: parent.width-10
                    height: 20
                    clip: true
                    color: (pointsList.currentIndex === index) && (pointsList.isSelected) ? mainWindow.hTextColor : mainWindow.textColor
                    text: name
                    verticalAlignment: Text.AlignVCenter
                }
            }

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
                color: pointsList.currentIndex === index ? mainWindow.hTextColor : mainWindow.textColor
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
            onPointUpdated: {
                if (isVisible) {
                    for (var i = 0; i < points.count; ++i)
                    {
                        if (points.get(i).name === pointName)
                        {
                            points.get(i).description = description
                            points.get(i).readyStatus = status
                            return
                        }
                    }
                    points.append({name: pointName, description: description, readyStatus: status})
                    console.log("PointLine \""+pointName+"\" added")
                }
            }
            onClearTable: {
                points.clear()
            }
        }
    }
}
