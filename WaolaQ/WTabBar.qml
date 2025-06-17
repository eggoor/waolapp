import QtQuick
import QtQuick.Controls

TabBar {
    height: 32
    position: TabBar.Footer
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
	background: Rectangle {
		color: palette.window
	}
}
