
#SmartCalc v.3.0

	###Программа разработана с помощью JavaFX на языке Java 1.8 с подключением модуля С++ в виде динамической библиотеки.

	Калькулятор поддерживает основные арифметические операции, математические и тригонометрические функции (аргументы вычисляются в радианах):
		- скобки, сложение, вычитание, умножение, деление, унарный плюс и унарный минус
		- возведение в степень, квадратный корень, остаток от деления, натуральный и десятичный логарифмы
		- синус, косинус, тангенс, арксинус, арккосинус, арктангенс
	
	На вход программы могут подаваться как целые числа, так и вещественные числа, 
	записанные через точку или запятую, и в экспоненциальной форме записи.
	Предусмотрено вычисление произвольных скобочных арифметических выражений в инфиксной нотации с подстановкой значения переменной X в виде числа.
	Так же реализовано графическое представление выражения в виде графика математической функции с помощью кнопки "BuildGraph".
	Предыдущие 10 выражений сохраняются во вкладке "History".
	
	
	При написании программы использовался стиль написания кода Google Style Code.
		clang-format -style=google -i src/main/java/smartcalc/model/*.java src/main/java/smartcalc/view/*.java src/main/java/smartcalc/presenter/*.java src/main/resources/cpp/CalcModel/generated/*.cc src/test/java/smartcalc/model/*.java
		clang-format -style=google -n src/main/java/smartcalc/model/*.java src/main/java/smartcalc/view/*.java src/main/java/smartcalc/presenter/*.java src/main/resources/cpp/CalcModel/generated/*.cc src/test/java/smartcalc/model/*.java

##Порядок полной сборки программы:

	1) Для создания С++ библиотеки нужно сгенерировать h-файл с определением native-методов из java, используем следующие команды:

		a) javac -h . ИМЯ-ФАЙЛА

		b) g++ -c -I"%JAVA_HOME\include" -I"%JAVA_HOME\include\win32" smartcalc_model_CalcModelImpl.cc -o polishCalcLib.o
			пример: 
			g++ -c -I"D:\DEN\PROGRAMS\Java\jdk1.8.0_181\include" -I"D:\DEN\PROGRAMS\Java\jdk1.8.0_181\include\win32" smartcalc_model_CalcModelImpl.cc -o polishCalcLib.o

		c) g++ -shared -o polishCalcLib.dll polishCalcLib.o -Wl,--add-stdcall-alias

	2) Далее необходимо скопировать библиотеку C++ polishCalcLib.dll в директорию с исполняемым файлом (в случае mvn сборки в директорию target), 
		так как внутри программы переопределяется -Djava.library.path="..." на корневую директорию приложения.
		Вы уже можете запустить приложение через "java -jar target\SmartCalcv3-1.0-SNAPSHOT-jar-with-dependencies.jar".

	3) С помощью Launch4j собираем исходник .exe с указанием jar-файла и иконки приложения из директории resources.
		! Не забудьте в директорию с .exe-файлом также скопировать файл polishCalcLib.dll

	4) С помощью InnoSetup запускаем скрипт для создания установщика, изменив в нём переменную srcPath на абсолютный путь директории проекта.

	5) Производим установку с помощью установщика из директории installerOutput.



	

