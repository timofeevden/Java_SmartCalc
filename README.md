
#SmartCalc v.3.0

	###��������� ����������� � ������� JavaFX �� ����� Java 1.8 � ������������ ������ �++ � ���� ������������ ����������.

	����������� ������������ �������� �������������� ��������, �������������� � ������������������ ������� (��������� ����������� � ��������):
		- ������, ��������, ���������, ���������, �������, ������� ���� � ������� �����
		- ���������� � �������, ���������� ������, ������� �� �������, ����������� � ���������� ���������
		- �����, �������, �������, ��������, ����������, ����������
	
	�� ���� ��������� ����� ���������� ��� ����� �����, ��� � ������������ �����, 
	���������� ����� ����� ��� �������, � � ���������������� ����� ������.
	������������� ���������� ������������ ��������� �������������� ��������� � ��������� ������� � ������������ �������� ���������� X � ���� �����.
	��� �� ����������� ����������� ������������� ��������� � ���� ������� �������������� ������� � ������� ������ "BuildGraph".
	���������� 10 ��������� ����������� �� ������� "History".
	
	
	��� ��������� ��������� ������������� ����� ��������� ���� Google Style Code.
		clang-format -style=google -i src/main/java/smartcalc/model/*.java src/main/java/smartcalc/view/*.java src/main/java/smartcalc/presenter/*.java src/main/resources/cpp/CalcModel/generated/*.cc src/test/java/smartcalc/model/*.java
		clang-format -style=google -n src/main/java/smartcalc/model/*.java src/main/java/smartcalc/view/*.java src/main/java/smartcalc/presenter/*.java src/main/resources/cpp/CalcModel/generated/*.cc src/test/java/smartcalc/model/*.java

##������� ������ ������ ���������:

	1) ��� �������� �++ ���������� ����� ������������� h-���� � ������������ native-������� �� java, ���������� ��������� �������:

		a) javac -h . ���-�����

		b) g++ -c -I"%JAVA_HOME\include" -I"%JAVA_HOME\include\win32" smartcalc_model_CalcModelImpl.cc -o polishCalcLib.o
			������: 
			g++ -c -I"D:\DEN\PROGRAMS\Java\jdk1.8.0_181\include" -I"D:\DEN\PROGRAMS\Java\jdk1.8.0_181\include\win32" smartcalc_model_CalcModelImpl.cc -o polishCalcLib.o

		c) g++ -shared -o polishCalcLib.dll polishCalcLib.o -Wl,--add-stdcall-alias

	2) ����� ���������� ����������� ���������� C++ polishCalcLib.dll � ���������� � ����������� ������ (� ������ mvn ������ � ���������� target), 
		��� ��� ������ ��������� ���������������� -Djava.library.path="..." �� �������� ���������� ����������.
		�� ��� ������ ��������� ���������� ����� "java -jar target\SmartCalcv3-1.0-SNAPSHOT-jar-with-dependencies.jar".

	3) � ������� Launch4j �������� �������� .exe � ��������� jar-����� � ������ ���������� �� ���������� resources.
		! �� �������� � ���������� � .exe-������ ����� ����������� ���� polishCalcLib.dll

	4) � ������� InnoSetup ��������� ������ ��� �������� �����������, ������� � �� ���������� srcPath �� ���������� ���� ���������� �������.

	5) ���������� ��������� � ������� ����������� �� ���������� installerOutput.



	

