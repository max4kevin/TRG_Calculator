import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    id: workZone

    property bool isReady: imageTRG.status === Image.Ready

    //FIXME: crutch
    function sendSelectionMsg() {
        if (mouseArea.selectedPoint !== "") {
            pointMessage.text = qsTr("Point ")+mouseArea.selectedPoint+qsTr(" selected")
        }
    }

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

    function showLine(pointName1, pointName2, isEnabled) {
        mouseArea.linesStateChanged(isEnabled)
    }

    function selectPoint(pointName) {
        mouseArea.selectedPoint = pointName
        mouseArea.enableLock()
        mouseArea.disableLock(pointName)
//        pointMessage.text = qsTr("Point ")+pointName+qsTr(" selected")
        sendSelectionMsg()
    }

    function deselectPoint() {
        mouseArea.selectedPoint = ""
        mouseArea.enableLock()
        pointMessage.loadBackendMsg()
    }

    function deselectLine() {
        mouseArea.linesDeselected()
    }

    function focusOnPoint(pointName) {
        for (var i = 0; i < mouseArea.children.length; ++i) {
            if (mouseArea.children[i].name === pointName) {
                mouseArea.focusOn(mouseArea.children[i].x, mouseArea.children[i].y)
                return
            }
        }
        console.log(pointName+" point not found when focused")
    }

    function focusOnLine(pointName1, pointName2){
        for (var i = 0; i < mouseArea.children.length; ++i) {
            if (mouseArea.children[i].pointName1 === pointName1 && mouseArea.children[i].pointName2 === pointName2) {
                var x = (mouseArea.children[i].x1 + mouseArea.children[i].x2)/2
                var y = (mouseArea.children[i].y1 + mouseArea.children[i].y2)/2
                mouseArea.children[i].isSelected = true
                mouseArea.focusOn(x, y)
                return
            }
        }
        console.log(pointName1, pointName2, "line not found when focused")
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
        background: ControlBackground {implicitWidth: 270}

        MenuDelegate {
            action: controls.pointsSwitch
        }

        MenuDelegate {
            action: controls.invertSwitch
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

        ScrollBar.vertical: CustomScrollBar {}

        ScrollBar.horizontal: CustomScrollBar {}

        Keys.onSpacePressed: {
            if(!event.isAutoRepeat)
            {
                interactive = true
                boundsBehavior = Flickable.DragAndOvershootBounds
                mouseArea.cursorShape = Qt.OpenHandCursor
                mouseArea.enableLock()
            }
        }

        Keys.onReleased: {
            if (event.key === Qt.Key_Space && !event.isAutoRepeat)
            {
                interactive = false
                boundsBehavior = Flickable.StopAtBounds
                mouseArea.cursorShape = Qt.CrossCursor
                if (mouseArea.selectedPoint !== "") {
                    mouseArea.disableLock(mouseArea.selectedPoint);
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
                    rightPanel.resetSelections()
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
                rightPanel.resetSelections()
                source = ""
                source = "image://imageProvider/img"
            }

            function close()
            {
                source = ""
                rightPanel.resetSelections()
                message.send(qsTr("File closed"))
                backEnd.reset()
                pointMessage.backendMsg = qsTr("Please open file");
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
                property string selectedPoint: ""

                signal scaled(var scaleFactor)
                signal deletePoint(var pointName)
                signal enableLock
                signal disableLock(var pointName)
                signal linesDeselected()
                signal pointsNamesStateChanged(var isEnabled)

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

                function focusOn(x, y) {
                    flickArea.contentX = flickArea.contentWidth === flickArea.width ?
                                0 : x*imageTRG.scale - flickArea.width/2
                    flickArea.contentY = flickArea.contentHeight === flickArea.height ?
                                0 : y*imageTRG.scale - flickArea.height/2
                    flickArea.contentX -= flickArea.horizontalOvershoot
                    flickArea.contentY -= flickArea.verticalOvershoot
                }

                function pointMoved(pointName, x, y) {
                    backEnd.writeCoordinates(pointName, x, y)
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
                            childRec.drag.target = undefined //FIXME: crutch
                            childRec.isTilted = isTilted
                            childRec.isEntilted = isEntilted
                            childRec.visible = isVisible
                            childRec.size = mouseArea.pointsSize
                            childRec.x = x - childRec.width/2
                            childRec.y = y - childRec.height/2
                            childRec.scale = 1/imageTRG.scale
                            if (pointName === mouseArea.selectedPoint)
                            {
                                childRec.drag.target = childRec
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
                            childRec.visible = rightPanel.linesZone.isNewVisible
                            mouseArea.children[i1].moving.connect(childRec.movePoint1)
                            mouseArea.children[i2].moving.connect(childRec.movePoint2)
                            childRec.removed.connect(mouseArea.children[i1].disconnectLine)
                            childRec.removed.connect(mouseArea.children[i2].disconnectLine)
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
                            //TODO: Check pointName in pointList?
                            if (mouseArea.selectedPoint === "") {
                                backEnd.writeCoordinates(mouseArea.mouseX, mouseArea.mouseY)
                            }
                            else {
                                //FIXME: Set calibration points first!
                                for (var i = 0; i < mouseArea.children.length; ++i) {
                                    if (mouseArea.children[i].name === mouseArea.selectedPoint) {
                                        return rightPanel.pointsZone.deselectPoint()
                                    }
                                }
                                console.log(mouseArea.selectedPoint)
                                backEnd.writeCoordinates(mouseArea.selectedPoint, mouseArea.mouseX, mouseArea.mouseY)
                            }
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
                            flickArea.flick(wheel.angleDelta.x*20, wheel.angleDelta.y*20)
                        }
                    }
                }

                DropArea {
                    id: pointsDropArea
                    anchors.fill: parent
                    onEntered: {
                        if (!drag.hasText && drag.source) {
                            drag.source.isCaught = true
                        }
                    }

                    onExited: {
                        if (!drag.hasText && drag.source) {
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
                        if (status) {
                            mouseArea.updatePoint(pointName, color, x, y, isTilted, isEntilted, isVisible)
                        }
                        else {
                            mouseArea.deletePoint(pointName)
                        }
                    }

                    onPointsConnected: {
                        mouseArea.connectPoints(pointName1, pointName2, color)
                    }

//                    onClearTables: {
//                        if (mouseArea.selectedPoint !== "") {
//                            workZone.selectPoint(mouseArea.selectedPoint)
//                        }
//                    }
                }
            }
        }
    }
}
