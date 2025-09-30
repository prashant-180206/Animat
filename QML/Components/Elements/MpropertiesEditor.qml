import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Animat 1.0
import "Input"
import "PropertyPanels"

Rectangle {
    id: root
    property MProperties mprop: canvas.SelectedMobject ? canvas.SelectedMobject.getProperties() : canvas.getProperties()

    color: "#121212"
    radius: 8
    border.color: "#2a2a2a"
    border.width: 1

    ScrollView {
        anchors.fill: parent
        anchors.margins: 16
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            spacing: 16

            // Mobject ID Display
            MobjectIdPanel {
                activeId: canvas.activeId || ""
                onDeleteRequested: {
                    if (canvas.activeId) {
                        canvas.removeMobject(canvas.activeId);
                    }
                }
            }

            // Name Input
            MobjectNamePanel {
                visible: mprop && mprop.base
                Layout.fillWidth: true
                mprop: root.mprop
            }

            // Position and Size
            TransformPanel {
                visible: mprop && mprop.base
                Layout.fillWidth: true
                mprop: root.mprop
            }

            // Colors
            AppearancePanel {
                visible: mprop && mprop.base
                Layout.fillWidth: true
                mprop: root.mprop
            }

            // Numeric Properties
            NumericPanel {
                visible: mprop && mprop.base
                Layout.fillWidth: true
                mprop: root.mprop
            }

            // MPolygon Point Management
            PolygonPanel {
                Layout.fillWidth: true
                mprop: root.mprop
                canvasRef: canvas
            }

            // Circle specific properties
            NumberInput {
                visible: mprop && mprop.circle
                label: "Radius:"
                value: mprop && mprop.circle ? mprop.circle.radius : 0
                func: () => {
                    if (mprop && mprop.circle)
                        mprop.circle.radius = newValue;
                }
            }

            NumberInput {
                visible: mprop && (mprop.type === "Curve" || mprop.type === "Circle")
                label: "Segments:"
                value: mprop && mprop.curve ? mprop.curve.segments : 0
                integersOnly: true
                func: () => {
                    if (mprop && mprop.curve)
                        mprop.curve.segments = newValue;
                }
            }

            // Line Properties Panel
            LinePanel {
                visible: root.mprop && root.mprop.line
                Layout.fillWidth: true
                mprop: root.mprop
            }

            // Curve Properties Panel
            CurvePanel {
                visible: root.mprop && root.mprop.curve
                Layout.fillWidth: true
                mprop: root.mprop
            }

            // Text Properties Panel
            TextPanel {
                visible: root.mprop && root.mprop.text
                Layout.fillWidth: true
                mprop: root.mprop
            }
        }
    }
}
