import QtQuick

Loader {
	id: loader
	clip: true
	z: 1

	readonly property int animationDuration: 768
	property alias slideDown: slideDown
	property alias slideUp: slideUp

	NumberAnimation {
		id: slideDown
		target: loader.item
		property: "y"
		from: -height
		to: 0
		duration: loader.animationDuration
		easing.type: Easing.OutQuart
	}

	NumberAnimation {
		id: slideUp
		target: loader.item
		property: "y"
		from: 0
		to: -height
		duration: loader.animationDuration
		easing.type: Easing.InQuart
		onFinished: {
			if (loader.item) {
				loader.sourceComponent = null
				focus = false
			}
		}
	}

	onLoaded: focus = true

	Keys.onPressed: (event)=> {
		if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
			item.close(true)
			event.accepted = true
		} else if (event.key === Qt.Key_Escape) {
			item.close(false)
			event.accepted = true
		}
	}
}
