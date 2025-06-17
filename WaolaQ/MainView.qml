import QtQuick

MainViewForm {
	tableView {
		id: tableView
		focus: true
		property alias model: tableView.model

		Component.onCompleted: {
			rowSelectionModel.select(model.index(0, 0),
				ItemSelectionModel.Select | ItemSelectionModel.Rows)
		}

		Keys.onPressed: (event) => {
			if (event.key === Qt.Key_Up) {
				selectPrev(event)
				event.accepted = true
			} else if (event.key === Qt.Key_Down) {
				selectNext(event)
				event.accepted = true
			} else if (event.key === Qt.Key_Home) {
				home(event)
				event.accepted = true
			} else if (event.key === Qt.Key_End) {
				end(event)
				event.accepted = true
			}
		}
	}

	function selectPrev(event) {
		const selection = rowSelectionModel.selectionCount > 0
		let selRow = 0;

		if (selection) {
			const lastRow = model.rowCount() - 1

			let minRow = lastRow

			for (let item of rowSelectionModel.selection) {
				minRow = Math.min(minRow, item.top);
			}

			selRow = minRow !== 0 ? minRow - 1 : lastRow

			if ((event.modifiers & Qt.ShiftModifier) === 0) {
				rowSelectionModel.clearSelection()
			}
		}

		rowSelectionModel.select(model.index(selRow, 0),
			ItemSelectionModel.Select | ItemSelectionModel.Rows)
	}

	function selectNext(event) {
		const lastRow = model.rowCount() - 1
		const selection = rowSelectionModel.selectionCount > 0
		let selRow = lastRow

		if (selection) {
			let maxRow = 0

			for (let item of rowSelectionModel.selection) {
				maxRow = Math.max(maxRow, item.bottom);
			}

			selRow = maxRow !== lastRow ? maxRow + 1 : 0

			if ((event.modifiers & Qt.ShiftModifier) === 0) {
				rowSelectionModel.clearSelection()
			}
		}

		rowSelectionModel.select(model.index(selRow, 0),
			ItemSelectionModel.Select | ItemSelectionModel.Rows)
	}

	function home(event) {
		const lastRow = model.rowCount() - 1
		const selection = rowSelectionModel.selectionCount > 0
		const selRow = 0;

		if (selection) {
			if ((event.modifiers & Qt.ShiftModifier) === 0) {
				rowSelectionModel.clearSelection()
			} else {
				let maxRow = 0

				for (let item of rowSelectionModel.selection) {
					maxRow = Math.max(maxRow, item.bottom);
				}

				for(let i = maxRow; i > selRow; --i) {
					rowSelectionModel.select(model.index(i, 0),
						ItemSelectionModel.Select | ItemSelectionModel.Rows)
				}
			}
		}

		rowSelectionModel.select(model.index(selRow, 0),
			ItemSelectionModel.Select | ItemSelectionModel.Rows)
	}

	function end(event) {
		const lastRow = model.rowCount() - 1
		const selection = rowSelectionModel.selectionCount > 0

		if (selection) {
			if ((event.modifiers & Qt.ShiftModifier) === 0) {
				rowSelectionModel.clearSelection()
			} else {

				let minRow = lastRow

				for (let item of rowSelectionModel.selection) {
					minRow = Math.min(minRow, item.top);
				}

				for(let i = minRow; i < lastRow; ++i) {
					rowSelectionModel.select(model.index(i, 0),
						ItemSelectionModel.Select | ItemSelectionModel.Rows)
				}
			}
		}

		rowSelectionModel.select(model.index(lastRow, 0),
			ItemSelectionModel.Select | ItemSelectionModel.Rows)
	}
}
