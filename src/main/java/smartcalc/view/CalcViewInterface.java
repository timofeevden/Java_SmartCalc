package smartcalc.view;

import java.util.List;

public interface CalcViewInterface {
  String getExpression();

  double getX();

  double getXmax();

  double getXmin();

  double getYmax();

  double getYmin();

  void updateResult(String resultOutput);

  void updateHistory();

  void updateHistory(List<String> expressionsHistory);
}