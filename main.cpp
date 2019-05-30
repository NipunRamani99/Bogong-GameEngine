
#include "headers/Engine.h"
int main()
{		
	Engine engine;
    engine.Start();
	do {
	   engine.Loop();
	} while (engine.KeepRendering);
	engine.End();
	return 0;
}
