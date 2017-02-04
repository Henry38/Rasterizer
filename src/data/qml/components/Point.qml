import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

Item {
    id: root

    property string title: ""
    property vector2d point: Qt.vector2d(0,0)

    signal valueChanged();

    Layout.fillWidth : true
    Layout.preferredHeight: 100

    Component.onCompleted: {
        point.x = -1 + Math.random() * 2;
        point.y = -1 + Math.random() * 2;
        fireValueChanged();
    }

    Rectangle {
        anchors.fill : parent
        color: "lightgray"
    }

    ColumnLayout {
        anchors.fill: parent

        Label {
            Layout.topMargin: 5
            Layout.leftMargin: 5
            text: title
        }

        InputCoord {
            id: x_input
            title: "x: "

            textField.text: point.x.toFixed(3)

            textField.onAccepted: {
                point.x = parseFloat(textField.text)
                fireValueChanged()
            }
        }

        InputCoord {
            id: y_input
            title: "y: "

            textField.text: point.y.toFixed(3)

            textField.onAccepted: {
                point.y = parseFloat(textField.text)
                fireValueChanged()
            }
        }
    }

    function x() {
        return point.x;
    }

    function y() {
        return point.y;
    }

    function set(px,py) {
        point.x = px;
        point.y = py;
        fireValueChanged();
    }

    function fireValueChanged() {
        root.valueChanged()
    }
}
