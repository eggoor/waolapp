import QtQuick

Rectangle {
	required property bool selected
	required property int contentHeight

	implicitWidth: 128
	implicitHeight: contentHeight + 8

	color: selected ? palette.highlight : (model.row % 2 ? palette.alternateBase : palette.base)

	MouseArea {
		anchors.fill: parent
		onClicked: (mouse) => {
			if (mouse.button === Qt.LeftButton) {
				let index = tableView.model.index(model.row, 0)

				if (mouse.modifiers & Qt.ShiftModifier) {
					let minRow = model.row
					let maxRow = minRow

					for (let item of rowSelectionModel.selection) {
						minRow = Math.min(minRow, item.top);
						maxRow = Math.max(maxRow, item.bottom);
					}

					for (let i = minRow; i <= maxRow; ++i) {
						rowSelectionModel.select(tableView.model.index(i, 0),
							ItemSelectionModel.Select | ItemSelectionModel.Rows)
					}
				}
				else if (Qt.ControlModifier & mouse.modifiers) {
					rowSelectionModel.select(index, ItemSelectionModel.Toggle | ItemSelectionModel.Rows)
				} else {
					rowSelectionModel.clear()
					rowSelectionModel.select(index, ItemSelectionModel.Select | ItemSelectionModel.Rows)
				}
			}
		}
		onDoubleClicked: wakeUp()
	}
}
