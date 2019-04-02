import org.junit.Test;
import static org.junit.Assert.*;

public class TestCalculator {

    @Test
    public void TestAdd() throws Exception {
        Calculator calc = new Calculator();
        int actual = calc.add(3, 5);
        int expected = 8;
        assertEquals(expected, actual);
    }

    @Test
    public void TestSubtract() throws Exception {
        Calculator calc = new Calculator();
        int actual = calc.subtract(1, -1);
        int expected = 2;
        assertEquals(expected, actual);
    }

    @Test
    public void TestMultiply() throws Exception {
        Calculator calc = new Calculator();
        int actual = calc.multiply(6, 2);
        int expected = 12;
        assertEquals(expected, actual);
    }

    @Test
    public void TestDivide() throws Exception {
        Calculator calc = new Calculator();
        int actual = calc.divide(10, 5);
        int expected = 2;
        assertEquals(expected, actual);
    }

    @Test
    public void TestAddToLast() throws Exception {
        Calculator calc = new Calculator();
        calc.add(6, 2);
        int actual = calc.addToLast(10);
        int expected = 18;
        assertEquals(expected, actual);
    }

    @Test
    public void TestGetLast() throws Exception {
        Calculator calc = new Calculator();
        calc.subtract(100, 0);
        int actual = calc.getLast();
        int expected = 100;
        assertEquals(expected, actual);
    }
}
