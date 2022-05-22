import QtQuick 2.0

import QtQuick.Layouts 1.15

Rectangle {
    id:root
    property string titleText: "XGroupBox"
    property color titleTextColor: "black"
   anchors.topMargin:_title.height
   Layout.topMargin:_title.height/2
   implicitHeight:50
   implicitWidth:50
   border.color: "transparent"
   border.width: 1
   radius: 5
   Rectangle {
       x:20
       color:root.color
       height: _title.height
       width:_title.width
       anchors.top: parent.top
       anchors.topMargin: -_title.height/2
       Text {
           id: _title
           color:titleTextColor
           text: titleText
           font.pixelSize: 14
       }
   }
}
