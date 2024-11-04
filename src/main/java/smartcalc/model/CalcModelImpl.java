package smartcalc.model;

import java.io.File;
import java.lang.reflect.Field;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import smartcalc.model.CalcModelInterface;
import smartcalc.model.HistoryManager;

public class CalcModelImpl implements CalcModelInterface {
  private final int HISTORY_SIZE = 10;
  private HistoryManager historyManager;
  private String expression;
  private String polishExpression;
  private int errorNumber;
  private double x;
  private double result;
  private List<String> expressionsHistory;

  public CalcModelImpl() {
    historyManager = new HistoryManager();
    expressionsHistory = historyManager.loadHistroy(HISTORY_SIZE);
    errorNumber = 0;
    result = 0.0;
    x = 1.0;
  }

  static {
    try {
      String libDllPath =
          new File(CalcModelImpl.class.getProtectionDomain().getCodeSource().getLocation().toURI())
              .getParent();
      // System.out.println("Current absolute path is: " + libDllPath + "\nPRPETRY:\n" + dirPath);
      File liba = new File(libDllPath + "polishCalcLib.dll");
      System.setProperty("java.library.path", libDllPath);
      Field fieldSysPath = ClassLoader.class.getDeclaredField("sys_paths");
      fieldSysPath.setAccessible(true);
      fieldSysPath.set(null, null);
      System.loadLibrary("polishCalcLib");
    } catch (UnsatisfiedLinkError e) {
      System.out.println(e);
    } catch (Exception e) {
      System.out.println(e);
    }
  }

  @Override
  public void calculate(String expression, double x) {
    translateToPolish(expression);
    saveInHistory(expression);
    if (getErrorNumber() == 0) {
      calculatePolishExpression(x);
    }
  }

  @Override
  public int getError() {
    return getErrorNumber();
  }

  @Override
  public double getResult() {
    return getCalculatingResult();
  }

  @Override
  public List<String> getExpressionsHistory() {
    return expressionsHistory;
  }

  private void saveInHistory(String expression) {
    if (!expressionsHistory.isEmpty()
        && expression.equals(expressionsHistory.get(expressionsHistory.size() - 1))) {
      return;
    }
    if (expressionsHistory.size() < HISTORY_SIZE) {
      expressionsHistory.add(expression);
    } else {
      for (int i = 0; i < HISTORY_SIZE - 1; ++i) {
        expressionsHistory.set(i, expressionsHistory.get(i + 1));
      }
      expressionsHistory.set(HISTORY_SIZE - 1, expression);
    }
    historyManager.writeToHistory(expression);
  }

  @Override
  public void calculateGraphData(
      String expression, double xMin, double xMax, double yMin, double yMax) {
    translateToPolish(expression);
    if (getErrorNumber() == 0) {
      calculateGraphData(xMin, xMax, yMin, yMax);
    }
  }

  @Override
  public List<List> getGraphData() {
    List<List> dataList = getGraphDataFromDLL();
    if (dataList == null) {
      dataList = new ArrayList<>();
    }
    return dataList;
  }

  private native String translateToPolish(String expression);

  private native double calculatePolishExpression(double x);

  private native void calculateGraphData(double xMin, double xMax, double yMin, double yMax);

  private native List<List> getGraphDataFromDLL();

  private native int getErrorNumber();

  private native double getCalculatingResult();
}