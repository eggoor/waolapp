

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import WaolaQt
import Qt.labs.qmlmodels

Item {
	width: 384
	height: 180

	readonly property var borderColor: palette.mid

	property bool edit: false

	property alias displayName: displayName.text
	property alias hostname: hostname.text
	property alias ipAddress: ipAddress.text
	property alias macAddress: macAddress.text

	property alias btnSave: btnSave
	property alias btnCancel: btnCancel

	Rectangle {
		id: rectRoot
		color: palette.base

		anchors.fill: parent

		// Right border
		Rectangle {
			width: 1
			border.color: borderColor
			anchors.top: parent.top
			anchors.bottom: parent.bottom
			anchors.right: parent.right
		}

		// Bottom border
		Rectangle {
			height: 1
			border.color: borderColor
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.bottom: parent.bottom
		}

		// Left border
		Rectangle {
			width: 1
			border.color: borderColor
			anchors.top: parent.top
			anchors.bottom: parent.bottom
			anchors.left: parent.left
		}

		ColumnLayout {
			anchors.top: parent.top
			anchors.topMargin: 8
			anchors.horizontalCenter: parent.horizontalCenter
			spacing: 4

			LabeledTextField {
				id: displayName
				labelText: "Display name:"
				placeholderText: "Enter hots's display name"
			}

			LabeledTextField {
				id: hostname
				labelText: "Hostname:"
				placeholderText: "Enter hostname "
			}

			LabeledTextField {
				id: ipAddress
				labelText: "IP address:"
				placeholderText: "Enter hots's IP address "
			}

			LabeledTextField {
				id: macAddress
				labelText: "MAC address:"
				placeholderText: "Enter hots's MAC address "
			}
		}

		RowLayout {
			spacing: 32
			anchors.bottom: parent.bottom
			anchors.bottomMargin: 8
			anchors.horizontalCenter: parent.horizontalCenter
			Button {
				id: btnCancel
				text: "Cancel"
				Layout.alignment: Qt.AlignHCenter
			}

			Button {
				id: btnSave
				text: "Save"
				Layout.alignment: Qt.AlignHCenter
				KeyNavigation.tab: displayName
			}
		}
	}
}
