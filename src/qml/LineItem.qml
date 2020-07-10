import QtQuick 2.11

//TODO: Show/hide lines

Item {
    id: line

    property color color

    property string pointName1
    property string pointName2

    property real x1: 0
    property real y1: 0
    property real x2: 1
    property real y2: 1

    property bool isSelected: false

    x: x1
    y: y1
    z: -1

    signal removed(var line)

    function movePoint1(x, y) {
        x1 = x
        y1 = y
    }

    function movePoint2(x, y) {
        x2 = x
        y2 = y
    }

    function calculateDistance() {
        return Math.sqrt(Math.pow(x2-x1, 2) + Math.pow(y2-y1, 2))
    }

    Rectangle {
        id: rect
        color: line.color
        height: calculateDistance()
        width: 1

        SequentialAnimation on color {
            id: lineAnimation
            running: line.isSelected && mainWindow.isBlinkAllowed
            loops: Animation.Infinite
            onRunningChanged: {
                if (!running) {
                    rect.color = line.color
                }
            }

            ColorAnimation { from: line.color; to: "white"; duration: mainWindow.blinkDuration }
            ColorAnimation { from: "white"; to: line.color;  duration: mainWindow.blinkDuration }
        }
    }

    Connections {
        target: parent

        onDeletePoint: {
            if (pointName === pointName1 || pointName === pointName2)
            {
                console.log("Destroing line", pointName1, pointName2)
                removed(line)
                pointName1 = "" //FIXME: Сrutch
                pointName2 = ""
                line.destroy()
            }
        }

        onScaled: rect.width = 1/scaleFactor

        onLinesDeselected: line.isSelected = false
    }


    Connections {
        target: rightPanel.linesZone
        onLineVisibilityChanged: {
            if (line.pointName1 === pointName1 && line.pointName2 === pointName2) {
                visible = isEnabled
            }
        }
    }

    transform: Rotation {

        function calculteAngle() {
            var rotationAngle = Math.acos(Math.abs(line.y2 - line.y1)/calculateDistance())/Math.PI*180
            if (line.x2 > line.x1) {
                if (line.y2 > line.y1) {
                    return -rotationAngle
                }
                else {
                    return rotationAngle-180
                }
            }
            else {
                if (line.y2 > line.y1) {
                    return rotationAngle
                }
                else {
                    return 180-rotationAngle
                }
            }
        }

        angle: calculteAngle()
    }
}
