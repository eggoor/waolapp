import QtQuick

Item {
	Shortcut {
		sequence: "Ctrl+W"
		onActivated: wakeUp()
	}

	Shortcut {
		sequence: StandardKey.Save
		onActivated: scan()
	}

	Shortcut {
		sequences: [StandardKey.Refresh, "Ctrl+R"]
		onActivated: refresh()
	}

	Shortcut {
		sequences: [StandardKey.Copy]
		onActivated: copy()
	}

	Shortcut {
		sequence: StandardKey.New
		onActivated: showEditView(false)
	}

	Shortcut {
		sequence: "Ctrl+E"
		onActivated: showEditView(true)
	}

	Shortcut {
		sequences: [StandardKey.Delete]
		onActivated: del()
	}

	Shortcut {
		sequence: "Meta+A"
		onActivated: about()
	}

	Shortcut {
		sequences: [StandardKey.SelectAll]
		onActivated: selectAll()
	}

	Shortcut {
		sequences: [StandardKey.Cancel]
		onActivated: cancel()
	}
}
