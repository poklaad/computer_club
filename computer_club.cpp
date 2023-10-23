#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


struct Event { // Структура-событие
	std::string time;
	int id;
	std::string client_name;
	int table = 0;
};

std::vector<std::string> split_string(std::string& line, char del) { // Разделение строки на составные части по разделителю
	std::vector<std::string> tokens;
	std::stringstream ss(line);
	std::string token;
	while (getline(ss, token, del)) {
		tokens.push_back(token);
	}
	return tokens;

}

bool check_data_symbols(std::string& data, const char* format) { //Проверка, что строка не пустая и не содержит лишних символов
	if (data.find_first_not_of(format) == std::string::npos && data.length() != 0)
		return true;
	return false;
}

bool check_time_format(std::string& time) { // Проверка соответствия времени виду "24-часовой формат с двоеточием в качестве разделителя XX:XX, незначащие нули обязательны"
	if (time[2] == ':' && isdigit(time[0]) && isdigit(time[1]) && isdigit(time[3]) && isdigit(time[4]))
		return true;
	return false;
}

void check_init_data(std::ifstream& in, int& comp_numb, std::string& start_time, std::string& end_time, int& cost) { // Считывание и проверка корректности первых трех строк файла с данными
	
	// Считываение и проверка количества столов
	std::string n;
	getline(in, n);
	if (!(check_data_symbols(n, "0123456789"))) {
		throw n;
		return;
	}
	else {
		comp_numb = std::stoi(n);
	}

	// Считывание и проверка времени работы
	std::string time;
	getline(in, time);
	if (time[5] != ' ') {
		throw time;
		return;
	}

	// Разделение начала и конца времени работы
	std::vector<std::string> tokens;
	tokens = split_string(time, ' ');

	// Проверка начала времени работы
	start_time = tokens[0];
	if (!(check_data_symbols(start_time, ":0123456789") && check_time_format(start_time))) {
		throw time;
		return;
	}

	// Проверка конца времени работы
	end_time = tokens[1];
	if (!(check_data_symbols(end_time, ":0123456789") && check_time_format(end_time))) {
		throw time;
		return;
	}
	
	// Считывание и проверка стоимости часа в компьютерном клубе
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

void check_event_format(std::string& line) { // Проверка корректности строк с событиями
	
	// Разделение строки на элементы события
	std::vector<std::string> tokens;
	tokens = split_string(line, ' ');

	if (tokens.size() < 3 || tokens.size() > 4) { // Проверка количества элементов события
		throw line;
		return;
	}

	Event event;

	// Проверка формата времени
	if (!(check_data_symbols(tokens[0], ":0123456789") && check_time_format(tokens[0]))) {
		throw line;
		return;
	}
	event.time = tokens[0];
	
	// Проверка формата id события
	if (!(check_data_symbols(tokens[1], "0123456789"))) {
		throw line;
		return;
	}
	event.id = std::stoi(tokens[1]);

	// Проверка формата имени клиента
	if (!(check_data_symbols(tokens[2], "abcdefghijklmnopqrstuvwxyz0123456789_-"))) {
		throw line;
		return;
	}
	event.client_name = tokens[2];

	// Проверка формата номера стола (если есть в событии)
	if (tokens.size() == 4) {
		if (!(check_data_symbols(tokens[3], "0123456789"))) {
			throw line;
			return;
		}
		event.table = std::stoi(tokens[3]);
	}
	return;
}


// ПРОВЕРИТЬ, ЧТО ВСЕ СОБЫТИЯ ИДУТ ПОСЛЕДОВАТЕЛЬНО
// ПРОВЕРИТЬ, ЧТО ЭЛЕМЕНТЫ В СОБЫТИИ РАЗДЕЛЯЮТСЯ ПРОБЕЛАМИ

int main(int argc, char *argv[]) {
	std::string FileName = "";
	if (argc == 2) {
		FileName = argv[1];
	}
	else {
		std::cout << ""; // Не введен файл для чтения
		return 1;
	}
	try {

		// Попытка открыть файл
		std::ifstream in;
		in.open(FileName);
		if (!in.is_open()) throw FileName;

		// Считывание и проверка первых трех строк файла с данными
		int comp_numb, cost;
		std::string start_time;
		std::string end_time;
		check_init_data(in, comp_numb, start_time, end_time, cost);
		
		// Считывание, проверка и обработка событий
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