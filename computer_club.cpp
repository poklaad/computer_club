#include <iostream>
#include <fstream>
#include <string>

bool check_data_symbols(std::string& data, const char* format) {
	if (data.find_first_not_of(format) == std::string::npos && data.length() != 0)
		return true;
	return false;
}

bool check_time_format(std::string& time) {
	if (time[2] == ':' && isdigit(time[0]) && isdigit(time[1]) && isdigit(time[3]) && isdigit(time[4]))
		return true;
	return false;
}

void check_init_data(std::ifstream& in, int& comp_numb, std::string& start_time, std::string& end_time, int& cost) {
	std::string n;
	getline(in, n);
	
	if (!(check_data_symbols(n, "0123456789"))) {
		throw n;
		return;
	}
	else {
		comp_numb = std::stoi(n);
	}

	std::string time;
	getline(in, time);
	if (time[5] != ' ') {
		throw time;
		return;
	}

	start_time = time.substr(0, 5);
	if (!(check_data_symbols(start_time, ":0123456789") && check_time_format(start_time))) {
		throw time;
		return;
	}

	end_time = time.substr(time.size() - 5);
	if (!(check_data_symbols(end_time, ":0123456789") && check_time_format(end_time))) {
		throw time;
		return;
	}
	
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




// опнбепхрэ, врн бяе янашрхъ хдср онякеднбюрекэмн
// опнбепхрэ, врн щкелемрш б янашрхх пюгдекъчряъ опнаекюлх

int main(int argc, char *argv[]) {
	std::string FileName = "";
	if (argc == 2) {
		FileName = argv[1];
	}
	else {
		std::cout << "Wrong number of input arguments: " << argc;
		return 1;
	}
	try {
		std::ifstream in;
		in.open(FileName);
		in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		
		int comp_numb, cost;
		std::string start_time;
		std::string end_time;
		check_init_data(in, comp_numb, start_time, end_time, cost);
		

		
	}
	catch(const std::ifstream::failure& err) {
		std::cerr << err.what();
		return 2;
	}
	catch (const std::string err) {
		std::cerr << err;
		return 3;
	}



	return 0;
}