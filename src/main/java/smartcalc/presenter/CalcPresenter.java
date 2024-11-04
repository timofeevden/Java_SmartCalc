package smartcalc.presenter;

import java.awt.Point;
import java.util.List;
import smartcalc.model.CalcModelImpl;
import smartcalc.model.CalcModelInterface;
import smartcalc.view.CalcViewInterface;

public class CalcPresenter {
  private final String ERROR_TEXT = "Error";
  private final String EQUAL_TEXT = " = ";
  private CalcModelInterface model;
  private CalcViewInterface view;

  public CalcPresenter(CalcViewInterface view) {
    this.view = view;
    model = new CalcModelImpl();
  }

  public void calculate() {
    String expression = view.getExpression();
    if (expression == null || expression.isEmpty()) {
      return;
    }
    double x = view.getX();
    model.calculate(expression, x);
    int errorNumber = model.getError();

    String resultString = "";
    if (errorNumber != 0) {
      // System.out.println(ERROR_TEXT + EQUAL_TEXT + errorNumber);
      resultString = ERROR_TEXT;
    } else {
      resultString = String.valueOf(model.getResult());
    }
    view.updateResult(resultString);
    view.updateHistory(model.getExpressionsHistory());
  }

  public void updateHistory() {
    view.updateHistory(model.getExpressionsHistory());
  }

  public List<String> getHistory() {
    return model.getExpressionsHistory();
  }

  public void calculateGraphData(
      String expression, double xMin, double xMax, double yMin, double yMax) {
    model.calculateGraphData(expression, xMin, xMax, yMin, yMax);
  }

  public List<List> getGraphData() {
    return model.getGraphData();
  }
}