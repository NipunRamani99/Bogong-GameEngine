
#include "../include/Engine.h"
#include <Windows.h>
int main()
{

	std::system("pause");
	bogong::Engine engine;
    engine.Start();
	do {
	   engine.Loop();
	} while (engine.KeepRendering);
	engine.End();
	return 0;
}
