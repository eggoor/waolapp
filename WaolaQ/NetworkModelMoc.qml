import QtQuick
import Qt.labs.qmlmodels

TableModel {
    TableModelColumn { display: "friendlyName" }
    TableModelColumn { display: "hostname" }
    TableModelColumn { display: "ipAddress" }
    TableModelColumn { display: "macAddress" }
    TableModelColumn { display: "lastSeenOnline" }
    TableModelColumn { display: "wakeupResult" }

    rows: [
        {
            friendlyName: "Router",
            hostname: "router.lan",
            ipAddress: "192.68.1.1",
            macAddress: "F4:1E:57:9E:0C:73",
            lastSeenOnline: "Recently",
            wakeupResult: ""
        },
        {
            friendlyName: "Google DNS",
            hostname: "dns.google",
            ipAddress: "8.8.8.8",
            macAddress: "F4:1E:57:9E:0C:73",
            lastSeenOnline: "Never",
            wakeupResult: "Fail"
        },
        {
            friendlyName: "sshp",
            hostname: "sshp",
            ipAddress: "192.168.1.4",
            macAddress: "30:13:8B:8B:54:BE",
            lastSeenOnline: "",
            wakeupResult: ""
        }
    ]

	readonly property bool isEmpty: false
	readonly property bool isIdle: true

    function scan() {
        console.log("Go scan!")
    }
}
