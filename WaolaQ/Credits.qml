import QtQuick
import QtQuick.Controls

Item {
	TextArea {
		id: textArea
		anchors.fill: parent
		anchors.margins: 1
		readOnly: true
		font.pixelSize: 15
		textFormat: TextEdit.RichText
		text: "Idea, architecture and implementation: <a href='mailto:egggor@gmail.com'>egggor@gmail.com</a><br/><br/>Icon set: TUNUHA"
		wrapMode: TextEdit.Wrap

		onLinkActivated: function(link) {
			Qt.openUrlExternally(link)
		}

		onLinkHovered: function(link) {
			mouseArea.cursorShape = link ? Qt.PointingHandCursor : Qt.IBeamCursor
		}
	}

	MouseArea {
		id: mouseArea
		anchors.fill: textArea
		acceptedButtons: Qt.NoButton // Allow TextArea to handle clicks
		cursorShape: Qt.IBeamCursor // Default cursor for TextArea
	}
}
