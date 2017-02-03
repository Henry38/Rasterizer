import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

Item {
    id: root

    property string title: ""
    property vector2d point: Qt.vector2d(random(),random())

    signal valueChanged();

    Layout.fillWidth : true
    Layout.preferredHeight: 100

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

    function random() {
        return -1 + Math.random() * 2;
    }

    function x() {
        return point.x;
    }

    function y() {
        return point.y;
    }

    function fireValueChanged() {
        root.valueChanged()
    }
}
