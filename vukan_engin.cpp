#include "vengin_applic.hpp"

//stl
#include <stdexcept>
#include <iostream>

int main() {
	vengin::applic app;
	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return 1;
}