import QtQuick
import QtQuick.Controls
import WaolaQt

ScrollView {
	anchors.fill: parent
	anchors.margins: 1
	ETextReader {
		id: textReader
	}

	TextArea {
		font.pixelSize: 15
		readOnly: true
		textFormat: TextEdit.PlainText
		text: textReader.readFile(":/LICENSE.txt")
		wrapMode: TextEdit.Wrap
		onLinkActivated: function(link) {
			Qt.openUrlExternally(link)
		}
	}
}
