

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls

Rectangle {
	id: root
	focus: true
	color: palette.base

	property alias tableView: tableView

	HorizontalHeaderView {
		id: headerView
		anchors.left: parent.left
		anchors.top: parent.top
		anchors.right: parent.right
		syncView: tableView
		clip: true
	}

	TableView {
		id: tableView
		anchors.fill: parent
		focus: true
		anchors.topMargin: headerView.height
		columnSpacing: 0
		rowSpacing: 1
		clip: true
		model: networkModel
		selectionModel: rowSelectionModel

		property int refreshKey: 0

		Timer {
			interval: 40000
			running: true
			repeat: true
			onTriggered: tableView.refreshKey++
		}

		delegate: WDelegateChooser {}
	}
}
