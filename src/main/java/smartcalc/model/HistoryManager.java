package smartcalc.model;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class HistoryManager {
  private File historyFile;
  private FileWriter fileWriter;
  private String historyFilePath;

  public HistoryManager() {
    try {
      historyFilePath =
          new File(CalcModelImpl.class.getProtectionDomain().getCodeSource().getLocation().toURI())
              .getParent();
      historyFilePath = historyFilePath + "\\HistoryOfSmartCalc.txt";
      historyFile = new File(historyFilePath);
      historyFile.createNewFile();
    } catch (IOException | java.net.URISyntaxException e) {
      // e.printStackTrace();
    }
  }

  public void writeToHistory(String expression) {
    try (FileWriter writer = new FileWriter(historyFile, true)) {
      writer.write(expression);
      writer.write("\n");
    } catch (IOException e) {
      // e.printStackTrace();
    }
  }

  public List<String> loadHistroy(int historySize) {
    List<String> historyList = new ArrayList<String>(historySize);
    if (historyFile.exists()) {
      try {
        BufferedReader reader = new BufferedReader(new FileReader(historyFile));
        String line = reader.readLine();
        while (line != null && historySize-- > 0) {
          if (!line.equals("\n") || !line.equals("\r")) {
            historyList.add(line);
          }
          line = reader.readLine();
        }
        reader.close();
      } catch (IOException e) {
        // e.printStackTrace();
      }
    }
    return historyList;
  }
}