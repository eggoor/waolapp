import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root
    spacing: 8

    property alias labelText: label.text
    property alias text: textField.text
    property alias placeholderText: textField.placeholderText
    property alias validator: textField.validator

    Label {
        id: label
		Layout.preferredWidth: 128
        horizontalAlignment: Qt.AlignRight
        Accessible.name: text
    }

    TextField {
        id: textField
		focus: true
		Layout.preferredWidth: 192
		placeholderText: root.placeholderText
        Accessible.description: label.text
    }
}
