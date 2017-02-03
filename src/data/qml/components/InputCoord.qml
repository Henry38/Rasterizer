import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

RowLayout {

    property string title: ""
    property real min: -1.0
    property real max: 1.0
    property alias textField: textField

    Layout.alignment: Qt.AlignHCenter

    Label {
        text: title
    }

    TextField {
        id: textField
        text: ""

        validator: DoubleValidator {
            bottom: min;
            top: max;
            decimals: 3
            locale: "en_EN"
        }

        Layout.preferredWidth: 96
        Layout.preferredHeight: 24
        Layout.alignment: Qt.AlignHCenter
    }
}
