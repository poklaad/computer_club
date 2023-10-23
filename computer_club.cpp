#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>


struct Event {
	std::string time;
	int id = 0;
	std::string client_name;
	int table = 0;
};

struct Table {
	int id = 0;
	std::string usage_time = "00:00";
	int profit = 0;
	bool is_busy = false;
	std::string start_using_time = "";
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

void check_init_data(std::ifstream& in, int& comp_count, std::string& start_time, std::string& end_time, int& hour_price) { // Reading and checking first three lines from data file
	
	// Reading and checking the number of tables
	std::string n;
	getline(in, n);
	if (!(check_data_symbols(n, "0123456789"))) {
		throw n;
		return;
	}
	else {
		comp_count = std::stoi(n);
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
	
	// Reading and checking work time hour price in computer club
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

void check_event_format(std::string& line, Event& event, int& comp_count) { // Checking the correctness of event strings
	
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
		// Table number must be in range [1; comp_count]
		if (event.table < 1 || event.table > comp_count) {
			throw line;
			return;
		}
	}
	return;
}

bool first_before_second(std::string time_1, std::string time_2) { // Compares time_1 and time_2 and returns true if time_1 <= time_2
	// turning strings of time into numbers of hours and minutes
	int hours_1 = std::stoi(time_1.substr(0, 2));
	int minutes_1 = std::stoi(time_1.substr(3, 2));
	int hours_2 = std::stoi(time_2.substr(0, 2));
	int minutes_2 = std::stoi(time_2.substr(3, 2));
	if (hours_1 > hours_2) return false;
	if (hours_1 == hours_2 && minutes_1 > minutes_2) return false;
	return true;
}

std::string sum_dif_time(std::string time_now, std::string time_before, std::string operation) { // Adds or subtracts time_now and time_before. Operation is equal to "plus" or "minus". 
																								 // Time_before was before time_now, so if time_before is greater than time_now, than 
																								 //  substraction should be performed with respect to midnight.

	// Turning strings of time into integers of hours and minutes
	int hours_now = std::stoi(time_now.substr(0, 2));
	int minutes_now = std::stoi(time_now.substr(3, 2));
	int hours_before = std::stoi(time_before.substr(0, 2));
	int minutes_before = std::stoi(time_before.substr(3, 2));
	int min;
	int h;

	// Calculating time
	if (operation == "plus") {
		min = minutes_now + minutes_before;
		h = (min >= 60) ? (hours_now + hours_before + 1) : (hours_now + hours_before);
		min %= 60;
	}
	else {
		if (first_before_second(time_before, time_now)) {

			// Substraction without respect to midnight
			min = minutes_now - minutes_before;
			h = (min < 0) ? (hours_now - hours_before - 1) : (hours_now - hours_before);
			min = (min < 0) ? (60 + min) : (min);
		}
		else { 
			
			// Substraction with respect to midnight
			min = minutes_now + (60 - minutes_before);
			h = (min >= 60) ? (hours_now + (23 - hours_before) + 1) : (hours_now + (23 - hours_before));
			min %= 60;
		}
	}

	// Turning integers of hours and minutes into strings of time
	std::string time = "";
	if (h < 10) {
		time += "0" + std::to_string(h);
	}
	else {
		time += std::to_string(h);
	}
	time += ":";
	if (min < 10) {
		time += "0" + std::to_string(min);
	}
	else {
		time += std::to_string(min);
	}
	return time;
}

std::vector<std::string> event_process(std::vector<Event>& events, Table* tables, int& comp_count, std::string& start_time, std::string& end_time, int& hour_price) { // Events processing. Returns vector of strings related to events that will be printed as a result of the program
	std::vector<std::string> out_lines; // lines to output
	std::map<std::string, int> clients_tables; // clients and their tables (if client doesn't have table, table's number = 0)
	for (int i = 0; i < events.size(); ++i) {
		// Checking that events are consequential
		if (i + 1 < events.size()) {
			if (!first_before_second(events[i].time, events[i + 1].time)) {
				throw events[i + 1];
				return out_lines;
			}
		}
		std::string out_line;
		switch (events[i].id) {
		case 1: { // Client has come

			// Creating first out line
			out_line = events[i].time + " " + std::to_string(events[i].id) + " " + events[i].client_name;

			// There should NOT be table number
			if (events[i].table != 0) {
				out_line = out_line + " " + std::to_string(events[i].table);
				out_lines.push_back(out_line);
				out_line = "";
				throw events[i];
				return out_lines;
			}

			out_lines.push_back(out_line);
			out_line = "";

			bool excep = false;

			// Check if client came not in business hours
			bool in_time;
			if (first_before_second(start_time, end_time)) { // working time doesn't include midnight
				if (first_before_second(start_time, events[i].time) && first_before_second(events[i].time, end_time))
					in_time = true;
				else
					in_time = false;
			}
			else { // working time includes midnight
				if (!(first_before_second(start_time, events[i].time) && first_before_second(events[i].time, end_time)))
					in_time = true;
				else
					in_time = false;
			}
			if (!in_time) { // If client came not in business hours
				out_line = events[i].time + " 13 NotOpenYet";
				out_lines.push_back(out_line);
				out_line = "";
				excep = true;
			}

			
			if (!excep) // Check if there has already been an exception
				if (clients_tables.count(events[i].client_name)) { // Check if the client has already come
					out_line = events[i].time + " 13 YouShallNotPass";
					out_lines.push_back(out_line);
					out_line = "";
					excep = true;
				} else {
					// If there is no exception, add client to the map of clients with no tabel (0)
					clients_tables.insert(std::make_pair(events[i].client_name, 0));
					excep = false;
			}

			out_line = "";
			break;
		}
		case 2: {

			// Creating first out line
			out_line = events[i].time + " " + std::to_string(events[i].id) + " " + events[i].client_name;

			// There should be table number
			if (events[i].table == 0) {
				out_lines.push_back(out_line);
				out_line = "";
				throw events[i];
				return out_lines;
			}
			else {
				out_line = out_line + " " + std::to_string(events[i].table);
				out_lines.push_back(out_line);
				out_line = "";
			}

			bool excep = false;
			int targ_table_num = events[i].table;

			if (!clients_tables.count(events[i].client_name)) { // Check if the client in the club
				out_line = events[i].time + " 13 ClientUnknown";
				out_lines.push_back(out_line);
				out_line = "";
				excep = true;
			}
			
			if (!excep)// Check if there has already been an exception
				if (tables[targ_table_num].is_busy) { // Check if the target table is free
					// Target table is busy
					out_line = events[i].time + " 13 PlaceIsBusy";
					out_lines.push_back(out_line);
					out_line = "";
					excep = true;

				}
				else {

					// Target table is free
					int cur_table_num = clients_tables[events[i].client_name];
					
					tables[cur_table_num].is_busy = false; // Current table set free
					tables[targ_table_num].is_busy = true; // Target table set busy
					
					// We don't count table with number 0
					if (cur_table_num != 0) {
						std::string this_client_time = sum_dif_time(events[i].time, tables[cur_table_num].start_using_time, "minus"); // Calculate time spent at the current table by this client
						tables[cur_table_num].usage_time = sum_dif_time(this_client_time, tables[cur_table_num].usage_time, "plus"); // Calculate time spent at the current table at all
						int hours = std::stoi(this_client_time.substr(0, 2));
						int minutes = std::stoi(this_client_time.substr(3, 2));
						if (minutes > 0) 
							++hours;
						tables[cur_table_num].profit = hours * hour_price;
					}

					tables[cur_table_num].start_using_time = ""; // Clear current table's using start time
					tables[targ_table_num].start_using_time = events[i].time; // Set target table's using start time
				}

			break;
		}
		case 3: {

			// Creating first out line
			out_line = events[i].time + " " + std::to_string(events[i].id) + " " + events[i].client_name;

			// There should NOT be table number
			if (events[i].table != 0) {
				out_line = out_line + " " + std::to_string(events[i].table);
				out_lines.push_back(out_line);
				out_line = "";
				throw events[i];
				return out_lines;
			}

			out_lines.push_back(out_line);
			out_line = "";

			bool excep = false;

			break;
		}
		case 4: {

			// Creating first out line
			out_line = events[i].time + " " + std::to_string(events[i].id) + " " + events[i].client_name;

			// There should NOT be table number
			if (events[i].table != 0) {
				out_line = out_line + " " + std::to_string(events[i].table);
				out_lines.push_back(out_line);
				out_line = "";
				throw events[i];
				return out_lines;
			}

			out_lines.push_back(out_line);
			out_line = "";

			bool excep = false;

			break;
		}
		default: { // There is no such id, throw an error

			// Creating out line
			out_line = events[i].time + " " + std::to_string(events[i].id) + " " + events[i].client_name;

			if (events[i].table != 0) {
				out_line = out_line + " " + std::to_string(events[i].table);
				out_lines.push_back(out_line);
				out_line = "";
				throw events[i];
				return out_lines;
			}
			break;
		}
		}
	}

	return out_lines;
}

void output(std::vector <std::string>& events_lines, Table* tables, int& comp_count, std::string& start_time, std::string& end_time) { // Output of the work result to the console
	
	// The start of the work time
	std::cout << start_time << std::endl;

	// Results of events processing
	for (int i = 0; i < events_lines.size(); ++i) {
		std::cout << events_lines[i] << std::endl;
	}

	// The end of the work time
	std::cout << end_time << std::endl;
	
	// Output of profit and usage time of each table
	for (int i = 1; i < comp_count; ++i) {
		std::cout << tables[i].id << " " << tables[i].profit << " " << tables[i].usage_time << std::endl;
	}
	// Output result for the last table without endl
	std::cout << tables[comp_count].id << " " << tables[comp_count].profit << " " << tables[comp_count].usage_time;
	return;
}


// ÏĞÎÂÅĞÈÒÜ, ×ÒÎ ÂÑÅ ÑÎÁÛÒÈß ÈÄÓÒ ÏÎÑËÅÄÎÂÀÒÅËÜÍÎ
// ÏĞÎÂÅĞÈÒÜ, ×ÒÎ Â ÊÎÍÖÅ ĞÀÁÎ×ÅÃÎ ÄÍß ÂÑÅ ÓØËÈ. ÂÑÅ ÓÕÎÄßÒ Â ÀËÔÀÂÈÒÍÎÌ ÏÎĞßÄÊÅ ÈÕ ÈÌÅÍ

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
		int comp_count, hour_price;
		std::string start_time;
		std::string end_time;
		check_init_data(in, comp_count, start_time, end_time, hour_price);
		
		// Reading and checking events
		std::string line;
		std::vector <Event> events;
		while (std::getline(in, line)) {
			Event event;
			check_event_format(line, event, comp_count);
			events.push_back(event);
		}

		// Processing events
		std::vector <std::string> events_lines;
		Table* tables = new Table [comp_count + 1]; // We don't use the table with number 0
		for (int i = 0; i < comp_count + 1; i++) {
			tables[i].id = i;
		}

		events_lines = event_process(events, tables, comp_count,start_time, end_time, hour_price);

		// Output result
		output(events_lines, tables, comp_count, start_time, end_time);
	}
	catch (const std::string err) {
		std::cerr << err;
		return 2;
	}
	catch (Event err) {
		std::cerr << err.time << " " << err.id << " " << err.client_name;
		if (err.table != 0) std::cerr << " " << err.table;
		return 3;
	}
	return 0;
}