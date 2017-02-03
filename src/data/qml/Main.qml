import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2

import components 1.0
import ImageModel 1.0

Window {
    id: root
    title: "Rasterizer"
    visible: true

    width: 640
    height: 640
    color: "gray"

    property var imageModel: ImageModel {
        id: imageModel
    }

    signal update();

    onUpdate: {
        imageModel.rasterize(point1.x(), point1.y(), point2.x(), point2.y(), point3.x(), point3.y());
        canvas.requestPaint();
    }

    ColumnLayout {
        anchors.fill: parent

        Canvas {
            id: canvas

            Layout.fillWidth: true
            Layout.fillHeight: true

            onPaint: {
                var ctx = canvas.getContext('2d');
                ctx.fillStyle = Qt.rgba(1,1,1,1);
                ctx.fillRect(0,0,width,height);

            // grid

                ctx.beginPath();
                ctx.lineWidth = 0.3;
                ctx.strokeStyle = Qt.rgba(0, 0, 0, 1);

                var r = imageModel.resolution;
                var ni = Math.ceil( 1 / r );
                var nj = Math.ceil( 1 / r );
                for (var i = -ni; i <= ni; ++i) {
                    var p = worldToScreen(i * r, -1);
                    var q = worldToScreen(i * r,  1);
                    ctx.moveTo(p.x, p.y);
                    ctx.lineTo(q.x, q.y);
                }
                for (var j = -nj; j <= nj; ++j) {
                    var p = worldToScreen(-1, j * r);
                    var q = worldToScreen( 1, j * r);
                    ctx.moveTo(p.x, p.y);
                    ctx.lineTo(q.x, q.y);
                }

                ctx.stroke();

            // pixel

                ctx.fillStyle = Qt.rgba(0, 1, 0, 1);

                var pixels = imageModel.pixelData;
                var w = ((canvas.width / 2.0) * r) - 2;
                var h = ((canvas.height / 2.0) * r) - 2;
                for (var i = 0; i < pixels.length; ++i) {
                    var x = pixels[i].x;
                    var y = pixels[i].y;
                    var p = gridToScreen(x, y);
                    ctx.fillRect(p.x-w/2, p.y-h/2, w, h);
                }

            // axe

                ctx.beginPath();
                ctx.lineWidth = 1;
                ctx.strokeStyle = Qt.rgba(0, 0, 0, 1);

                var o = worldToScreen(0,0);
                var oi = worldToScreen(1,0);
                var oj = worldToScreen(0,1);
                ctx.moveTo(o.x, o.y);
                ctx.lineTo(oi.x, oi.y);
                ctx.lineTo(oi.x-16, oi.y-8);
                ctx.moveTo(oi.x, oi.y);
                ctx.lineTo(oi.x-16, oi.y+8);

                ctx.moveTo(o.x, o.y);
                ctx.lineTo(oj.x, oj.y);
                ctx.lineTo(oj.x-8, oj.y+16);
                ctx.moveTo(oj.x, oj.y);
                ctx.lineTo(oj.x+8, oj.y+16);

                ctx.stroke();

            // triangle

                ctx.beginPath();
                ctx.lineWidth = 2;
                ctx.strokeStyle = Qt.rgba(1, 0, 0, 1);

                var p1 = worldToScreen(point1.x(), point1.y());
                var p2 = worldToScreen(point2.x(), point2.y());
                var p3 = worldToScreen(point3.x(), point3.y());

                ctx.moveTo(p1.x,p1.y);
                ctx.lineTo(p2.x,p2.y);
                ctx.lineTo(p3.x,p3.y);
                ctx.lineTo(p1.x,p1.y);

                ctx.stroke();
            }
        }

        RowLayout {
            id: rowLayout

            // Point 1
            Point {
                id : point1
                title: "Point 1"

                onValueChanged: {
                    root.update();
                }
            }

            // Point 2
            Point {
                id : point2
                title: "Point 2"

                onValueChanged: {
                    root.update();
                }
            }

            // Point 3
            Point {
                id : point3
                title: "Point 3"

                onValueChanged: {
                    root.update();
                }
            }
        }
    }

    function worldToScreen(x, y) {
        var camera_width = 2;
        var camera_height = 2;
        var p = Qt.vector2d((2*x)/camera_width, (2*y)/camera_height);

        var screen_x = (canvas.width / 2.0) + (p.x * (canvas.width / 2.0));
        var screen_y = (canvas.height / 2.0) - (p.y * (canvas.height / 2.0));
        return Qt.vector2d(screen_x,screen_y);
    }

    function gridToScreen(x, y) {
        var r = imageModel.resolution;
        return worldToScreen((x+0.5)*r, (y+0.5)*r);
    }
}
