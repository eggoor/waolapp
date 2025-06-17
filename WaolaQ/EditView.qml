import QtQuick

EditViewForm {
	btnCancel {
		onClicked: close(false)
	}
	btnSave {
		onClicked: close(true)
	}

	function close(savingChanges) {
		if (savingChanges) {
			saveChanges(edit);
		}
		hideOverlay()
	}
}
