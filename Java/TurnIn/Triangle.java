import java.awt.Color;

public class Triangle {
		
		double xpositionbl, ypositionbl, width, perimeter, area, height;
		private Color color;
		
		public Triangle(double xpositionbl, double ypositionbl, double width, double height){ 
			this.xpositionbl = xpositionbl; 
			this.ypositionbl = ypositionbl;
			this.width = width;
			this.height=height;
		}
		
		public double calculatePerimeter() {
			double perimeter=width+2*Math.sqrt(.25*(width*width)+(height*height));
			return perimeter;
		}
		
		public double calculateArea() { 
			double area=width*height/2.0;
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
		
