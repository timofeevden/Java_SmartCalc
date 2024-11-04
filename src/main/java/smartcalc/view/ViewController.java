package smartcalc.view;

import java.awt.Point;
// import javafx.scene.Node;
import java.util.List;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.ScatterChart;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.MenuItem;
import javafx.scene.control.Spinner;
import javafx.scene.control.SplitMenuButton;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.layout.Pane;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import smartcalc.presenter.CalcPresenter;

public class ViewController implements CalcViewInterface {
  private final String RESULT_TEXT = "result";
  private final String ERROR_TEXT = "Error";
  private final String X_NAME = "x";
  private final String Y_NAME = "y";
  private final String Y_EQUAL = "y = ";
  private final String ARROW_LEFT = "<-";
  private final String ARROW_RIGHT = "->";
  private CalcPresenter presenter;
  private String expression = "";
  private String resultOutput = "";
  private double x = 0;
  private double xMax = 10;
  private double xMin = -10;
  private double yMax = 10;
  private double yMin = -10;

  @FXML private Label labelResult;
  @FXML private TextArea textAreaExpression;
  @FXML private Spinner<Double> spinnerX;
  @FXML private Spinner<Double> spinnerXmin;
  @FXML private Spinner<Double> spinnerXmax;
  @FXML private Spinner<Double> spinnerYmin;
  @FXML private Spinner<Double> spinnerYmax;
  @FXML private SplitMenuButton splitMenuHistory;
  @FXML private ScatterChart<Number, Number> graph;
  @FXML private Button buttonShowMenu;

  public ViewController() {
    presenter = new CalcPresenter(this);
  }

  @Override
  public String getExpression() {
    expression = textAreaExpression.getText();
    return expression;
  }

  @Override
  public double getX() {
    String valueText = spinnerX.getEditor().getText().replace(',', '.');
    x = Double.parseDouble(valueText);
    return x;
  }

  @Override
  public double getXmax() {
    String valueText = spinnerXmax.getEditor().getText().replace(',', '.');
    xMax = Double.parseDouble(valueText);
    return xMax;
  }

  @Override
  public double getXmin() {
    String valueText = spinnerXmin.getEditor().getText().replace(',', '.');
    xMin = Double.parseDouble(valueText);
    return xMin;
  }

  @Override
  public double getYmax() {
    String valueText = spinnerYmax.getEditor().getText().replace(',', '.');
    yMax = Double.parseDouble(valueText);
    return yMax;
  }

  @Override
  public double getYmin() {
    String valueText = spinnerYmin.getEditor().getText().replace(',', '.');
    yMin = Double.parseDouble(valueText);
    return yMin;
  }

  @Override
  public void updateResult(String resultString) {
    resultOutput = resultString;
    labelResult.setText(resultOutput);
  }

  @Override
  public void updateHistory(List<String> expressionsHistory) {
    splitMenuHistory.getItems().clear();
    for (int i = expressionsHistory.size() - 1; i >= 0; --i) {
      MenuItem oldExpression = new MenuItem(expressionsHistory.get(i));
      oldExpression.setOnAction(actionEvent -> {
        updateExpressionFromHistory(((MenuItem) actionEvent.getSource()).getText());
        presenter.calculate();
      });
      splitMenuHistory.getItems().add(oldExpression);
    }
  }

  private void updateExpressionFromHistory(String oldExpression) {
    expression = oldExpression;
    textAreaExpression.setText(expression);
  }

  @Override
  public void updateHistory() {
    presenter.updateHistory();
  }

  @FXML
  private void addNumber(ActionEvent actionEvent) {
    String buttonText = ((Button) actionEvent.getSource()).getText();
    expression = expression + buttonText;
    textAreaExpression.setText(expression);
  }

  @FXML
  private void addOperation(ActionEvent actionEvent) {
    String buttonText = ((Button) actionEvent.getSource()).getText();
    expression = expression + " " + buttonText + " ";
    textAreaExpression.setText(expression);
  }

  @FXML
  private void addMathFunction(ActionEvent actionEvent) {
    String buttonText = ((Button) actionEvent.getSource()).getText();
    expression = expression + buttonText + "(";
    textAreaExpression.setText(expression);
  }

  @FXML
  private void calculate(ActionEvent actionEvent) {
    presenter.calculate();
  }

  @FXML
  private void clearOne(ActionEvent actionEvent) {
    if (expression.length() > 0) {
      expression = expression.substring(0, expression.length() - 1);
    }
    textAreaExpression.setText(expression);
  }

  @FXML
  private void allClear(ActionEvent actionEvent) {
    expression = "";
    resultOutput = "";
    textAreaExpression.setText(expression);
    labelResult.setText(RESULT_TEXT);
  }

  @FXML
  void buildGraph(ActionEvent event) {
    NumberAxis xAxis = new NumberAxis(X_NAME, getXmin(), getXmax(), 1);
    NumberAxis yAxis = new NumberAxis(Y_NAME, getYmin(), getYmax(), 1);
    ScatterChart<Number, Number> graphChart = new ScatterChart<>(xAxis, yAxis);

    XYChart.Series<Number, Number> dataSeries = new XYChart.Series<>();
    dataSeries.setName(Y_EQUAL + expression);

    presenter.calculateGraphData(this.getExpression(), xMin, xMax, yMin, yMax);

    List<List> points = presenter.getGraphData();

    for (int i = 0; i < points.size(); ++i) {
      List iPoint = (List) points.get(i);
      double x = (double) iPoint.get(0);
      double y = (double) iPoint.get(1);
      dataSeries.getData().add(new XYChart.Data<>(x, y));
    }

    graphChart.getData().add(dataSeries);

    Pane newLayout = new Pane();
    newLayout.getChildren().add(graphChart);
    Scene secondScene = new Scene(newLayout);
    // New window (Stage)
    Stage newWindow = new Stage();
    newWindow.setTitle("Graph");
    newWindow.setResizable(false);
    newWindow.setScene(secondScene);
    newWindow.centerOnScreen();
    newWindow.show();
  }

  @FXML
  void openMoreOptions(ActionEvent event) {
    // Stage stage = (Stage) ((Node) event.getSource()).getScene().getWindow();
    Stage stage = (Stage) buttonShowMenu.getScene().getWindow();

    if (buttonShowMenu.getText().equals(ARROW_RIGHT)) {
      stage.setWidth(545);
      stage.setHeight(475);
      buttonShowMenu.setText(ARROW_LEFT);
    } else if (buttonShowMenu.getText().equals(ARROW_LEFT)) {
      stage.setWidth(300);
      stage.setHeight(475);
      buttonShowMenu.setText(ARROW_RIGHT);
    }
  }
}