#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

#include <sys/ioctl.h>

#include "picojson.h"

int current_terminal_width;

// len("YYYY-MM-DD HH:MM:SS") = 19
int current_timestamp_width = 19;

void print_entry(const picojson::value& entry) {
	std::string timestamp = "";

	picojson::value data = entry;

	// Find and remove timestamp.
	if(entry.is<picojson::object>()) {
		picojson::object entry_o = entry.get<picojson::object>();
		const auto it = entry_o.find("timestamp");
		if(it != entry_o.end() && it->second.is<std::string>()) {
			timestamp = it->second.get<std::string>();
			data.get<picojson::object>().erase("timestamp");
		}
	}

	// Create one-line candidate of remaining JSON.
	const std::string remaining_1 = data.serialize();

	// Single-line or multi-line
	if(current_timestamp_width + remaining_1.size() <= current_terminal_width) {
		std::cout << timestamp << remaining_1 << std::endl;
	} else {
		// TODO: fancy (color + indent) output.
		std::cout <<
			timestamp << std::endl <<
			data.serialize() << std::endl;
	}
}

void print_entry(const std::string& entry) {
	std::cout << "TEXT " << entry << std::endl;	
}

int main(int argc, char** argv) {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	current_terminal_width = w.ws_col;

	std::vector<std::unique_ptr<std::ofstream>> outputs;
	for(int i = 1; i < argc; i++) {
		outputs.push_back(
			std::unique_ptr<std::ofstream>(
				new std::ofstream(argv[i])));
	}

	while(!std::cin.eof()) {
		std::string line;
		std::getline(std::cin, line);

		// Pass-through everything.
		for(auto& output : outputs) {
			*output << line << std::endl;
		}

		// Ignore empty line.
		if(line.empty()) {
			continue;
		}

		// Parse and pretty-print.
		picojson::value entry;
		std::string error;
		picojson::parse(entry, line.begin(), line.end(), &error);
		if(error.empty()) {
			print_entry(entry);
		} else {
			print_entry(line);
		}
	}
	return 0;
}
