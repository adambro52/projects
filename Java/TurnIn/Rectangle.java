import java.awt.Color;

public class Rectangle {
		
		double xpositionbl, ypositionbl, width, perimeter, area, height;
		private Color color;
		
		public Rectangle(double xpositionbl, double ypositionbl, double width, double height){ 
			this.xpositionbl = xpositionbl; 
			this.ypositionbl = ypositionbl;
			this.width = width;
			this.height=height;
		}
		
		public double calculatePerimeter() {
			double perimeter=2*width+2*height;
			return perimeter;
		}
		
		public double calculateArea() { 
			double area=width*height;
			return area;
		}
		
		public void setColor(Color col){
			color=col;
		}
		
		public void setPos(double x, double y) {
			xpositionbl=x;
			ypositionbl=y;
		}
		
		public void setHeight(double hi) {
			height=hi;
		}
		
		public void setWidth(double wi) {
			width=wi;
		}
		 
		public Color getColor() {
			return color;
		}
		
		public double getXPos() {
			return xpositionbl;
		}
		
		public double getYPos() {
			return ypositionbl;
		}
		
		public double getHeight() {
			return height;
		}
		
		public double getWidth() {
			return width;
		}

}
	