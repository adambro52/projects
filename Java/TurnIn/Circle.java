import java.awt.Color;

public class Circle {
		
		double xposition, yposition, radius, perimeter, area;
		private Color color;
		
		public Circle(double xposition, double yposition, double radius){ 
			this.xposition = xposition; 
			this.yposition = yposition;
			this.radius = radius;
		}
		
		public double calculatePerimeter() {
			double perimeter=2*(Math.PI)*radius;
			return perimeter;
		}
		
		public double calculateArea() { 
			double area=Math.PI*radius*radius;
			return area;
		}
		
		public void setColor(Color col){
			color=col;
		}
		
		public void setPos(double x, double y) {
			xposition=x;
			yposition=y;
		}
		
		public void setRadius(double rad) {
			radius=rad;
		}
		 
		public Color getColor() {
			return color;
		}
		
		public double getXPos() {
			return xposition;
		}
		
		public double getYPos() {
			return yposition;
		}
		
		public double getRadius() {
			return radius;
		}

}