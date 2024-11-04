#include "calc_model.h"

//  2 - лишнняя точка
//  3 - скобки
//  4 - ошибка в экспонентациаьной записи
//  5 - недостаточно чисел для операций или матем. ф-ий или ничего не подошло
//  при вычислении 6 - деление на 0

namespace s21 {

//  Конструктор по-умолчанию
CalcModel::CalcModel()
    : expression_(""),
      polish_expression_(""),
      error_(0),
      result_(0.0),
      x_(0.0){};

//  Конструктор копированием
CalcModel::CalcModel(const CalcModel& other)
    : error_(other.error_), result_(other.result_), x_(other.x_) {
  expression_ = other.expression_;
  polish_expression_ = other.polish_expression_;
}

//  ===================== EXPRESSION PARCER, TRANSLATE TO  POLISH NOTATION
//  ======================================================================

//  Перевод выражения в польскую нотацию
int CalcModel::TranslateToPolish(const std::string& new_expression) {
  this->SetExpression(new_expression);
  data_x_.clear();
  data_y_.clear();
  result_ = 0;
  if (expression_.empty()) {
    error_ = 1;  //  пустая строка
    return error_;
  }
  error_ = 0;
  //	если цифры, то отправляем в строку polish_expression_ через пробелы
  //	если оператор, то кидаем в стэк
  std::stack<char> stack_operators;
  for (int i = 0; expression_[i] != '\0' && !error_; ++i) {
    if (expression_[i] == ' ' || expression_[i] == '\t' ||
        expression_[i] == '\n') {
      //  пробелы, табы, переносы пропускаем
    } else if (CheckUnar(i, stack_operators)) {
      //  унарный минус
    } else if (CheckNumber(i)) {
      //  число
    } else if (CheckBrackets(i, stack_operators)) {
      //  скобки или оператор со скобками
    } else if (CheckOperators(i, stack_operators)) {
      //  оператор
    } else if (expression_[i] == 'x' || expression_[i] == 'X') {
      polish_expression_.push_back('X');  //  число Х
    } else if (expression_[i] == 'P' && expression_[i + 1] == 'i') {
      polish_expression_.push_back('P');  //  число Pi
      ++i;
    } else {
      error_ = 1;  //  лишний нечитаемый нами символ
    }
  }
  //  из стека отправляем все в конец строки
  while (stack_operators.empty() != true) {
    polish_expression_.push_back(stack_operators.top());
    stack_operators.pop();
  }
  if (polish_expression_.empty()) {
    error_ = 1;  //  Если переведенная строка пустая
  }

  return error_;
}

//  Проверяем унарный минус в выржении
bool CalcModel::CheckUnar(int& i, std::stack<char>& stack_operators) {
  bool was_unar = false;
  if ((expression_[i] == '-' || expression_[i] == '+') &&
      (IsNumber(expression_[i + 1]) || IsDot(expression_[i + 1]) || i == 0 ||
       expression_[i + 1] == '(' || IsBrktFunc(i + 1))) {
    if (i == 0) {
      if (expression_[i] == '-') {
        stack_operators.push('~');
      }
      was_unar = true;
    } else if (expression_[i - 1] == '(' || expression_[i - 1] == '+' ||
               expression_[i - 1] == '-' || expression_[i - 1] == '*' ||
               expression_[i - 1] == '/' || expression_[i - 1] == '^') {
      if (expression_[i] == '-') {
        stack_operators.push('~');
      }
      was_unar = true;
    }
  }

  return was_unar;
}

//  Проверяем на число в выражении
bool CalcModel::CheckNumber(int& i) {
  bool was_number = false;
  if (IsNumber(expression_[i]) || IsDot(expression_[i]) ||
      (expression_[i] == 'e' || expression_[i] == 'E')) {
    bool was_dot = false;
    bool was_exp = false;
    while (!error_ && (IsNumber(expression_[i]) || IsDot(expression_[i]) ||
                       (expression_[i] == 'e' || expression_[i] == 'E'))) {
      if (IsDot(expression_[i])) {
        if (was_dot == false) {  //  Проверяем корректность точки
          if (i == 0) {
            polish_expression_.push_back('0');
          } else if (!IsNumber(expression_[i - 1])) {
            polish_expression_.push_back('0');
          }
          polish_expression_.push_back('.');
          ++i;
          was_dot = true;
        } else {
          error_ = 2;  //  лишняя точка
        }
      } else if (expression_[i] == 'E' || expression_[i] == 'e') {
        if (!was_exp && was_number) {  //  Проверяем корректность E
          polish_expression_.push_back('E');
          ++i;
          was_exp = true;
          if (expression_[i] == '-' || expression_[i] == '+') {
            polish_expression_.push_back(expression_[i++]);
            if (expression_[i] == '-' || expression_[i] == '+') {
              error_ = 4;  //  повтор унароного знака после Е
            }
          }
          if (IsNumber(expression_[i])) {
            polish_expression_.push_back(expression_[i++]);
          } else {
            error_ = 4;  //  после Е нет числа
          }
        } else {
          error_ = 4;  //  лишняя E в числе
        }
      } else {
        polish_expression_.push_back(expression_[i++]);
        was_number = true;
      }
    }
    polish_expression_.push_back(' ');
    --i;
  }
  return was_number;
}

//  Проверяем выражение на скобки и функции
bool CalcModel::CheckBrackets(int& i, std::stack<char>& stack_operators) {
  bool was_bkt = false;
  if (i > 0 && expression_[i] == '(' && !IsOperator(expression_, i - 1)) {
    if (expression_[i - 1] != '(') {
      error_ = 3;  //  перед '(' нет действия
    }
  }  //  если скобка открывается, добавляем её в стек:
  if (!error_ && (expression_[i] == '(' || IsBrktFunc(i))) {
    if (expression_[i] == 'l' && expression_[i + 1] == 'n') {
      stack_operators.push('l');
      i += 2;
    } else if (expression_[i] == 'l' && expression_[i + 1] == 'g') {
      stack_operators.push('L');
      i += 2;
    } else if (expression_[i] == 's' && expression_[i + 1] == 'i' &&
               expression_[i + 2] == 'n') {
      stack_operators.push('s');
      i += 3;
    } else if (expression_[i] == 'c' && expression_[i + 1] == 'o' &&
               expression_[i + 2] == 's') {
      stack_operators.push('c');
      i += 3;
    } else if (expression_[i] == 't' && expression_[i + 1] == 'a' &&
               expression_[i + 2] == 'n') {
      stack_operators.push('t');
      i += 3;
    } else if (expression_[i] == 'a' && expression_[i + 1] == 'c' &&
               expression_[i + 2] == 'o' && expression_[i + 3] == 's') {
      stack_operators.push('C');
      i += 4;
    } else if (expression_[i] == 'a' && expression_[i + 1] == 's' &&
               expression_[i + 2] == 'i' && expression_[i + 3] == 'n') {
      stack_operators.push('S');
      i += 4;
    } else if (expression_[i] == 'a' && expression_[i + 1] == 't' &&
               expression_[i + 2] == 'a' && expression_[i + 3] == 'n') {
      stack_operators.push('T');
      i += 4;
    } else if (expression_[i] == 's' && expression_[i + 1] == 'q' &&
               expression_[i + 2] == 'r' && expression_[i + 3] == 't') {
      stack_operators.push('Q');
      i += 4;
    }
    stack_operators.push(expression_[i]);
    was_bkt = true;
  } else if (expression_[i] == ')') {
    //  вытаскиваем все в строку до отрывающей скобки
    while (!stack_operators.empty() && stack_operators.top() != '(') {
      polish_expression_.push_back(stack_operators.top());
      stack_operators.pop();
    }

    if (!stack_operators.empty()) {  //  была '('
      stack_operators.pop();         //  удаляем '(' из стека
    } else {
      error_ = 3;  //  не было '('
    }
    was_bkt = true;
  }

  return was_bkt;
}

//  Проверяем выражение на операторы
bool CalcModel::CheckOperators(int& i, std::stack<char>& stack_operators) {
  if (expression_[i] == '+' || expression_[i] == '-' || expression_[i] == '*' ||
      expression_[i] == '/' || expression_[i] == '^' ||
      (expression_[i] == 'm' && expression_[i + 1] == 'o' &&
       expression_[i + 2] == 'd')) {
    if (stack_operators.empty() == false) {
      //  разворачиваем стек по приоритету
      while (!stack_operators.empty() &&
             ((LeftAssoc(expression_[i]) &&
               Priority(stack_operators.top()) >= Priority(expression_[i])) ||
              (!LeftAssoc(expression_[i]) &&
               Priority(stack_operators.top()) > Priority(expression_[i])))) {
        polish_expression_.push_back(stack_operators.top());
        stack_operators.pop();
      }
    }
    stack_operators.push(expression_[i]);
    if (expression_[i] == 'm' && expression_[i + 1] == 'o' &&
        expression_[i + 2] == 'd') {
      i += 2;
    }
    return true;
  }

  return false;
}

//  Определяем приоритет оператора
int CalcModel::Priority(char operatr) noexcept {
  int priority = 0;

  if (operatr == 's' || operatr == 'S' || operatr == 'c' || operatr == 'C' ||
      operatr == 't' || operatr == 'T' || operatr == 'l' || operatr == 'L' ||
      operatr == 'Q') {
    priority = 5;
  } else if (operatr == '^') {
    priority = 5;
  } else if (operatr == '*' || operatr == '/' || operatr == '%' ||
             operatr == 'm') {
    priority = 4;
  } else if (operatr == '+' || operatr == '-') {
    priority = 3;
  } else if (operatr == '~') {
    priority = 6;
  } else if (operatr == '(') {
    priority = 1;
  }

  return priority;
}

//  Проверяем на скобочную ф-ию
bool CalcModel::IsBrktFunc(int i) noexcept {
  if ((expression_[i] == 'l' && expression_[i + 1] == 'n' &&
       expression_[i + 2] == '(') ||

      (expression_[i] == 's' && expression_[i + 1] == 'i' &&
       expression_[i + 2] == 'n' && expression_[i + 3] == '(') ||

      (expression_[i] == 'c' && expression_[i + 1] == 'o' &&
       expression_[i + 2] == 's' && expression_[i + 3] == '(') ||

      (expression_[i] == 't' && expression_[i + 1] == 'a' &&
       expression_[i + 2] == 'n' && expression_[i + 3] == '(') ||

      (expression_[i] == 'l' && expression_[i + 1] == 'g' &&
       expression_[i + 2] == '(') ||

      (expression_[i] == 'a' && expression_[i + 1] == 's' &&
       expression_[i + 2] == 'i' && expression_[i + 3] == 'n' &&
       expression_[i + 4] == '(') ||

      (expression_[i] == 'a' && expression_[i + 1] == 'c' &&
       expression_[i + 2] == 'o' && expression_[i + 3] == 's' &&
       expression_[i + 4] == '(') ||

      (expression_[i] == 'a' && expression_[i + 1] == 't' &&
       expression_[i + 2] == 'a' && expression_[i + 3] == 'n' &&
       expression_[i + 4] == '(') ||

      (expression_[i] == 's' && expression_[i + 1] == 'q' &&
       expression_[i + 2] == 'r' && expression_[i + 3] == 't' &&
       expression_[i + 4] == '(')) {
    return true;
  }

  return false;
}

//  Проверяем символ на цифру
bool CalcModel::IsNumber(char symbol) noexcept {
  if (symbol >= '0' && symbol <= '9') {
    return true;
  }
  return false;
}

//  Проверяем символ на точку\запятую
bool CalcModel::IsDot(char symbol) noexcept {
  if (symbol == '.' || symbol == ',') {
    return true;
  }
  return false;
}

//  Проверяем на левую ассиативность
bool CalcModel::LeftAssoc(char operatr) noexcept {
  if (operatr == '^') {
    return false;
  }

  return true;
}

//  ======================================== CALCULATE EXPRESSION
//  ======================================================================

//  Логика вычисления
int CalcModel::CalculateExpression() {
  if (error_) {
    return error_;
  }
  double numbers[255] = {0};
  int i_str = 0, i_num = 0;

  for (double unar_minus = 1; polish_expression_[i_str] != '\0' && !error_;
       ++i_str) {
    if (IsOperator(polish_expression_, i_str) &&
        i_num > 0) {  //  если матем. оператор
      DoArithmetic(numbers, i_str, i_num);
    } else if (IsNumber(polish_expression_[i_str])) {  //  если число
      numbers[i_num++] = unar_minus * TakeNumber(i_str);
      unar_minus = 1;
    } else if (polish_expression_[i_str] == '~' &&
               i_num > 0) {  //  если унарный минус
      numbers[i_num - 1] = (-1) * numbers[i_num - 1];
      unar_minus = 1;
    } else if (polish_expression_[i_str] == 'X') {  //  если X
      numbers[i_num++] = x_;
    } else if (polish_expression_[i_str] == 'P') {  //  если Pi
      numbers[i_num++] = M_PI;
    } else if (i_num > 0) {  //  смотрим матем. ф-ии:
      DoMathFunc(numbers, i_str, i_num, unar_minus);
    } else {
      //  недостаточно чисел для операций или матем. ф-ий или ничего не подошло
      error_ = 5;
    }
  }

  if (!error_) {
    if (i_num != 0) {
      --i_num;  //  для мат. ф-ий типа sqrt(x))
    }
    result_ = numbers[i_num];
  } else {
    result_ = 0;
  }

  return error_;
}

//  Проверяем на оператор
bool CalcModel::IsOperator(const std::string& expression, int i) noexcept {
  bool was_operator = false;
  if (expression[i] == '^' || expression[i] == '*' || expression[i] == '/' ||
      expression[i] == '+' || expression[i] == '-' || expression[i] == 'm') {
    was_operator = true;
  } else if (i >= 2) {
    if (expression[i - 2] == 'm' && expression[i - 1] == 'o' &&
        expression[i] == 'd') {
      was_operator = true;
    }
  }

  return was_operator;
}

//  Считаем арифметику
void CalcModel::DoArithmetic(double* numbers, int& i_str, int& i_num) {
  if (IsOperator(polish_expression_, i_str)) {
    if (i_num > 1) {
      if (polish_expression_[i_str] == '+') {
        numbers[i_num - 2] = numbers[i_num - 2] + numbers[i_num - 1];
        --i_num;
      } else if (polish_expression_[i_str] == '-') {
        numbers[i_num - 2] = numbers[i_num - 2] - numbers[i_num - 1];
        --i_num;
      } else if (polish_expression_[i_str] == '*') {
        numbers[i_num - 2] = numbers[i_num - 2] * numbers[i_num - 1];
        --i_num;
      } else if (polish_expression_[i_str] == '/') {
        if (numbers[i_num - 1] == 0.0) {
          error_ = 6;  //  деление на 0
        } else {
          numbers[i_num - 2] = numbers[i_num - 2] / numbers[i_num - 1];
          --i_num;
        }
      } else if (polish_expression_[i_str] == 'm') {  //  mod
        numbers[i_num - 2] = fmod(numbers[i_num - 2], numbers[i_num - 1]);
        --i_num;
      } else if (polish_expression_[i_str] == '^') {
        numbers[i_num - 2] = pow(numbers[i_num - 2], numbers[i_num - 1]);
        --i_num;
      }
    } else {
      error_ = 5;  //  недостаточно чисел для оперции
    }
  }
}

//  Забираем число в double
double CalcModel::TakeNumber(int& i) {
  char char_number[255] = {'\0'};
  int cnt = 0;

  while (IsNumber(polish_expression_[i]) || polish_expression_[i] == '.' ||
         polish_expression_[i] == 'E') {
    char_number[cnt++] = polish_expression_[i++];
    if (polish_expression_[i - 1] == 'E' &&
        (polish_expression_[i] == '-' || polish_expression_[i] == '+')) {
      char_number[cnt++] = polish_expression_[i];
      ++i;
    }
  }
  char_number[cnt + 1] = '\0';
  double number = atof(char_number);

  return number;
}

//  Вычисляем матем.ф-ии
void CalcModel::DoMathFunc(double* numbers, int& i_str, int& i_num,
                           double& unar_minus) {
  if (polish_expression_[i_str] == 'l') {
    if (numbers[i_num - 1] <= 0) {
      error_ = 1;
    } else {
      numbers[i_num - 1] = unar_minus * log(numbers[i_num - 1]);
      unar_minus = 1;
    }
  } else if (polish_expression_[i_str] == 'L') {
    if (numbers[i_num - 1] <= 0) {
      error_ = 1;
    } else {
      numbers[i_num - 1] = unar_minus * log10(numbers[i_num - 1]);
      unar_minus = 1;
    }
  } else if (polish_expression_[i_str] == 's') {
    numbers[i_num - 1] = unar_minus * sin(numbers[i_num - 1]);
    unar_minus = 1;
  } else if (polish_expression_[i_str] == 'c') {
    numbers[i_num - 1] = unar_minus * cos(numbers[i_num - 1]);
    unar_minus = 1;
  } else if (polish_expression_[i_str] == 't') {
    if (fmod(numbers[i_num - 1], M_PI / 2) == 0) {
      error_ = 1;  //  tg Pi/2 и 3Pi/2 не существует
    } else {
      numbers[i_num - 1] = unar_minus * tan(numbers[i_num - 1]);
      unar_minus = 1;
    }
  } else if (polish_expression_[i_str] == 'S') {
    numbers[i_num - 1] = unar_minus * asin(numbers[i_num - 1]);
    unar_minus = 1;
  } else if (polish_expression_[i_str] == 'C') {
    numbers[i_num - 1] = unar_minus * acos(numbers[i_num - 1]);
    unar_minus = 1;
  } else if (polish_expression_[i_str] == 'T') {
    numbers[i_num - 1] = unar_minus * atan(numbers[i_num - 1]);
    unar_minus = 1;
  } else if (polish_expression_[i_str] == 'Q') {
    if (numbers[i_num - 1] < 0) {
      error_ = 1;
    } else {
      numbers[i_num - 1] = unar_minus * sqrt(numbers[i_num - 1]);
      unar_minus = 1;
    }
  }
}

//  ======================================== CALCULATE DATA FOR GRAPH
//  ======================================================================

//  Вычисляем данные для графика
void CalcModel::CalculateDataGraph(double x_min, double x_max, double y_min,
                                   double y_max) {
  if (!error_) {  //  увеличиваем память векторов:
    size_t size = abs((x_max - x_min) / step_);
    data_x_.reserve(size);
    data_y_.reserve(size);

    for (double x_i = x_min, y_i = 0; x_i <= x_max; x_i += step_) {
      x_ = x_i;
      CalculateExpression();
      y_i = result_;

      if (!error_ && (y_i >= y_min && y_i <= y_max)) {
        data_x_.push_back(x_i);
        data_y_.push_back(y_i);
      }
    }
  }
}

//  ======================================== GETTERS AND SETTERS
//  ======================================================================

void CalcModel::SetX(const double& new_x) noexcept {
  x_ = new_x;
  result_ = 0;
}

void CalcModel::SetExpression(const std::string& new_expression) {
  expression_ = new_expression;
  polish_expression_.clear();
  polish_expression_.reserve(expression_.size());
}

const std::string& CalcModel::GetExpression() noexcept { return expression_; }

const std::string& CalcModel::GetPolishExpression() noexcept {
  return polish_expression_;
}

int CalcModel::GetError() noexcept { return error_; }

double CalcModel::GetResult() noexcept { return result_; }

const std::vector<double>& CalcModel::GetDataX() noexcept { return data_x_; }

const std::vector<double>& CalcModel::GetDataY() noexcept { return data_y_; }

}  // namespace s21
