/**
(C) 2017-2018, Bad Programmer (badpr0ogrammer@gmail.com)
*/
#define MAP_REG_QML "\
import QtQuick 2.7; \
import QtQuick.Controls 2.2; \
import QtLocation 5.9; \
import QtPositioning 5.9; \
ApplicationWindow{ \
  id:appWin; \
  width:600; height:600; \
  visible:true; \
  signal appWinAccepted(var rect); \
  signal appWinRejected(); \
  Frame{ \
    visible:true; \
    anchors.fill:parent; \
    Map{ \
     id:osmMap; objectName:'osmMap'; \
     plugin:Plugin{ name:'mapbox'; \
       PluginParameter{ name:'mapbox.access_token'; \
         value: 'pk.eyJ1IjoiYW5kcmV5MTIzIiwiYSI6ImNqMHJsMHB3bzAwMWcyd3FsbHVkOHN5eWMifQ.vzJq7KBDoINFbrA-DYH8Bw' } \
     } \
     anchors.fill:parent; \
     visible:true; \
     center{ latitude:0; longitude:0 } \
     zoomLevel:3; \
     MouseArea{ \
       anchors.fill:parent; hoverEnabled:false; \
       onPressed:{ \
         if(mouse.modifiers & Qt.ShiftModifier){ \
           var coord = osmMap.toCoordinate(Qt.point(mouseX,mouseY)); \
           redRect.bottomRight.latitude = coord.latitude; \
           redRect.bottomRight.longitude = coord.longitude; \
           hoverEnabled = true; \
           redRect.coords(); \
         } \
       } \
       onPositionChanged:{ \
         if(mouse.modifiers & Qt.ShiftModifier){ \
           var coord = osmMap.toCoordinate(Qt.point(mouseX,mouseY)); \
           redRect.bottomRight.latitude = coord.latitude; \
           redRect.bottomRight.longitude = coord.longitude; \
           redRect.coords(); \
         } \
       } \
       onReleased: hoverEnabled = false; \
     } \
     MapRectangle{ \
       id:redRect; \
       objectName:'redRect'; \
       border.color:'red'; border.width:4; opacity:1; \
       topLeft{ latitude:0.5; longitude:-0.5 } bottomRight{ latitude:-0.5; longitude:0.5 } \
       MouseArea{ \
         anchors.fill:parent; cursorShape:drag.active ? Qt.ClosedHandCursor:Qt.OpenHandCursor; \
         drag{ target:parent; } \
       } \
       function coords(){ \
         var x = (redRect.topLeft.longitude + redRect.bottomRight.longitude)/2; x = Number(x.toFixed(4)); \
         var y = (redRect.topLeft.latitude + redRect.bottomRight.latitude)/2;	y = Number(y.toFixed(4)); \
         var w = redRect.bottomRight.longitude - redRect.topLeft.longitude; w = Number(w.toFixed(4)); \
         var h = redRect.topLeft.latitude - redRect.bottomRight.latitude; h = Number(h.toFixed(4)); \
         latLonLabel.text = 'latitude: '+y+'    longitude: '+x+'    width: '+w+'    height: '+h; \
       } \
       onXChanged:coords(); \
       onYChanged:coords(); \
     } \
   } \
 } \
 footer:ToolBar{ \
   Column{ \
     anchors.horizontalCenter: parent.horizontalCenter; \
     spacing:6; \
     Label{ id:latLonLabel; text:'Drag the rect, pls'; color:'darkRed'; font.pointSize:12; anchors.horizontalCenter:parent.horizontalCenter; } \
     Label{ text:'Drag to move, Shift+Drag to resize.'; color:'darkGreen'; font.pointSize:12; anchors.horizontalCenter:parent.horizontalCenter; } \
     Row{ \
       spacing:6; \
       anchors.horizontalCenter: parent.horizontalCenter; \
       ToolButton{ \
         text:'OK'; \
         font.pixelSize:12; \
         width:100; \
         onClicked: appWin.appWinAccepted(redRect); \
       } \
       ToolButton{ \
         text:'Cancel'; \
         font.pixelSize:12; \
         width:100; \
         onClicked: appWin.appWinRejected(); \
       } \
     } \
   } \
  }\
}"
