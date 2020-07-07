import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    id: linesZone

    property alias isNewVisible: showNewSwitch.checked

    function deselectLine() {
        linesList.isSelected = false
    }

    //FIXME: Megacrutch (because of dynamic item to delegate signals bag - not all signals catch!)
    function addLine(pointName1, pointName2) {
        for (var i = 0; i < lines.count; ++i) {
            if (lines.get(i).pointName1 === pointName1 && lines.get(i).pointName2 === pointName2) {
                return
            }
            if (lines.get(i).pointName1.charAt(0) > pointName1.charAt(0)) {
                lines.insert(i, {pointName1: pointName1, pointName2: pointName2, isVisible: isNewVisible})
                return
            }
        }
        if (i === lines.count) {
            lines.append({pointName1: pointName1, pointName2: pointName2, isVisible: isNewVisible})
        }
    }

    //FIXME: Can't move the signal to delegate because of catching delegate signal bug
    signal lineVisibilityChanged(var pointName1, var pointName2, var isEnabled)

    ListModel {id: lines}

    ListView {
        id: linesList
        width: parent.width
        anchors.top: parent.top
//        anchors.bottom: buttons.top
        anchors.bottom: buttons.top
        orientation: Qt.Vertical
        spacing: rightPanel.spacingValue
        clip: true
        highlightMoveDuration: 0
        boundsBehavior: Flickable.StopAtBounds

        property bool isSelected: false

        delegate: MouseArea {
            id: lineRow
            height: 20
            anchors.left: parent.left
            anchors.right: parent.right

//            onClicked: {
//                if (pointsList.currentIndex === index) {
//                    if (pointsList.isSelected) {
//                        workZone.deselectPoint()
//                        return
//                    }
//                }
//                else {
//                    pointsList.currentIndex = index
//                }
//                pointsList.isSelected = true
//                workZone.selectPoint(nameText.text)
//                workZone.focusOnPoint(nameText.text)
//            }

            Text {
                id: nameText

                height: 20
                anchors.left: hideBtn.right
                anchors.right: parent.right
                anchors.leftMargin: rightPanel.spacing
                anchors.rightMargin: rightPanel.spacing
                clip: true
                color: (pointsZone.currentIndex === index) && (pointsZone.isSelected) ? mainWindow.hTextColor : mainWindow.textColor
                verticalAlignment: Text.AlignVCenter
                text: name1+"-"+name2

                property string name1: pointName1
                property string name2: pointName2
            }

            CheckBox {
                id: hideBtn
                checked: isVisible
                width: 20
                anchors.left: parent.left
                anchors.leftMargin: rightPanel.spacing
                anchors.rightMargin: rightPanel.spacing
                display: AbstractButton.IconOnly

                indicator: Item {
                    implicitWidth: 20
                    implicitHeight: 20
                    Rectangle {
                        width: 13
                        height: 13
                        anchors.centerIn: parent
                        opacity: enabled ? 1.0 : 0.3
                        color: hideBtn.highlighted ? mainWindow.hControlColor : mainWindow.controlColor
                        border.color: hideBtn.highlighted ? mainWindow.hTextColor : mainWindow.textColor
                        radius: 3
                        Rectangle {
                           width: 5
                           height: 5
                           anchors.centerIn: parent
                           visible: hideBtn.checked
                           opacity: enabled ? 1.0 : 0.3
                           color: hideBtn.highlighted ? mainWindow.hTextColor : mainWindow.textColor
                           radius: 2
                        }
                    }
                }

                onCheckStateChanged: {
                    if (lines.get(index).isVisible !== checked)
                    {
                        lines.setProperty(index, "isVisible", checked)
                        linesZone.lineVisibilityChanged(nameText.name1, nameText.name2, checked)
                    }
                }
            }
        }
        model: lines
    }
    Row {
        id: buttons
        height: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: showNewSwitch.top
//        anchors.bottom: parent.bottom

        CustomButton {
            id: showAllBtn
            height: parent.height
            width: parent.width/2
            text: qsTr("Show all")
            enabled: lines.count > 0
            onClicked: {
                for (var i = 0; i < lines.count; ++i)
                {
                    lines.setProperty(i, "isVisible", true)
                    linesZone.lineVisibilityChanged(lines.get(i).pointName1, lines.get(i).pointName2, true)
                }
            }
        }

        CustomButton {
            id: hideAllBtn
            height: parent.height
            width: parent.width/2
            text: qsTr("Hide all")
            enabled: lines.count > 0
            onClicked: {
                for (var i = 0; i < lines.count; ++i)
                {
                    lines.setProperty(i, "isVisible", false)
                    linesZone.lineVisibilityChanged(lines.get(i).pointName1, lines.get(i).pointName2, false)
                }
            }
        }
    }

    Switch {
        id: showNewSwitch
        height: 20
        width: implicitWidth
        checked: true
        anchors.bottom: parent.bottom
    }

    //FIXME: Megacrutch (because of dynamic item to delegate signals bag - not all signals catch!)
    Connections {
        target: backEnd
        onPointUpdated: {
            if (!status) {
                for (var i = 0; i < lines.count; ++i) {
                    if (pointName === lines.get(i).pointName1 || pointName === lines.get(i).pointName2) {
                        lines.remove(i)
                    }
                }
            }
        }

        onPointsConnected: {
            addLine(pointName1, pointName2)
        }
    }
}
