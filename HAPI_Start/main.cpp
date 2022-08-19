#include"Core/Visualisation.h"


#include<HAPI_lib.h>
using namespace HAPISPACE;

void HAPI_Main()
{
	srand((unsigned int)time(0));

	Visualisation game;
	game.Run();
}