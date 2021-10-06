#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string decToHex(unsigned  long int x) {																			//�������, ������������ ������, ���������� �������� � 16-������ �����
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
string outPrint(ifstream& input) {																				//�������, ������������ ������ ��� '\n', '\t' � ' '
	char ch;
	string str("");
	while (input.get(ch)) {
		if (ch != ' ' && ch != '\n' && ch != '\t') str = str + ch;
	}
	input.clear();
	input.seekg(0);
	return str;
}
string outPrint(string& input) {																				//�������, ������������ ������ ��� '\n', '\t' � ' '
	char ch;
	string str("");
	for (int i = 0; i != input.length(); i++) {
		ch = input[i];
		if (ch != ' ' && ch != '\n' && ch != '\t') str = str + ch;
	}
	return str;
}

unsigned long int numberOfbigEndianOct(unsigned long int x) {													//�������, ������������ ����� �������, ���������� ����� ��������������� ���������
	int b = ceil(0.125*log2(x+1));																				//��� Big Endian
	if (x+1 == 0 || x +1 == 1) b = 1;
	return b;
}

unsigned long int getSize(ifstream& input) {																	//�������, ������������ ����� �������� ��� ����� '\n', '\t' � ' '
	char ch;
	unsigned int size = 0;
	while (input.get(ch)) {
		if (ch != ' ' && ch != '\n' && ch != '\t') size++;
	}
	input.clear();
	input.seekg(0);
	return (size);
}

unsigned long int getSize(string input) {																		//�������, ������������ ����� �������� ��� ����� '\n', '\t' � ' '
	char ch;
	unsigned int size = 0;
	for (int i = 0; i != input.length(); i++) {
		ch = input[i];
		if (ch != ' ' && ch != '\n' && ch != '\t') size++;
	}
	return (size);
}


void checkInput(ifstream& input) {																				//��������� �������� �����
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
		cout << "\nError � " << e << " - ������������ ����:" << endl;
		if (e == 0) cout << "�� ������� ������� ����!\n" << endl;
		if (e == 1) cout << "� ������ ����� ���������� ������������ �������!\n" << endl;
		if (e == 2) cout << "���� ������ �������� �� ������� ����� �����������!\n" << endl;
		cout << "���������� �����������!\n" << endl;
		exit(0);
	}
}

void checkInput(string &input) {																				//��������� �������� �����			
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
		cout << "\nError � " << e << " - ������������ ����:\n" << endl;
		if (e == 1) cout << "� ������ ����� ���������� ������������ �������!\n" << endl;
		if (e == 2) cout << "���� ������ �������� �� ������� ����� �����������!\n" << endl;
		cout << "���������� �����������!\n" << endl;
		exit(0);
	}
}


int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	if (argc == 3) {																							//���������� ��� 3-� ���������� ��������� ������
		ifstream Input(argv[1]);
		ofstream Output(argv[2]);
		checkInput(Input);
		unsigned long int oSize = getSize(Input) / 2;											
		cout << "\n���������� ������� ��������!\n" << endl;
		cout <<"���������� �������������� ����: " << oSize << endl;
		if (oSize<128) {
			cout << "������ �����: Short Endian" << endl;
			Output << "0x04" << decToHex(oSize) << outPrint(Input) << endl;
		}
		else{
			cout << "������ �����: Big Endian" << endl;
			Output << "0x04" << decToHex(128 + numberOfbigEndianOct(oSize)) << decToHex(oSize) << outPrint(Input) << endl;
		}
		cout << "���� � �������������� ���������� ������� ��������!\n" << endl;
	}
	
	
	
	if (argc == 2 && (string)argv[1] == "-") {																	//���������� ��� 2-� ���������� ��������� ������ ("-")
		string Input;
		cout << "\n���������� ������� ��������!\n" << endl;
		cout << "������� �������� ������������������ � 16-������ ������� ���������(����������� ����� � ��������� ����� �������� ��������):\n";
		getline(cin, Input);
		cout << endl;
		checkInput(Input);
		unsigned long int oSize = getSize(Input) / 2;
		cout << "���������� �������������� ����: " << oSize << endl;
		if (oSize < 128) {
			cout << "������ �����: Short Endian" << endl;
			cout << "�������������� ����������: ";
			cout << "0x04" << decToHex(oSize) << outPrint(Input) << endl << endl;
		}
		else {
			cout << "������ �����: Big Endian" << endl;
			cout << "�������������� ����������: ";
			cout << "0x04" << decToHex(128 + numberOfbigEndianOct(oSize)) << decToHex(oSize) << outPrint(Input) << endl<< endl;
		}
	}

	if (argc == 1||argc>3||(argc == 2 && (string)argv[1] != "-")) {												//���������� ��� ��������� �������
		cout << "\n���������� �������� �����������!" << endl;
		cout << "������������� ��� ������ ������ ���������:\n" << endl;
		cout << "\t1) ����� ������ � �������:" << endl;
		cout << "\t���������� ������ ��������� � ��������� ������ ��� ���������: ���� � �����, ������� ��������� ������������,\n\t� ���� � �����, � ������� ��������� ��������� �������������� ����������." << endl;
		cout << "\t� ����� ����� ������ ���� �������� �������� ������������������ � 16-������ ������� ��������� (����������� \n\t����� � ��������� ����� �������� ��������)\n" << endl;
		cout << "\t2) ����� ������ � �������� ����� � ������:" << endl;
		cout << "\t���������� ������ ��������� � ��������� ������ �������� '-', ����� ���� ������������ ����� ���������� ������ \n\t� ���� ����� �������� ������������������ � 16-������ ������� ��������� (����������� ����� � ��������� ����� \n\t�������� ��������).\n" << endl;
	}

	cout << "���������� �����������!\n" << endl;
	system("pause");
	return 0;
}

