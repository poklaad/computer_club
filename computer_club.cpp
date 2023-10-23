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

struct Table {
	int id;
	std::string usage_time;
	int profit = 0;
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

void check_init_data(std::ifstream& in, int& comp_numb, std::string& start_time, std::string& end_time, int& hour_price) { // Считывание и проверка корректности первых трех строк файла с данными
	
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
		hour_price = std::stoi(c);
	}

	return;
}

void check_event_format(std::string& line, Event& event) { // Проверка корректности строк с событиями
	
	// Разделение строки на элементы события
	std::vector<std::string> tokens;
	tokens = split_string(line, ' ');

	if (tokens.size() < 3 || tokens.size() > 4) { // Проверка количества элементов события
		throw line;
		return;
	}

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

std::vector<std::string> event_process(std::vector<Event>& events, Table* tables, int& comp_numb) { // Обработка событий. Возвращает вектор строк, относящихся к событиям, которые отправятся на вывод программы
	std::vector<std::string> out_lines;
	return out_lines;
}

void output(std::vector <std::string>& events_lines, Table* tables, int& comp_numb, std::string& start_time, std::string& end_time, int& hour_price) { // Вывод результата работы в консоль
	
	// Время начала работы
	std::cout << start_time << std::endl;

	// Результаты обработки событий
	for (int i = 0; i < events_lines.size(); ++i) {
		std::cout << events_lines[i] << std::endl;
	}

	// Время конца работы
	std::cout << end_time << std::endl;
	
	// Вывод выручки и времени занятости каждого стола
	for (int i = 0; i < comp_numb; ++i) {
		std::cout << tables[i].id << " " << tables[i].profit << " " << tables[i].usage_time << std::endl;
	}
	// Данные о последнем столе выводятся без перевода строки
	std::cout << tables[comp_numb - 1].id << " " << tables[comp_numb - 1].profit << " " << tables[comp_numb - 1].usage_time;
	return;
}


// ПРОВЕРИТЬ, ЧТО ВСЕ СОБЫТИЯ ИДУТ ПОСЛЕДОВАТЕЛЬНО

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
		int comp_numb, hour_price;
		std::string start_time;
		std::string end_time;
		check_init_data(in, comp_numb, start_time, end_time, hour_price);
		
		// Считывание, проверка и обработка событий
		std::string line;
		std::vector <Event> events;
		while (std::getline(in, line)) {
			Event event;
			check_event_format(line, event);
			events.push_back(event);
		}

		// Обработка событий
		std::vector <std::string> events_lines;
		Table* tables = new Table [comp_numb];
		events_lines = event_process(events, tables, comp_numb);

		// Вывод результата
		output(events_lines, tables, comp_numb, start_time, end_time, hour_price);
	}
	catch (const std::string err) {
		std::cerr << err;
		return 2;
	}
	return 0;
}