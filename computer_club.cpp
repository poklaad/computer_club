#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


struct Event { // ���������-�������
	std::string time;
	int id;
	std::string client_name;
	int table = 0;
};

std::vector<std::string> split_string(std::string& line, char del) { // ���������� ������ �� ��������� ����� �� �����������
	std::vector<std::string> tokens;
	std::stringstream ss(line);
	std::string token;
	while (getline(ss, token, del)) {
		tokens.push_back(token);
	}
	return tokens;

}

bool check_data_symbols(std::string& data, const char* format) { //��������, ��� ������ �� ������ � �� �������� ������ ��������
	if (data.find_first_not_of(format) == std::string::npos && data.length() != 0)
		return true;
	return false;
}

bool check_time_format(std::string& time) { // �������� ������������ ������� ���� "24-������� ������ � ���������� � �������� ����������� XX:XX, ���������� ���� �����������"
	if (time[2] == ':' && isdigit(time[0]) && isdigit(time[1]) && isdigit(time[3]) && isdigit(time[4]))
		return true;
	return false;
}

void check_init_data(std::ifstream& in, int& comp_numb, std::string& start_time, std::string& end_time, int& cost) { // ���������� � �������� ������������ ������ ���� ����� ����� � �������
	
	// ����������� � �������� ���������� ������
	std::string n;
	getline(in, n);
	if (!(check_data_symbols(n, "0123456789"))) {
		throw n;
		return;
	}
	else {
		comp_numb = std::stoi(n);
	}

	// ���������� � �������� ������� ������
	std::string time;
	getline(in, time);
	if (time[5] != ' ') {
		throw time;
		return;
	}

	// ���������� ������ � ����� ������� ������
	std::vector<std::string> tokens;
	tokens = split_string(time, ' ');

	// �������� ������ ������� ������
	start_time = tokens[0];
	if (!(check_data_symbols(start_time, ":0123456789") && check_time_format(start_time))) {
		throw time;
		return;
	}

	// �������� ����� ������� ������
	end_time = tokens[1];
	if (!(check_data_symbols(end_time, ":0123456789") && check_time_format(end_time))) {
		throw time;
		return;
	}
	
	// ���������� � �������� ��������� ���� � ������������ �����
	std::string c;
	getline(in, c);
	if (!(check_data_symbols(c, "0123456789"))) {
		throw c;
		return;
	}
	else {
		cost = std::stoi(c);
	}

	return;
}

void check_event_format(std::string& line) { // �������� ������������ ����� � ���������
	
	// ���������� ������ �� �������� �������
	std::vector<std::string> tokens;
	tokens = split_string(line, ' ');

	if (tokens.size() < 3 || tokens.size() > 4) { // �������� ���������� ��������� �������
		throw line;
		return;
	}

	Event event;

	// �������� ������� �������
	if (!(check_data_symbols(tokens[0], ":0123456789") && check_time_format(tokens[0]))) {
		throw line;
		return;
	}
	event.time = tokens[0];
	
	// �������� ������� id �������
	if (!(check_data_symbols(tokens[1], "0123456789"))) {
		throw line;
		return;
	}
	event.id = std::stoi(tokens[1]);

	// �������� ������� ����� �������
	if (!(check_data_symbols(tokens[2], "abcdefghijklmnopqrstuvwxyz0123456789_-"))) {
		throw line;
		return;
	}
	event.client_name = tokens[2];

	// �������� ������� ������ ����� (���� ���� � �������)
	if (tokens.size() == 4) {
		if (!(check_data_symbols(tokens[3], "0123456789"))) {
			throw line;
			return;
		}
		event.table = std::stoi(tokens[3]);
	}
	return;
}


// ���������, ��� ��� ������� ���� ���������������
// ���������, ��� �������� � ������� ����������� ���������

int main(int argc, char *argv[]) {
	std::string FileName = "";
	if (argc == 2) {
		FileName = argv[1];
	}
	else {
		std::cout << ""; // �� ������ ���� ��� ������
		return 1;
	}
	try {

		// ������� ������� ����
		std::ifstream in;
		in.open(FileName);
		if (!in.is_open()) throw FileName;

		// ���������� � �������� ������ ���� ����� ����� � �������
		int comp_numb, cost;
		std::string start_time;
		std::string end_time;
		check_init_data(in, comp_numb, start_time, end_time, cost);
		
		// ����������, �������� � ��������� �������
		std::string line;
		while (std::getline(in, line)) {
			check_event_format(line);
		}
	}
	catch (const std::string err) {
		std::cerr << err;
		return 2;
	}
	return 0;
}