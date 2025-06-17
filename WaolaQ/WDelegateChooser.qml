import QtQuick
import Qt.labs.qmlmodels

DelegateChooser {
	DelegateChoice {
		column: 0
		delegate: CellGeneric {}
	}

	DelegateChoice {
		column: 1
		delegate: CellGeneric {}
	}

	DelegateChoice {
		column: 2
		delegate: CellGeneric {}
	}

	DelegateChoice {
		column: 3
		delegate: CellGeneric {
			implicitWidth: 144
		}
	}

	DelegateChoice {
		column: 4
		delegate: CellLso {
			value: lastSeenOnline
		}
	}

	DelegateChoice {
		column: 5
		delegate: CellGeneric {
			implicitWidth: 96
		}
	}
}
