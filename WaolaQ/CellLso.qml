import QtQuick

import "Converters.js" as Converters

CellRect {
	contentHeight: cellText.contentHeight
	property date value: new Date(0)

	function triggeredLastSeenOnline(val, refreshKey) {
		return Converters.lastSeenOnline(val)
	}

	CellText {
		id: cellText
		text: triggeredLastSeenOnline(value, tableView.refreshKey);
	}
}
