import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl

TabButton {
	width: implicitWidth
	leftPadding: 4
	rightPadding: 4
	font.pixelSize: 16
	background: Rectangle {
		color: Color.blend(parent.checked ? parent.palette.midlight : parent.palette.button,
			parent.palette.dark, parent.down ? 0.5 : 0.0)
		radius: 6
		border.color: borderColor
		border.width: 1
	}
}
