import QtQuick

AboutViewForm {
	tabBar {
		onCurrentIndexChanged: {
			switch (tabBar.currentIndex) {
			case 0: aboutViewLoader.source = "About.qml"; break;
			case 1: aboutViewLoader.source = "Credits.qml"; break;
			case 2: aboutViewLoader.source = "License.qml"; break;
			}
		}
	}
	btnClose {
		onClicked: close()
	}

	function close() {
		hideOverlay()
	}
}
