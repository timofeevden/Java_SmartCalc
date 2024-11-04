package smartcalc.view;

import java.io.*;
import javafx.application.Application;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;
import smartcalc.view.ViewController;

public class SmartCalcApp extends Application {
  public static void main(String[] args) {
    launch(args);
  }

  @Override
  public void start(Stage stage) {
    FXMLLoader fxmlLoader =
        new FXMLLoader(SmartCalcApp.class.getResource("/fxml/SmartCalcView.fxml"));
    try {
      Scene scene = new Scene(fxmlLoader.load());
      stage.setWidth(300);
      stage.setHeight(475);
      ((ViewController) fxmlLoader.getController()).updateHistory();
      stage.setTitle("SmartCalc v3.0"); // 294  	451
      stage.setScene(scene);
      stage.setResizable(false);
      stage.show();
    } catch (IOException e) {
      System.out.println("Trouble with fxml file: " + e);
      System.exit(-1);
    }
  }
}