import java.awt.Color;

public class canvasTest{
	public static void main(String[] args){
		//Canvas drawing = new Canvas(1000,1000);
		Circle myCircle = new Circle(0,0,400);
		myCircle.setColor(Color.BLUE);
		//drawing.drawShape(myCircle);

		Triangle a = new Triangle(100,400,300,300);
		a.setColor(Color.GREEN);
		//drawing.drawShape(a);

		Rectangle r = new Rectangle(110,110,500,500);
		r.setColor(Color.RED);
		//drawing.drawShape(r);

		//System.out.println(r.getXPos());
		System.out.println(a.calculateArea());
	}
}