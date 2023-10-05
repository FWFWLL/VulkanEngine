#include "Application.hpp"

#include <cstdlib>
#include <iostream>

int main() {
	try {
		FFL::Application app;

		app.run();
	} catch(const std::exception &exception) {
		std::cerr << exception.what() << std::endl;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

