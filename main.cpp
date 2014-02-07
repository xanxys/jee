#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "picojson.h"

int main(int argc, char** argv) {
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
		picojson::value v;
		std::string error;
		picojson::parse(v, line.begin(), line.end(), &error);
		if(error.empty()) {
			// JSON entry
			std::cout << "JSON " << line << std::endl;
		} else {
			// Text entry
			std::cout << "TEXT " << line << std::endl;
		}
	}
	return 0;
}
