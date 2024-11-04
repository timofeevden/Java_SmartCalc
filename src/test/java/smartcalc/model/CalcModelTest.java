package smartcalc.model;

import java.lang.Math;
import java.util.List;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvFileSource;
import org.junit.jupiter.params.provider.ValueSource;

public class CalcModelTest {
  private CalcModelInterface model = new CalcModelImpl();
  private double delta = 0.000001;
  private int HISTORY_SIZE = 10;

  @Test
  public void historyExpressionsTest() {
    model.calculate("1", 1.0);
    model.calculate("1", 1.0);
    List<String> history = model.getExpressionsHistory();
    Assertions.assertEquals(history.get(history.size() - 1), "1");

    for (int i = 2; i <= HISTORY_SIZE + 1; ++i) {
      model.calculate(String.valueOf(i), 1.0);
    }
    history = model.getExpressionsHistory();
    Assertions.assertEquals(history.size(), HISTORY_SIZE);
    Assertions.assertEquals(history.get(0), "2");
  }

  @ParameterizedTest
  @CsvFileSource(resources = "/errorsTestData.csv")
  public void errorExpressionsTest(String expression) {
    model.calculate(expression, 1.0);
    Assertions.assertTrue(model.getError() != 0);
  }

  @ParameterizedTest
  @CsvFileSource(resources = "/testData.csv")
  public void someExpressionsTest(String expression, double answer) {
    model.calculate(expression, 1.0);
    double result = model.getResult();
    Assertions.assertEquals(result, answer, delta);
  }

  @ParameterizedTest
  @CsvFileSource(resources = "/xTestData.csv")
  public void someExpressionsTest(String expression, double x, double answer) {
    model.calculate(expression, x);
    double result = model.getResult();
    Assertions.assertEquals(result, answer, delta);
  }

  @Test
  public void moreSomeExpressionsTest() {
    model.calculate("-5mod(-2)", 1.0);
    double result = model.getResult();
    double answer = -5 % -2;
    Assertions.assertEquals(result, answer);

    model.calculate("-5mod2", 1.0);
    result = model.getResult();
    answer = -5 % 2;
    Assertions.assertEquals(result, answer);

    model.calculate("5mod(-2)", 1.0);
    result = model.getResult();
    answer = 5 % -2;
    Assertions.assertEquals(result, answer);

    model.calculate("5 mod 3", 1.0);
    result = model.getResult();
    answer = 2.0;
    Assertions.assertEquals(result, answer);

    model.calculate("-sin(50)", 1.0);
    result = model.getResult();
    answer = -Math.sin(50.0);
    Assertions.assertEquals(result, answer);

    model.calculate("-asin(1)", 1.0);
    result = model.getResult();
    answer = -Math.asin(1.0);
    Assertions.assertEquals(result, answer);

    model.calculate("-cos(50)", 1.0);
    result = model.getResult();
    answer = -Math.cos(50.0);
    Assertions.assertEquals(result, answer);

    model.calculate("-(acos(1))", 1.0);
    result = model.getResult();
    answer = -(Math.acos(1.0));
    Assertions.assertEquals(result, answer);

    model.calculate("-tan(50)", 1.0);
    result = model.getResult();
    answer = -Math.tan(50.0);
    Assertions.assertEquals(result, answer);

    model.calculate("-ln(50)", 1.0);
    result = model.getResult();
    answer = -Math.log(50.0);
    Assertions.assertEquals(result, answer);

    model.calculate("-lg(50.0)", 1.0);
    result = model.getResult();
    answer = -Math.log10(50.0);
    Assertions.assertEquals(result, answer);

    model.calculate("sin(Pi)", 1.0);
    result = model.getResult();
    answer = Math.sin(Math.PI);
    Assertions.assertEquals(result, answer, delta);

    model.calculate("Pi", 1.0);
    result = model.getResult();
    answer = Math.PI;
    Assertions.assertEquals(result, answer);

    model.calculate("sin(50)*cos(-50)", 1.0);
    result = model.getResult();
    answer = Math.sin(50.0) * Math.cos(-50);
    Assertions.assertEquals(result, answer);

    model.calculate("ln(10) + lg(2) * sqrt(4)", 1.0);
    result = model.getResult();
    answer = Math.log(10.0) + Math.log10(2.0) * Math.sqrt(4.0);
    Assertions.assertEquals(result, answer);

    model.calculate("acos(-0.06e+1)+asin(0.9)-atan(0.3)", 1.0);
    result = model.getResult();
    answer = Math.acos(-0.06e+1) + Math.asin(0.9) - Math.atan(0.3);
    Assertions.assertEquals(result, answer);

    model.calculate("sin(cos(ln(sqrt(lg(45.868)))))", 1.0);
    result = model.getResult();
    answer = Math.sin(Math.cos(Math.log(Math.sqrt(Math.log10(45.868)))));
    Assertions.assertEquals(result, answer);
  }

  @Test
  public void graphDataTest() {
    model.calculateGraphData("5", 0.0, 5.0, -5.0, 5.0);
    List<List> graphData = model.getGraphData();

    for (int i = 0; i < graphData.size(); ++i) {
      Assertions.assertEquals(graphData.get(0).get(1), 5.0);
    }

    model.calculateGraphData("5", 0.0, 5.0, -5.0, 0.0);
    graphData = model.getGraphData();
    Assertions.assertEquals(graphData.size(), 0);

    model.calculateGraphData("qweafawasfg/0", 0.0, 5.0, -5.0, 0.0);
    graphData = model.getGraphData();
    Assertions.assertEquals(graphData.size(), 0);
  }
}
