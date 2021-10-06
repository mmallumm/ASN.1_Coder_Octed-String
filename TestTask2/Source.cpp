#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string decToHex(unsigned  long int x) {																			//Функция, возвращающая строку, содержащую аргумент в 16-ричной форме
	string hex("");
	do {
		int o = x % 16;
		if (o > 9) o += (int)'A' - 10;
		else o += (int)'0';
		hex = (char)o + hex;
		x /= 16;
	} while (x);
	if (hex.length() < 2) hex = '0' + hex;
	return hex;
}																													
string outPrint(ifstream& input) {																				//Функция, возвращающая строку без '\n', '\t' и ' '
	char ch;
	string str("");
	while (input.get(ch)) {
		if (ch != ' ' && ch != '\n' && ch != '\t') str = str + ch;
	}
	input.clear();
	input.seekg(0);
	return str;
}
string outPrint(string& input) {																				//Функция, возвращающая строку без '\n', '\t' и ' '
	char ch;
	string str("");
	for (int i = 0; i != input.length(); i++) {
		ch = input[i];
		if (ch != ' ' && ch != '\n' && ch != '\t') str = str + ch;
	}
	return str;
}

unsigned long int numberOfbigEndianOct(unsigned long int x) {													//Функция, возвращающая число октетов, содержащих длину закодированного сообщения
	int b = ceil(0.125*log2(x+1));																				//Для Big Endian
	if (x+1 == 0 || x +1 == 1) b = 1;
	return b;
}

unsigned long int getSize(ifstream& input) {																	//Функция, возвращающая число символов без учета '\n', '\t' и ' '
	char ch;
	unsigned int size = 0;
	while (input.get(ch)) {
		if (ch != ' ' && ch != '\n' && ch != '\t') size++;
	}
	input.clear();
	input.seekg(0);
	return (size);
}

unsigned long int getSize(string input) {																		//Функция, возвращающая число символов без учета '\n', '\t' и ' '
	char ch;
	unsigned int size = 0;
	for (int i = 0; i != input.length(); i++) {
		ch = input[i];
		if (ch != ' ' && ch != '\n' && ch != '\t') size++;
	}
	return (size);
}


void checkInput(ifstream& input) {																				//Процедура проверки ввода
	try {
		char ch;
		if (!input.is_open()) throw 0;
		while (input.get(ch)) {
			if (!((ch >= 48 && ch <= 57) || (ch >= 65 && ch <= 70) || (ch == ' ' || ch == '\n' || ch == '\t')))
				throw 1;
		}
		input.clear();
		input.seekg(0);
		if (getSize(input) % 2 != 0 || getSize(input) == 0) throw 2;
	}
	catch (int e) {
		cout << "\nError № " << e << " - Некорректынй ввод:" << endl;
		if (e == 0) cout << "Не удалось открыть файл!\n" << endl;
		if (e == 1) cout << "В потоке ввода обнаружены недопустимые символы!\n" << endl;
		if (e == 2) cout << "Ввод должен состоять из четного числа полуоктетов!\n" << endl;
		cout << "Приложение закрывается!\n" << endl;
		exit(0);
	}
}

void checkInput(string &input) {																				//Процедура проверки ввода			
	try {
		char ch;
		for (int i = 0; i != input.length(); i++) {
			ch = input[i];
			if (!((ch >= 48 && ch <= 57) || (ch >= 65 && ch <= 70) || (ch == ' ' || ch == '\n' || ch == '\t')))
				throw 1;
		}
		if (getSize(input) % 2 != 0 || getSize(input)==0) throw 2;
	}
	catch (int e) {
		cout << "\nError № " << e << " - Некорректынй ввод:\n" << endl;
		if (e == 1) cout << "В потоке ввода обнаружены недопустимые символы!\n" << endl;
		if (e == 2) cout << "Ввод должен состоять из четного числа полуоктетов!\n" << endl;
		cout << "Приложение закрывается!\n" << endl;
		exit(0);
	}
}


int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	if (argc == 3) {																							//Реализация при 3-х архументах командной строки
		ifstream Input(argv[1]);
		ofstream Output(argv[2]);
		checkInput(Input);
		unsigned long int oSize = getSize(Input) / 2;											
		cout << "\nПриложение успешно запущено!\n" << endl;
		cout <<"Количество закодированных байт: " << oSize << endl;
		if (oSize<128) {
			cout << "Формат длины: Short Endian" << endl;
			Output << "0x04" << decToHex(oSize) << outPrint(Input) << endl;
		}
		else{
			cout << "Формат длины: Big Endian" << endl;
			Output << "0x04" << decToHex(128 + numberOfbigEndianOct(oSize)) << decToHex(oSize) << outPrint(Input) << endl;
		}
		cout << "Файл с закодированным содержимым успешно сохранен!\n" << endl;
	}
	
	
	
	if (argc == 2 && (string)argv[1] == "-") {																	//Реализация при 2-х архументах командной строки ("-")
		string Input;
		cout << "\nПриложение успешно запущено!\n" << endl;
		cout << "Введите байтувую последовательность в 16-ричной системе счисления(принимаются цифры и латинские буквы верхнего регистра):\n";
		getline(cin, Input);
		cout << endl;
		checkInput(Input);
		unsigned long int oSize = getSize(Input) / 2;
		cout << "Количество закодированных байт: " << oSize << endl;
		if (oSize < 128) {
			cout << "Формат длины: Short Endian" << endl;
			cout << "Закодированное содержимое: ";
			cout << "0x04" << decToHex(oSize) << outPrint(Input) << endl << endl;
		}
		else {
			cout << "Формат длины: Big Endian" << endl;
			cout << "Закодированное содержимое: ";
			cout << "0x04" << decToHex(128 + numberOfbigEndianOct(oSize)) << decToHex(oSize) << outPrint(Input) << endl<< endl;
		}
	}

	if (argc == 1||argc>3||(argc == 2 && (string)argv[1] != "-")) {												//Реализация для остальных случаев
		cout << "\nПриложение запущено некорректно!" << endl;
		cout << "Предусмотрено два режима работы программы:\n" << endl;
		cout << "\t1) Режим работы с файлами:" << endl;
		cout << "\tПриложение должно принимать в командной строке два параметра: путь к файлу, который требуется закодировать,\n\tи путь к файлу, в который требуется сохранить закодированное содержимое." << endl;
		cout << "\tВ файле ввода должна быть записана байтовая последовательность в 16-ричной системе счисления (принимаются \n\tцифры и латинские буквы верхнего регистра)\n" << endl;
		cout << "\t2) Режим работы с потоками ввода и вывода:" << endl;
		cout << "\tПриложение должно принимать в командной строке параметр '-', после чего пользователю будет предложено ввести \n\tв поле ввода байтувую последовательность в 16-ричной системе счисления (принимаются цифры и латинские буквы \n\tверхнего регистра).\n" << endl;
	}

	cout << "Приложение закрывается!\n" << endl;
	system("pause");
	return 0;
}

