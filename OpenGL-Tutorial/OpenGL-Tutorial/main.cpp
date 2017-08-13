#include "Application.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	Application* app = new Application("OpenGL Application");

	app ->Run();

	delete app;

	return 0;
}