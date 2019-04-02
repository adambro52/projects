// Adam Smith 5135927
// Carolyn Kalinowski 4926023

import java.awt.Color;
import java.util.Scanner;
import java.util.Random;

public class fractals {

    public static Canvas drawing;
    public static double area = 0;
    public static Color[] colorArray = {Color.BLUE, Color.YELLOW, Color.RED, Color.GREEN, Color.BLACK, Color.CYAN, Color.MAGENTA, Color.ORANGE, Color.PINK};

  public static void main(String[] args){
    
     String cont = "y";
     Scanner a = new Scanner(System.in);
     do { //continuation loop to see all of the random pretty colors on as many fractals as you want easily


      System.out.println("Enter fractal type: ");
      String fractalType = a.next();

      int canvasWidth = 1000, canvasHeight = 1000;
      drawing = new Canvas(canvasWidth,canvasHeight);

        if (fractalType.equals("circle")) {

          recursiveCircle(canvasWidth/2,canvasHeight/2,200);

          }

        else if (fractalType.equals("triangle")) {

          recursiveTriangle(canvasWidth/4+100,5*canvasHeight/6,250,400);

          }

        else if (fractalType.equals("rectangle")) {

          recursiveRectangle(canvasWidth/4,canvasHeight/4,400,400);

          }

        else {

          System.out.println("Invalid fractal type. ");

          }

           System.out.println("Would you like to run again? (y/n) ");
           cont = a.next();

        }

        
      while (cont.equals("y"));

      
    }
  public static void recursiveRectangle(double xStart, double yStart, double initWidth, double initHeight){ 

    if( initWidth < 1){
      return;
    }
    else{
      Rectangle initRectangle = new Rectangle(xStart,yStart,initWidth,initHeight);

      Random rand = new Random();
      int s = rand.nextInt(8);
      initRectangle.setColor(colorArray[s]); //assigns random color to shape #pride

      drawing.drawShape(initRectangle); //Starts drawing with x,y in the top left corner

      area = area + initRectangle.calculateArea();

      double recursiveRatio = 2;

      recursiveRectangle(initRectangle.getXPos()-initWidth/(recursiveRatio*2), initRectangle.getYPos()-initHeight/(recursiveRatio*2),initWidth/recursiveRatio,initHeight/recursiveRatio);
      recursiveRectangle(initRectangle.getXPos()-initWidth/(recursiveRatio*2), initRectangle.getYPos()+initHeight-initHeight/(recursiveRatio*2), initWidth/recursiveRatio,initHeight/recursiveRatio);
      recursiveRectangle(initRectangle.getXPos()+initWidth-initWidth/(recursiveRatio*2), initRectangle.getYPos()-initHeight/(recursiveRatio*2), initWidth/recursiveRatio,initHeight/recursiveRatio);
      recursiveRectangle(initRectangle.getXPos()+initWidth-initWidth/(recursiveRatio*2), initRectangle.getYPos()+initHeight-initHeight/(recursiveRatio*2), initWidth/recursiveRatio,initHeight/recursiveRatio);

      }

      System.out.println(area);

    }
  public static void recursiveTriangle(double xStart, double yStart, double initWidth, double initHeight){

    if (initWidth < 1){
      return;
    }
    else {
      Triangle initTriangle = new Triangle(xStart,yStart,initWidth,initHeight);

      Random rand = new Random();
      int s = rand.nextInt(8);
      initTriangle.setColor(colorArray[s]); //assigns random color to shape #pride

      drawing.drawShape(initTriangle); //Starts drawing with x,y in the bottom left corner

      area = area + initTriangle.calculateArea();

      double recursiveRatio = 2;

      recursiveTriangle(initTriangle.getXPos()+initWidth, initTriangle.getYPos(),initWidth/recursiveRatio,initHeight/recursiveRatio);
      recursiveTriangle(initTriangle.getXPos()-initWidth/recursiveRatio, initTriangle.getYPos(),initWidth/recursiveRatio,initHeight/recursiveRatio);
      recursiveTriangle(initTriangle.getXPos()+initWidth/2-initWidth/(recursiveRatio*2), initTriangle.getYPos()-initHeight,initWidth/recursiveRatio,initHeight/recursiveRatio);

      System.out.println(area);

      }
    }
  public static void recursiveCircle(double xStart, double yStart, double initRadius)
  {

    if (initRadius < 1){
      return;
    }
    else
    {
      Circle initCircle = new Circle(xStart,yStart,initRadius);

      Random rand = new Random();
      int s = rand.nextInt(8);
      initCircle.setColor(colorArray[s]);  //assigns random color to shape #pride

      drawing.drawShape(initCircle); //Starts drawing with x,y in the center

      area = area + initCircle.calculateArea();

      double recursiveRatio = 2;

      recursiveCircle(initCircle.getXPos(), initCircle.getYPos()-initRadius-(initRadius/(recursiveRatio)),initRadius/recursiveRatio);
      recursiveCircle(initCircle.getXPos()+initRadius+(initRadius/(recursiveRatio)), initCircle.getYPos(),initRadius/recursiveRatio);
      recursiveCircle(initCircle.getXPos()-initRadius-(initRadius/(recursiveRatio)), initCircle.getYPos(),initRadius/recursiveRatio);
      recursiveCircle(initCircle.getXPos(), initCircle.getYPos()+initRadius+(initRadius/(recursiveRatio)),initRadius/recursiveRatio);

      System.out.println(area);

      }
    }
  }
  