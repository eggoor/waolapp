import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
	width: 54
	anchors.left: parent.left
	anchors.top: parent.top
	anchors.bottom: tabBar.top

	property int selectionCount: 0

	ColumnLayout {
		id: toolbarLayout
		spacing: 0

		WToolButton {
			id: btnWakeup
			Layout.topMargin: 4
			text: qsTr("Wakeup")
			icon.name: "alarm-symbolic"
			icon.source: "qrc:/icons/01wakeup.svg"
			enabled: rowSelectionModel.selectionCount > 0
			ToolTip.text: enabled ? qsTr("Wake up selected host(s)") : qsTr("Select one or more host(s) to enable")
			onClicked: wakeUp()
		}

		WToolSeparator {
			id: wakeupSeparator
		}

		WToolButton {
			id: btnScan
			text: qsTr("Scan")
			icon.name: networkModel.isIdle ? "edit-find-symbolic" : "process-stop-symbolic"
			icon.source: networkModel.isIdle ? "qrc:/icons/02scan.svg" : "qrc:/icons/03scan_cancel.svg"
			ToolTip.text: networkModel.isIdle ? qsTr("Scan network") : qsTr("Cancel operation")
			onClicked: scan()
		}

		WToolButton {
			id: btnRefresh
			text: qsTr("Refresh")
			icon.name: networkModel.isIdle ? "view-refresh-symbolic" : "process-stop-symbolic"
			icon.source: networkModel.isIdle ? "qrc:/icons/04refresh.svg" : "qrc:/icons/05refresh_cancel.svg"
			ToolTip.text: networkModel.isIdle ? qsTr("Refresh hosts") : qsTr("Cancel operation")
			onClicked: refresh()
		}

		WToolSeparator {
			id: scanRefreshSeparator
		}

		WToolButton {
			id: btnCopy
			text: qsTr("Copy")
			icon.name: "edit-copy-symbolic"
			icon.source: "qrc:/icons/06copy.svg"
			enabled: rowSelectionModel.selectionCount > 0
			ToolTip.text: enabled ? qsTr("Copy selected host(s)") : qsTr("Select one or more host(s) to enable")
			onClicked: copy()
		}

		WToolSeparator {
			id: copySeparator
		}

		WToolButton {
			id: btnAdd
			text: qsTr("Add")
			icon.name: "document-new-symbolic"
			icon.source: "qrc:/icons/07add.svg"
			ToolTip.text: qsTr("Manually add new host")
			onClicked: showEditView(false)
		}

		WToolButton {
			id: btnEdit
			text: qsTr("Edit")
			icon.name: "document-edit-symbolic"
			icon.source: "qrc:/icons/08edit.svg"
			enabled: rowSelectionModel.selectionCount === 1
			ToolTip.text: enabled ? qsTr("Edit selected host") : qsTr("Select a host to enable")
			onClicked: showEditView(true)
		}

		WToolButton {
			id: btnDelete
			text: qsTr("Delete")
			icon.name: "edit-delete-symbolic"
			icon.source: "qrc:/icons/09delete.svg"
			enabled: rowSelectionModel.selectionCount > 0
			ToolTip.text: enabled ? qsTr("Delete selected host(s)") : qsTr("Select one or more host(s) to enable")
			onClicked: del()
		}

		WToolSeparator {
			id: hostActionsSeparator
		}

		WToolButton {
			id: btnAbout
			text: qsTr("About")
			icon.name: "dialog-question-symbolic"
			icon.source: "qrc:/icons/10about.svg"
			ToolTip.text: qsTr("About Waola")
			onClicked: about()
		}
	}
}
