import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    property bool isReady: imageTRG.status === Image.Ready

    function zoom() {
        if (imageTRG.scale !== mouseArea.maxScaleFactor)
        {
            mouseArea.scale(mouseArea.maxScaleFactor/10, mouseArea.mouseX, mouseArea.mouseY)
        }
    }

    function unzoom() {
        mouseArea.scale(-mouseArea.maxScaleFactor/10, mouseArea.mouseX, mouseArea.mouseY)
    }

    function closeImage() {
        imageTRG.close()
        pointMessage.text = qsTr("Please open file")
    }

    function invertImage() {
        var contentX = flickArea.contentX
        var contentY = flickArea.contentY
        backEnd.invertImage()
        flickArea.contentX = contentX
        flickArea.contentY = contentY
    }

    function showPointsNames(isEnabled) {
        mouseArea.pointsNamesStateChanged(isEnabled)
    }

    function showLines(isEnabled) {
        mouseArea.linesStateChanged(isEnabled)
    }

    function changeLock(isEnabled) {
        if (isEnabled) {
            mouseArea.enableLock()
            message.send(qsTr("Points locked"))
        }
        else {
            mouseArea.disableLock()
            message.send(qsTr("Points unlocked"))
        }
    }

    //TODO: Custom files path and name
    function saveData() {
        backEnd.saveData(backEnd.getFilePath(), backEnd.getFileName())
        var scale = 800/imageTRG.width
        mouseArea.scaled(scale)
        imageTRG.grabToImage(function(result) {
            var path = backEnd.getFilePath()
            result.saveToFile(path+backEnd.getFileName()+"_result.png")
            mouseArea.scaled(imageTRG.scale)
            path = path.slice(0,path.length-1)
            message.send(qsTr("Result stored in .../")+path.split("/").pop())
        })
    }

    Menu {
        id: pointsMenu
        delegate: MenuDelegate {}
        background: ControlBackground {}

        MenuDelegate {
            action: controls.lockSwitch
        }

        MenuDelegate {
            action: controls.pointsSwitch
        }

        MenuDelegate {
            action: controls.linesSwitch
        }

    }

    Flickable {
        id: flickArea
        anchors.fill: parent
        focus: true
        clip: true
        contentWidth: Math.max(imageTRG.width * imageTRG.scale, width)
        contentHeight: Math.max(imageTRG.height  * imageTRG.scale, height)
        anchors.centerIn: parent
        boundsBehavior: Flickable.StopAtBounds
        interactive: false

        function switchKeys(isHolded) {
            Keys.enabled = !isHolded
        }

        ScrollBar.vertical: ScrollBar {
               active: true
        }

        ScrollBar.horizontal: ScrollBar {
               active: true
        }

        Keys.onSpacePressed: {
            if(!event.isAutoRepeat)
            {
                interactive = true
                boundsBehavior = Flickable.DragAndOvershootBounds
                mouseArea.cursorShape = Qt.OpenHandCursor
                if (!switchPointsLock.checked)
                {
                    mouseArea.enableLock()
                }
            }
        }

        Keys.onReleased: {
            if (event.key === Qt.Key_Space && !event.isAutoRepeat)
            {
                interactive = false
                boundsBehavior = Flickable.StopAtBounds
                mouseArea.cursorShape = Qt.CrossCursor
                if (!switchPointsLock.checked)
                {
                    mouseArea.disableLock()
                }
            }
        }

        onMovementEnded: {
            if (interactive)
            {
                mouseArea.cursorShape = Qt.OpenHandCursor
            }
        }

        DropArea {
            id: imageDropArea
            anchors.fill: parent

            function checkFileExtension(filePath) {
                        return (filePath.split('.').pop() === "jpg")||
                        (filePath.split('.').pop() === "png")||
                        (filePath.split('.').pop() === "bmp")||
                        (filePath.split('.').pop() === "trg")
            }

            //TODO: Check file saved

            function dropImage(drop) {
                if (checkFileExtension(drop.text)) {
                    backEnd.openFile(drop.text)
                }
                else {
                    message.sendErr(qsTr("Unsupported file format"))
                }
            }

            onDropped: {
                dropImage(drop)
            }
        }

        Image {
            id: imageTRG
            scale: 1
            transformOrigin: Item.Center
            anchors.centerIn: parent

            property string filePath: source

            function open()
            {
                update()
                message.send(qsTr("File loaded"))
            }

            function update() {
                source = ""
                source = "image://imageProvider/img"
            }

            function close()
            {
                source = ""
                message.send(qsTr("File closed"))
                backEnd.reset()
                pointMessage.text = ""
            }

            Connections {
                target: backEnd
                onFileLoaded: imageTRG.open()
                onImageUpdated: imageTRG.update()
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.CrossCursor
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                property real pointsSize: 12
                property real maxScaleFactor: 5

                signal scaled(var scaleFactor)
                signal deletePoint(var pointName)
                signal enableLock
                signal disableLock
                signal pointsNamesStateChanged(var isEnabled)
                signal linesStateChanged(var isEnabled)

                function scale(scaleIncrement, focusX, focusY)
                {
                    var offsetX = focusX*imageTRG.scale - flickArea.contentX
                    var offsetY = focusY*imageTRG.scale - flickArea.contentY
                    imageTRG.scale += scaleIncrement
                    if (imageTRG.scale > maxScaleFactor)
                    {
                        imageTRG.scale = maxScaleFactor
                    }
                    else
                    {
                        if (imageTRG.scale < 0.5*flickArea.height/parent.sourceSize.height)
                        {
                            imageTRG.scale = 0.5*flickArea.height/parent.sourceSize.height
                        }
                    }
                    scaled(imageTRG.scale)
                    flickArea.contentX = flickArea.contentWidth === flickArea.width ? 0 : focusX*imageTRG.scale - offsetX
                    flickArea.contentY = flickArea.contentHeight === flickArea.height ? 0 : focusY*imageTRG.scale - offsetY
                }

                function pointMoved(pointName, x, y) {
                    backEnd.movePoint(pointName, x, y)
                }

                function updatePoint(pointName, color, x, y, isTilted, isEntilted, isVisible) {
                    if (imageTRG.status === Image.Ready) {
                        for (var i = 0; i < mouseArea.children.length; ++i) {
                            if (mouseArea.children[i].name === pointName)
                            {
                                console.log("Updating point", pointName)
                                mouseArea.children[i].name = pointName
                                mouseArea.children[i].color = color
                                mouseArea.children[i].isTilted = isTilted
                                mouseArea.children[i].isEntilted = isEntilted
                                mouseArea.children[i].visible = isVisible
                                mouseArea.children[i].size = mouseArea.pointsSize
                                mouseArea.children[i].x = x - mouseArea.children[i].width/2
                                mouseArea.children[i].y = y - mouseArea.children[i].height/2
                                return
                            }
                        }

                        var component = Qt.createComponent("PointItem.qml")
                        if (component.status === Component.Ready) {
                            console.log("Drawing point", pointName)
                            var childRec = component.createObject(mouseArea)
                            childRec.name = pointName
                            childRec.color = color
                            childRec.isTilted = isTilted
                            childRec.isEntilted = isEntilted
                            childRec.visible = isVisible
                            childRec.size = mouseArea.pointsSize
                            childRec.x = x - childRec.width/2
                            childRec.y = y - childRec.height/2
                            childRec.scale = 1/imageTRG.scale
                            if (controls.lockSwitch.checked)
                            {
                                childRec.drag.target = undefined
                            }
                            childRec.isNameAlwaysOn = (controls.pointsSwitch.checked && isTilted)
                            childRec.hold.connect(flickArea.switchKeys)
                            childRec.moved.connect(pointMoved)
                        }
                    }
                }

                function connectPoints(pointName1, pointName2, color)
                {
                    if (imageTRG.status === Image.Ready) {
                        for (var i = 0; i < mouseArea.children.length; ++i)
                        {
                            if ( (mouseArea.children[i].pointName1 === pointName1)&&
                                    (mouseArea.children[i].pointName2 === pointName2) )
                            {
//                                console.log("Line is actualy exist")
                                mouseArea.children[i].color = color
                                return
                            }
                        }
                        var i1 = -1
                        var i2 = -1
                        for (i = 0; i < mouseArea.children.length; ++i) {
                            if (mouseArea.children[i].name === pointName1)
                            {
                                i1 = i
                            }
                            else if (mouseArea.children[i].name === pointName2)
                            {
                                i2 = i
                            }
                        }
                        if ((i1 === -1)||(i2 === -1)) {
                            var msg = ""
                            if (i1 === -1) {
                                msg += " "+pointName1
                            }
                            if (i2 === -1) {
                                msg += " "+pointName2
                            }
                            console.log("Line is not created - points"+msg+" not found")
                            return
                        }

                        var component = Qt.createComponent("LineItem.qml")
                        if (component.status === Component.Ready) {
                            var childRec = component.createObject(mouseArea)
                            childRec.pointName1 = pointName1
                            childRec.pointName2 = pointName2
                            childRec.x1 = mouseArea.children[i1].x+mouseArea.children[i1].width/2
                            childRec.y1 = mouseArea.children[i1].y+mouseArea.children[i1].height/2
                            childRec.x2 = mouseArea.children[i2].x+mouseArea.children[i2].width/2
                            childRec.y2 = mouseArea.children[i2].y+mouseArea.children[i2].height/2
                            childRec.color = color
                            childRec.width = 1/imageTRG.scale
                            childRec.visible = controls.linesSwitch.checked
                            mouseArea.children[i1].moving.connect(childRec.movePoint1)
                            mouseArea.children[i2].moving.connect(childRec.movePoint2)
                        }
                     }
                }

                onEntered: {
                    flickArea.focus = true;
                }

                onPressed: {
                    if (flickArea.interactive)
                    {
                        mouseArea.cursorShape = Qt.ClosedHandCursor
                    }
                }

                onReleased: {
                    if (flickArea.interactive)
                    {
                        mouseArea.cursorShape = Qt.OpenHandCursor
                    }
                }

                onClicked: {
                    if  (!flickArea.interactive) {
                        if (mouse.button === Qt.LeftButton) {
                            backEnd.writeCoordinates(mouseArea.mouseX, mouseArea.mouseY)
                        }
                        else {
                            pointsMenu.popup()
                        }
                    }
                }

                onWheel: {
                    if (wheel.modifiers & Qt.ControlModifier) {
                        wheel.accepted = true
                        var mouseY = mouseArea.mouseY
                        mouseArea.scale(0.2 * wheel.angleDelta.y / 140, mouseArea.mouseX, mouseArea.mouseY)
                    }
                    else
                    {
                        if (!flickArea.interactive) {
                            flickArea.flick(wheel.angleDelta.x*50, wheel.angleDelta.y*50)
                        }
                    }
                }

                DropArea {
                    id: pointsDropArea
                    anchors.fill: parent
                    onEntered: {
                        if (!drag.hasText) {
                            drag.source.isCaught = true
                        }
                    }

                    onExited: {
                        if (!drag.hasText) {
                            drag.source.isCaught = false
                        }
                    }

                    onDropped: {
                        if (drop.hasText) {
                            imageDropArea.dropImage(drop)
                        }
                    }
                }

                Connections {
                    target: backEnd
                    onPointUpdated: {
                        mouseArea.updatePoint(pointName, color, x, y, isTilted, isEntilted, isVisible)
                    }

                    onPointDeleted: {
                        mouseArea.deletePoint(pointName)
                    }

                    onPointsConnected: {
                        mouseArea.connectPoints(pointName1, pointName2, color)
                    }
                }
            }
        }
    }
}
