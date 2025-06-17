
/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
	width: 640
	height: 480
	readonly property var borderColor: palette.dark

	property alias btnClose: btnClose
	property alias tabBar: tabBar
	property alias aboutViewLoader: aboutViewLoader

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

		TabBar {
			id: tabBar
			anchors.top: parent.top
			anchors.topMargin: 4
			anchors.horizontalCenter: parent.horizontalCenter
			spacing: 2

			background: Rectangle {
				color: "transparent"
			}

			WTabButton {
				id: btnAbout
				text: qsTr("About")
			}
			WTabButton {
				id: btnCredits
				text: qsTr("Credits")
			}
			WTabButton {
				id: btnLicense
				text: qsTr("License")
			}
		}

		Loader {
			id: aboutViewLoader
			width: parent.width
			anchors.top: tabBar.bottom
			anchors.topMargin: 4
			anchors.bottom: btnClose.top
			anchors.bottomMargin: 8
			source: "About.qml"
		}

		Button {
			id: btnClose
			focus: true
			text: qsTr("Close")
			anchors.bottom: parent.bottom
			anchors.bottomMargin: 8
			anchors.horizontalCenter: parent.horizontalCenter
		}
	}
}
