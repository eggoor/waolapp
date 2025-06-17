import QtQuick

CellRect {
	contentHeight: cellText.contentHeight
	CellText {
		id: cellText
		text: display
	}
}
