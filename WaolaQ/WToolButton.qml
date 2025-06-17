import QtQuick.Controls

ToolButton {
    height: 40
    display: AbstractButton.IconOnly
    icon.height: 32
    icon.width: 32
	icon.color: enabled ? (hovered ? palette.highlight : palette.windowText) : palette.mid
    ToolTip.visible: hovered
}
