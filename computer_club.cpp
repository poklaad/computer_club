#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


struct Event {
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

std::vector<std::string> split_string(std::string& line, char del) { // Divedes the string into parts by delimiter
	std::vector<std::string> tokens;
	std::stringstream ss(line);
	std::string token;
	while (getline(ss, token, del)) {
		tokens.push_back(token);
	}
	return tokens;

}

bool check_data_symbols(std::string& data, const char* format) { // Cheks that the string is not empty and doesn't contatin unwanted symblos
	if (data.find_first_not_of(format) == std::string::npos && data.length() != 0)
		return true;
	return false;
}

bool check_time_format(std::string& time) { // Cheks if the string fits time format "XX:XX, 24-hour with ':' as a separator, unmeaningful nulls are required"
	if (time[2] == ':' && isdigit(time[0]) && isdigit(time[1]) && isdigit(time[3]) && isdigit(time[4]))
		return true;
	return false;
}

void check_init_data(std::ifstream& in, int& comp_numb, std::string& start_time, std::string& end_time, int& hour_price) { // Reading and checking first three lines from data file
	
	// Reading and checking the number of tables
	std::string n;
	getline(in, n);
	if (!(check_data_symbols(n, "0123456789"))) {
		throw n;
		return;
	}
	else {
		comp_numb = std::stoi(n);
	}

	// Reading and checking work time
	std::string time;
	getline(in, time);
	if (time[5] != ' ') {
		throw time;
		return;
	}

	// Separating the start and the end of the work time
	std::vector<std::string> tokens;
	tokens = split_string(time, ' ');

	// Checking the start of the work time
	start_time = tokens[0];
	if (!(check_data_symbols(start_time, ":0123456789") && check_time_format(start_time))) {
		throw time;
		return;
	}

	// Checking the end of the work time
	end_time = tokens[1];
	if (!(check_data_symbols(end_time, ":0123456789") && check_time_format(end_time))) {
		throw time;
		return;
	}
	
	// Reading and checking work time hour price in cimputer club
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

void check_event_format(std::string& line, Event& event) { // Checking the correctness of event strings
	
	// Splitting a string into event elements
	std::vector<std::string> tokens;
	tokens = split_string(line, ' ');

	if (tokens.size() < 3 || tokens.size() > 4) { // Checking the number of event elements
		throw line;
		return;
	}

	// Checking the time format
	if (!(check_data_symbols(tokens[0], ":0123456789") && check_time_format(tokens[0]))) {
		throw line;
		return;
	}
	event.time = tokens[0];
	
	// Checking the event id format
	if (!(check_data_symbols(tokens[1], "0123456789"))) {
		throw line;
		return;
	}
	event.id = std::stoi(tokens[1]);

	// Checking the format of the client name
	if (!(check_data_symbols(tokens[2], "abcdefghijklmnopqrstuvwxyz0123456789_-"))) {
		throw line;
		return;
	}
	event.client_name = tokens[2];

	// Checking the table number format (if present in the event)
	if (tokens.size() == 4) {
		if (!(check_data_symbols(tokens[3], "0123456789"))) {
			throw line;
			return;
		}
		event.table = std::stoi(tokens[3]);
	}
	return;
}

std::vector<std::string> event_process(std::vector<Event>& events, Table* tables, int& comp_numb) { // Events processing. Returns vector of strings related to events that will be printed as a result of the program
	std::vector<std::string> out_lines;
	return out_lines;
}

void output(std::vector <std::string>& events_lines, Table* tables, int& comp_numb, std::string& start_time, std::string& end_time, int& hour_price) { // Output of the work result to the console
	
	// The start of the work time
	std::cout << start_time << std::endl;

	// Results of events processing
	for (int i = 0; i < events_lines.size(); ++i) {
		std::cout << events_lines[i] << std::endl;
	}

	// The end of the work time
	std::cout << end_time << std::endl;
	
	// Output of profit and usage time of each table
	for (int i = 0; i < comp_numb; ++i) {
		std::cout << tables[i].id << " " << tables[i].profit << " " << tables[i].usage_time << std::endl;
	}
	// Result for the last table is output without endl
	std::cout << tables[comp_numb - 1].id << " " << tables[comp_numb - 1].profit << " " << tables[comp_numb - 1].usage_time;
	return;
}


// опнбепхрэ, врн бяе янашрхъ хдср онякеднбюрекэмн

int main(int argc, char *argv[]) {
	std::string FileName = "";
	if (argc == 2) {
		FileName = argv[1];
	}
	else {
		std::cout << ""; // No file entered for reading
		return 1;
	}
	try {

		// Attempt to open a file
		std::ifstream in;
		in.open(FileName);
		if (!in.is_open()) throw FileName;

		// Reading and checking the first three lines of the data file
		int comp_numb, hour_price;
		std::string start_time;
		std::string end_time;
		check_init_data(in, comp_numb, start_time, end_time, hour_price);
		
		// Reading and checking events
		std::string line;
		std::vector <Event> events;
		while (std::getline(in, line)) {
			Event event;
			check_event_format(line, event);
			events.push_back(event);
		}

		// Processing events
		std::vector <std::string> events_lines;
		Table* tables = new Table [comp_numb];
		events_lines = event_process(events, tables, comp_numb);

		// Output result
		output(events_lines, tables, comp_numb, start_time, end_time, hour_price);
	}
	catch (const std::string err) {
		std::cerr << err;
		return 2;
	}
	return 0;
}