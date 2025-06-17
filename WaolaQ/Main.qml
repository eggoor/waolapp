 import QtQuick
 import QtQuick.Controls
 import QtQuick.Dialogs

import WaolaQt

Window {
	id: window
	width: 800
	height: 600

	visible: true
	title: "Waola"

	ENetworkModel {
		id: networkModel
	}

	ItemSelectionModel {
		id: rowSelectionModel
		model: networkModel
		property int selectionCount: 0

		onSelectionChanged: {
			selectionCount = selectedRows().length
		}
	}

	WToolBar {
		id: toolBar
	}

	MainView {
		id: mainView
		anchors.fill: parent
		anchors.leftMargin: toolBar.width
		anchors.bottomMargin: tabBar.height

		Component.onCompleted: {
			networkModel.isIdleChanged.connect(onIsIdleChanged)
			if (networkModel.isEmpty) {
				scan()
			}
			else {
				refresh()
			}
		}
	}

	OverlayLoader {
		id: overlayLoader
		anchors.top: parent.top
		anchors.horizontalCenter: parent.horizontalCenter
	}

	Component {
		id: compEditView
		EditView {}
	}

	Component {
		id: compAboutView
		AboutView {}
	}

	WTabBar {
		id: tabBar
		y: toolBar.bottom
	}

	BusyIndicator {
		running: !networkModel.isIdle
		anchors.centerIn: parent
	}

	WShortcuts {}

	Component.onCompleted: {
		restoreFocus()
	}

	onClosing: {
		waitModel()
	}

	function restoreFocus() {
		mainView.tableView.forceActiveFocus()
	}

	function waitModel() {
		if (!networkModel.isIdle) {
			networkModel.cancel()
			networkModel.wait()
		}
	}

	function cancel() {
		if (overlayLoader.item) {
			hideOverlay()
		}
		else if (!networkModel.isIdle) {
			networkModel.cancel()
		} else {
			rowSelectionModel.clearSelection()
		}
	}

	function showEditView(edit) {
		if (!edit || rowSelectionModel.selectionCount === 1) {
			if (overlayLoader.sourceComponent !== compEditView) {
				overlayLoader.sourceComponent = compEditView

				if (edit) {
					let editView = overlayLoader.item
					let xHost = rowSelectionModel.selectedIndexes[0].row

					editView.edit = true
					editView.displayName = networkModel.data(networkModel.index(xHost, 0), Qt.UserRole + 2)
					editView.hostname = networkModel.data(networkModel.index(xHost, 1))
					editView.ipAddress = networkModel.data(networkModel.index(xHost, 2))
					editView.macAddress = networkModel.data(networkModel.index(xHost, 3))
				}
				overlayLoader.slideDown.start()
			}
		}
	}

	function showAboutView() {
		if (overlayLoader.sourceComponent !== compAboutView) {
			overlayLoader.sourceComponent = compAboutView
			overlayLoader.slideDown.start()
		}
	}

	function hideOverlay() {
		if (overlayLoader.item) {
			overlayLoader.slideUp.start()
			restoreFocus()
		}
	}

	function saveChanges(edit) {
		let saved = false
		let editView = overlayLoader.item

		if (edit) {
			let xHost = rowSelectionModel.selectedIndexes[0].row
			saved |= networkModel.setData(networkModel.index(xHost, 0), editView.displayName)
			saved |= networkModel.setData(networkModel.index(xHost, 1), editView.hostname)
			saved |= networkModel.setData(networkModel.index(xHost, 2), editView.ipAddress)
			saved |= networkModel.setData(networkModel.index(xHost, 3), editView.macAddress)
		}
		else {
			saved = networkModel.addHost(editView.displayName, editView.hostname,
				editView.ipAddress, editView.macAddress)
		}

		if (saved) {
			networkModel.persist()
		}
	}

	function wakeUp() {
		if (overlayLoader.item) {
			hideOverlay()
		}

		if (networkModel.isIdle) {
			if (rowSelectionModel.selectionCount > 0) {
				let sr = rowSelectionModel.selectedRows(0)
				safeCall(function() { networkModel.wakeUp(sr); })
			}
		} else {
			networkModel.cancel()
		}
	}

	function scan() {
		if (overlayLoader.item) {
			hideOverlay()
		}

		if (networkModel.isIdle) {
			safeCall(networkModel.scanNetwork)
		} else {
			safeCall(networkModel.cancel)
		}
	}

	function refresh() {
		if (overlayLoader.item) {
			hideOverlay()
		}

		if (networkModel.isIdle) {
			safeCall(networkModel.refresh)
		} else {
			safeCall(networkModel.cancel)
		}
	}

	EClipboard {
		id: clipboard
	}

	function copy() {
		if (rowSelectionModel.selectionCount > 0) {
			let sr = rowSelectionModel.selectedRows()
			let selectedText = ""
			sr.forEach(function(rowIndex) {
				let rowData = []
				rowData.push(networkModel.data(networkModel.index(rowIndex.row, 3), 0) || "")
				rowData.push(networkModel.data(networkModel.index(rowIndex.row, 2), 0) || "")
				rowData.push(networkModel.data(networkModel.index(rowIndex.row, 1), 0) || "")
				selectedText += rowData.join("\t") + "\n"
			})

			if (selectedText) {
				clipboard.text = selectedText;
			}
		}
	}

	function del() {
		try {
			if (rowSelectionModel.selectionCount > 0) {
				let sr = rowSelectionModel.selectedRows(0)
				if (networkModel.removeHosts(sr)) {
					networkModel.persist()
				}
			}
		} catch (e) {
			console.log(e)
			showMessage(e.message);
		}
	}

	function about() {
		showAboutView()
	}

	function selectAll() {
		for (let i = networkModel.rowCount(); i --> 0;) {
			rowSelectionModel.select(networkModel.index(i, 0),
				ItemSelectionModel.Select | ItemSelectionModel.Rows)
		}
	}

	function onIsIdleChanged(isIdle: bool) {
		try {
			if (isIdle) {
				networkModel.finalizeScan()
				networkModel.persist()
			}
		} catch (e) {
			console.log(e)
			showMessage(e.message)
		}
	}

	function safeCall(f) {
		try {
			if (typeof f !== "function") {
				showMessage("'" + f + "' is not a function");
			} else {
				f()
			}
		} catch (e) {
			console.log(e)
			showMessage(e.message);
		}
	}

	function showMessage(msg) {
		messageBox.text = msg
		messageBox.open()
	}

	MessageDialog {
		id: messageBox
		title: "Exception"
		buttons: MessageDialog.Ok
		onAccepted: this.close()
	}
}
