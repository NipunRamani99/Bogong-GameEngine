
#include "../include/Engine.h"

int main()
{
	bogong::Engine engine;
    engine.Start();
	do {
	   engine.Loop();
	} while (engine.KeepRendering);
	engine.End();
	return 0;
}
