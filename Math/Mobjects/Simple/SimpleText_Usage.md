// Example usage of SimpleText class
// 
// SimpleText is a lightweight text mobject designed for labels and simple text display
// without the overhead of clicking functionality. It inherits from Mobject rather than
// ClickableMobject for better performance in scenarios where interaction is not needed.
//
// Key Features:
// - No clicking/interaction capabilities (perfect for labels)
// - Lightweight compared to the full Text class
// - Essential text properties: text, color, fontSize, fontFamily, bold, italic
// - Simple positioning with setCenter() method
// - Automatic text metrics calculation for proper bounds
//
// Usage Example:
//
// // Create a simple label
// SimpleText* label = new SimpleText(scene, parent);
// label->setText("My Label");
// label->setColor(Qt::yellow);
// label->setFontSize(18);
// label->setBold(true);
// label->setCenter(100, 50);  // Position at center coordinates
//
// // Update text dynamically
// label->setText("Updated Label");
// label->setFontFamily("Times New Roman");
// label->setItalic(true);
//
// Properties available:
// - text (QString): The text content to display
// - color (QColor): Text color
// - fontSize (int): Font size in points
// - fontFamily (QString): Font family name (e.g., "Arial", "Times New Roman")
// - bold (bool): Bold text style
// - italic (bool): Italic text style  
// - position (QPointF): Text center position
//
// Methods:
// - setCenter(x, y): Position text by center point
// - textSize(): Get calculated text dimensions
//
// Use cases:
// - Axis labels in graphs
// - UI labels that don't need interaction
// - Static text elements
// - Performance-critical text rendering scenarios