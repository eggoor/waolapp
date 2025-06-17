import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl

Item {
	TextArea {
		id: textArea
		anchors.fill: parent
		anchors.margins: 1
		font.pixelSize: 15
		textFormat: TextEdit.RichText
		text: "Waola is a <a href='https://en.wikipedia.org/wiki/Wake-on-LAN'>Wake-on-LAN</a> utility with embedded <a href='https://en.wikipedia.org/wiki/Address_Resolution_Protocol'>ARP</a>-based network scanner.

			<br><br>Author of this tool ​​is a citizen of Ukraine who created and maintains this software literally under attacks of russian missiles.

			<br><br>Also I try to help our army to fight off the invasion as much as I can.

			<br><br>If this tool was useful to you and you'd like to help us to defend our freedom and independence - feel free to donate any convenient for you amount to PayPal account <a href='https://www.paypal.com/ua/home'>egggor@gmail.com</a>."
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
