import QtQuick 2.12
import QtQml 2.12
//TODO: Movable area near the point

Item {
    id: point
    width: size
    height: size
    scale: 1
    enabled: visible

    property alias name: nameText.text
    property alias drag: dragArea.drag
    property point beginDrag
    property bool isCaught: false
    property bool isNameAlwaysOn: false
    property bool isEntilted: true
    property bool isTilted: true
    property real size: 10
    property color color

    signal moving(var x, var y)
    signal moved(var name, var x, var y)
    signal hold(var isHolded)               //signal that mouse holds point
    signal removed()

    //FIXME: Crutch because of dynamic objects connections
    function disconnectLine(line) {
        if (name === line.pointName1) {
            moving.disconnect(line.movePoint1)
        }
        else if (name === line.pointName2) {
            moving.disconnect(line.movePoint2)
        }
    }

    Drag.active: dragArea.drag.active

    onXChanged: {
        moving(x+width/2, y+width/2)
    }

    onYChanged: {
        moving(x+width/2, y+width/2)
    }

    Connections {
        target: parent

        onDeletePoint: {
            if (pointName === point.name)
            {
                console.log("Destroing point", pointName)
                removed()
                point.name = "" //FIXME: Сrutch
                point.destroy()
            }
        }

        onEnableLock: {
            dragArea.drag.target = undefined
        }

        onDisableLock: {
            if (pointName === point.name)
            {
                dragArea.drag.target = point
            }
        }

        onScaled: scale = 1/scaleFactor

        onPointsNamesStateChanged: {
            if (isEntilted)
            {
                isNameAlwaysOn = isEnabled
                nameText.visible = isEnabled
            }
        }
    }

    MouseArea {
        id: dragArea
        anchors.fill: parent
        drag.target: point
        drag.threshold: 5
        hoverEnabled: true

        onEntered: if (!isNameAlwaysOn && isEntilted) nameText.visible = true
        onExited:  if (!isNameAlwaysOn && isEntilted) nameText.visible = false
        onDoubleClicked: {
            workZone.selectPoint(point.name)
            rightPanel.pointsZone.selectPoint(point.name)
            mainWindow.syncBlink()
        }

        onPressed: {
            if (drag.target === point) //if not locked
            {
                hold(true)
            }
            point.beginDrag = Qt.point(point.x, point.y)
        }

        onReleased: {
            if(!point.isCaught)
            {
                backAnimX.from = point.x
                backAnimX.to = beginDrag.x
                backAnimY.from = point.y
                backAnimY.to = beginDrag.y
                backAnim.start()
            }
            else
            {
                if (drag.target === point)
                {
                    hold(false)
                    moved(nameText.text, parent.x+parent.width/2, parent.y+parent.width/2)
                    console.log(parent.x+parent.width/2,parent.y+parent.width/2)
                }
            }
        }

        Item {
            id: cross
            anchors.fill: parent
            property color color

            Rectangle {
                width: 2
                height: parent.width
                color: parent.color
                anchors.centerIn: parent
                rotation: point.isTilted ? 45 : 0
            }

            Rectangle {
                width: parent.width
                height: 2
                color: parent.color
                anchors.centerIn: parent
                rotation: point.isTilted ? 45 : 0
            }

            SequentialAnimation on color{
                id: pointAnimation
                running: dragArea.drag.target === point && mainWindow.isBlinkAllowed
                loops: Animation.Infinite
                onRunningChanged: {
                    if (!running) {
                        cross.color = point.color
                    }
                }

                ColorAnimation { from: point.color; to: "white"; duration: mainWindow.blinkDuration }
                ColorAnimation { from: "white"; to: point.color;  duration: mainWindow.blinkDuration }
            }
        }

//        Canvas {
//            id: cross
//            anchors.fill: parent
//            rotation: point.isTilted ? 45 : 0

//            onPaint: {
//                var w = parent.width
//                var ctx = getContext("2d")
//                ctx.lineWidth = 1.4
//                ctx.miterLimit = 0.1
//                ctx.strokeStyle = point.color
//                ctx.beginPath()
//                ctx.moveTo(w/2, 0)
//                ctx.lineTo(w/2, w)
//                ctx.stroke()
//                ctx.moveTo(0, w/2)
//                ctx.lineTo(w, w/2)
//                ctx.stroke()
//            }
//        }

        ParallelAnimation {
            id: backAnim
            SpringAnimation { id: backAnimX; target: point; property: "x"; duration: 200; spring: 5; damping: 0.5 }
            SpringAnimation { id: backAnimY; target: point; property: "y"; duration: 200; spring: 5; damping: 0.5 }
        }

        Text {
            id: nameText
            anchors.horizontalCenter: parent.horizontalCenter
            y: parent.y - point.size*1.8
            font.pixelSize: point.size*1.5
            color: cross.color
            visible: point.isNameAlwaysOn

        }
    }
}
