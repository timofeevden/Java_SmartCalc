#ifndef SRC_MODEL_CALCMODEL_H
#define SRC_MODEL_CALCMODEL_H

#include <cmath>
#include <stack>
#include <string>
#include <vector>

namespace s21 {
/**
 * @brief Класс модели стандартного калькулятора
 */
class CalcModel {
 public:
  /**
   * @brief Конструктор класса модели по-умолчанию
   */
  CalcModel();
  /**
   * @brief Конструктор класса модели копированием
   * @param other другая модель
   */
  CalcModel(const CalcModel& other);
  /**
   * @brief Перевод строки в ОПН
   * @param new_expression выражение, которое нужно перевести
   */
  int TranslateToPolish(const std::string& new_expression);
  /**
   * @brief Вычисление переведенного выражения
   */
  int CalculateExpression();
  /**
   * @brief Геттер выражения
   */
  const std::string& GetExpression() noexcept;
  /**
   * @brief Геттер переведенного выражения
   */
  const std::string& GetPolishExpression() noexcept;
  /**
   * @brief Геттер ошибки
   */
  int GetError() noexcept;
  /**
   * @brief Геттер результата вычисления
   */
  double GetResult() noexcept;
  /**
   * @brief Задать новое значение Х
   * @param new_x Новое значение Х
   */
  void SetX(const double& new_x) noexcept;
  /**
   * @brief Задать новое выражение
   * @param new_expression Новое выражение
   */
  void SetExpression(const std::string& new_expression);
  /**
   * @brief Вычислить данные для графика
   * @param x_min Минимальное значение Х
   * @param x_max Максимальное значение Х
   * @param y_min Минимальное значение Y
   * @param y_max Максимальное значение Y
   */
  void CalculateDataGraph(double x_min, double x_max, double y_min,
                          double y_max);
  /**
   * @brief Геттер данных для графика для Х
   */
  const std::vector<double>& GetDataX() noexcept;
  /**
   * @brief Геттер данных для графика для Y
   */
  const std::vector<double>& GetDataY() noexcept;

 private:
  std::string expression_;
  std::string polish_expression_;
  int error_;
  double result_;
  double x_ = 0;
  double step_ = 0.2;
  std::vector<double> data_x_, data_y_;

 private:
  bool CheckBrackets(int& i, std::stack<char>& stack_operators);
  bool CheckOperators(int& i, std::stack<char>& stack_operators);
  bool CheckUnar(int& i, std::stack<char>& stack_operators);
  bool CheckNumber(int& i);
  bool IsOperator(const std::string& expression, int i) noexcept;
  bool IsBrktFunc(int i) noexcept;
  bool IsNumber(char symbol) noexcept;
  bool IsDot(char symbol) noexcept;
  bool LeftAssoc(char operatr) noexcept;
  int Priority(char operatr) noexcept;
  double TakeNumber(int& i);
  void DoArithmetic(double* numbers, int& i_str, int& i_num);
  void DoMathFunc(double* numbers, int& i_str, int& i_num, double& unar_minus);
};

}  //  namespace s21

#endif  // SRC_MODEL_CALCMODEL_H
