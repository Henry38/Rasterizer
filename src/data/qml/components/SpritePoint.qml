import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

Image {
    id: root
    source: mouseArea.drag.active ? "qrc:/icon/button_over.png" : "qrc:/icon/button.png"

    property var model: null

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: parent
        drag.minimumX: -root.width / 2
        drag.maximumX: canvas.width - (root.width / 2)
        drag.minimumY: -root.height / 2
        drag.maximumY: canvas.height - (root.height / 2)
        drag.threshold: 0
        drag.smoothed: false

        acceptedButtons: Qt.LeftButton

        onPositionChanged: {
            var p = screenToWorld(root.x + (root.width / 2), root.y + (root.height / 2));
            model.set(p.x,p.y);
        }
    }

    Connections {
        target: canvas
        onPaint: {
            var v = worldToScreen(model.x(),model.y());
            root.x = v.x - (root.width / 2);
            root.y = v.y - (root.height / 2);
        }
    }
}
