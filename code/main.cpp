#include "Engine.h"

int main()
{
	// Seed the random number generator
	srand(time(0));
	// Declare an instance of Engine
	Engine engine;
	// Start the engine
	engine.run();
	// Quit in the usual way when the engine is stopped
	return 0;
}