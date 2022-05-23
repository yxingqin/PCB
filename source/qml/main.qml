import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
ApplicationWindow {
    id:_window
    width:1000
    height: 840
    minimumHeight: 840
    minimumWidth:600
    visible:true
    title:"时间片轮转算法演示"
    background: Rectangle{
        color: _theme.primaryColor
    }
    QtObject {
        id:_theme
        property color primaryColor: "#2e2f30"
        property color secondaryColor: "LightSlateGray"
        property color thirdaryColor:"LightGray"
        property color textColor: "white"
        property color hoverColor: "gray"
    }
    function _printMessageInfo(msg,color="blue")
    {
        var str = `${(new Date()).toTimeString().split(' ')[0]}: ${msg}`;
        _console.append(`<b style='color: ${color};'>${str}</b>`);
    }
    function _printMessageLog(msg)
    {
        _console.append(`<b style='color: white;'>${msg}</b>`);
    }
    function _loadInsFile()
    {
        _tfFile.text=_tfFile.text.replace("file:///","");
        if( _tfFile.text.length>0&&$Simulator.loadInsFile( _tfFile.text))
        {
            _drapArea.visible=false;
        }else
        {
            _msgDloadError.open();
        }
    }
    menuBar: MenuBar{
        Menu {

            title:"文件"
            Action {
                text:"打开"
                onTriggered: {
                    _fileDialog.open();
                }
            }
            MenuSeparator { }
            Action {
                text:"退出"
                onTriggered:{
                    _window.close()
                }
            }
        }
        Menu {
            id:_menuLog
            title:"日志"
            Action  {
                text: "清空日志"
                onTriggered: {  _console.text = "" ;}
            }
            Action  {
                text: "保存日志"
            }
        }
        Menu{
            title:"帮助"
            Action {
                text: "关于"
            }
        }
    }
    Component {
        id:_comLVHeader
        Rectangle {
            width: parent.width
            height: 22
            Row {
                anchors.fill: parent
                Text {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width/3
                    height: parent.height
                    text: "进程"
                }
                Text {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width/3
                    height: parent.height
                    text: "指令"
                }
                Text {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width/3
                    height: parent.height
                    text: "剩余时间"
                }
            }
        }
    }
    Component {
        id:_comLVDelegate
        Rectangle {
            width: parent.width
            color:index%2===0?_theme.secondaryColor:_theme.thirdaryColor;
            height: 22
            border.color:"#FFF"
            Row {
                anchors.fill: parent
                Text {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width/3
                    height: parent.height
                    text: "p"+model.pcbId
                }
                Text {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width/3
                    height: parent.height
                    text: model.insType
                }
                Text {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width/3
                    height: parent.height
                    text: model.timeLen+" ms"
                }
            }
        }
    }


    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4
        XGroupBox {
            titleTextColor:_theme.textColor
            color:_theme.primaryColor
            border.color: _theme.thirdaryColor
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            titleText:"工具栏"
            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 5
                RowLayout {
                    Layout.fillHeight:  true
                    Layout.fillWidth: true
                    TextField {
                        id:_tfFile
                        Layout.fillWidth: true
                        placeholderText: "双击选择载入指令文件"
                        MouseArea {
                            anchors.fill: parent
                            onPressed:{
                                _tfFile.focus=true
                            }
                            onDoubleClicked: {
                                _fileDialog.open()
                            }
                        }
                    }
                    Button {
                        text:"载入"
                        onClicked:{
                            if(_tfFile.text.length>0)
                            {
//                                _lv_ready.visible=false
//                                _lv_ready.visible=true
//                                _lv_ready.forceLayout();
//                                console.log("更新数据")
                                _loadInsFile();
                            }
                        }
                    }
                }
                RowLayout {
                    spacing: 5
                    Layout.preferredWidth: 240
                    Text {
                        text: "时间片长度:"
                        font.pixelSize: 16
                        color:_theme.textColor
                    }
                    SpinBox {
                        id:_tfTimeSlice
                        value: 100
                        from:0
                        to:5000
                    }
                    Text {
                        text: "ms"
                        font.pixelSize: 16
                        color:_theme.textColor
                    }
                }
                Button {
                    text:"开始调度/暂停调度"
                }
            }
        }
        XGroupBox {
            color:_theme.primaryColor
            titleTextColor:_theme.textColor
            border.color: _theme.thirdaryColor
            Layout.fillWidth: true
            Layout.fillHeight:true
            titleText:"运行状态"
            Layout.minimumHeight:400
            Rectangle {
                id:_drapArea
                z:1
                anchors.fill: parent
                anchors.margins:5
                anchors.topMargin:10
                color:_theme.primaryColor
                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 16
                    color:_theme.textColor
                    text:"指令文件拖拽到此处打开"
                }
                DropArea {
                    anchors.fill: parent
                    onDropped: {
                        if (drop.hasUrls) {
                            _tfFile.text=drop.urls[0];
                            _loadInsFile();

                        }
                    }
                }
            }
            ColumnLayout {
                anchors.fill:parent
                anchors.margins: 2
                RowLayout {
                    Layout.preferredHeight: 60
                    Layout.alignment: Qt.AlignHCenter
                    Text {
                        font.pixelSize: 16
                        color:_theme.textColor
                        text:"当前运行进程"
                    }
                    Rectangle {
                        Layout.preferredHeight:40
                        Layout.preferredWidth: 100
                        color:_theme.thirdaryColor
                        Text {
                            id:_curPrc
                            anchors.centerIn: parent
                            text:"P0"
                        }

                    }
                }
                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.margins: 5
                    spacing:10
                    XGroupBox {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        color:_theme.primaryColor
                        border.color: _theme.thirdaryColor
                        titleTextColor:_theme.textColor
                        titleText: "就绪队列"
                        ListView {
                            id:_lv_ready
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing:2
                            model: $ModelReadyQue
                            clip:true
                            header:_comLVHeader
                            delegate: _comLVDelegate
                        }
                    }
                    XGroupBox {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        color:_theme.primaryColor
                        border.color: _theme.thirdaryColor
                        titleTextColor:_theme.textColor
                        titleText: "输入等待队列"
                        ListView {
                            id:_lv_inputWait
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing:2
                            model: 15
                            clip:true
                            header:_comLVHeader
                            //                            delegate: _comLVDelegate
                        }
                    }
                    XGroupBox {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        color:_theme.primaryColor
                        border.color: _theme.thirdaryColor
                        titleTextColor:_theme.textColor
                        titleText: "输出等待队列"
                        ListView {
                            id:_lv_outputWait
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing:2
                            model: 15
                            clip:true
                            header:_comLVHeader
                            //                            delegate: _comLVDelegate
                        }
                    }
                    XGroupBox {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        color:_theme.primaryColor
                        border.color: _theme.thirdaryColor
                        titleTextColor:_theme.textColor
                        titleText: "其他等待队列"
                        ListView {
                            id:_lv_otherWait
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing:2
                            model: 15
                            clip:true
                            header:_comLVHeader
                            //                            delegate: _comLVDelegate
                        }
                    }
                    XGroupBox {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        color:_theme.primaryColor
                        border.color: _theme.thirdaryColor
                        titleTextColor:_theme.textColor
                        titleText: "运行完毕进程"
                        ListView {
                            id:_lv_over
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing:2
                            model: 15
                            clip:true
                            header: Rectangle {
                                width: parent.width
                                height: 22
                                Row {
                                    anchors.fill: parent
                                    Text {
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        width: parent.width/2
                                        height: parent.height
                                        text: "进程"
                                    }
                                    Text {
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        width: parent.width/2
                                        height: parent.height
                                        text: "周转时间"
                                    }
                                }
                            }
                            delegate:Rectangle {
                                width: parent.width
                                color:index%2===0?_theme.secondaryColor:_theme.thirdaryColor;
                                height: 22
                                border.color:"#FFF"
                                Row {
                                    anchors.fill: parent
                                    Text {
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        width: parent.width/2
                                        height: parent.height
                                        text: "p"+index
                                    }
                                    Text {
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        width: parent.width/2
                                        height: parent.height
                                        text: index+" ms"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        XGroupBox {
            color:_theme.primaryColor
            border.color: _theme.thirdaryColor
            titleTextColor:_theme.textColor
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            titleText:"运行日志"
            ScrollView {
                id:_sv_log
                anchors.fill: parent
                anchors.margins: 5
                TextArea {
                    id:_console
                    textFormat: "RichText"
                    wrapMode: "Wrap"
                    selectByMouse: true
                    color:_theme.textColor

                    readOnly: true
                    onTextChanged:{
                        //滚动条向下移动
                        _sv_log.ScrollBar.vertical.position = 1 - _sv_log.ScrollBar.vertical.size;
                    }
                    Component.onCompleted: {
                        _printMessageInfo("测试")
                        _printMessageLog("测试")
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton
                onClicked: _menu.popup()
                Menu {
                    id:_menu
                    title:"日志"
                    Action  {
                        text: "清空日志"
                        onTriggered: {  _console.text = "" ;}
                    }
                    Action  {
                        text: "保存日志"
                    }
                }
            }
        }
    }
    MessageDialog {
        id: _msgDloadError
        title: "错误"
        visible:false
        icon:StandardIcon.Critical
        standardButtons:StandardButton.Ok
        text: "载入文件失败，请检查文件格式或者路径是否正确"
        onAccepted: {
            _tfFile.text="";
        }
    }
    FileDialog {
        id: _fileDialog
        title: "选择文件"
        folder: shortcuts.desktop
        nameFilters:[ "指令文件 (*.txt)", "All files (*)" ]
        onAccepted: {
            _tfFile.text=_fileDialog.fileUrl;
            _loadInsFile();
        }
    }


}
