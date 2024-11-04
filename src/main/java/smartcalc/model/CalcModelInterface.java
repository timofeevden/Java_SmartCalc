package smartcalc.model;

import java.awt.Point;
import java.lang.reflect.Field;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

public interface CalcModelInterface {
  void calculate(String expression, double x);

  double getResult();

  int getError();

  List<String> getExpressionsHistory();

  void calculateGraphData(String expression, double xMin, double xMax, double yMin, double yMax);

  List<List> getGraphData();
}