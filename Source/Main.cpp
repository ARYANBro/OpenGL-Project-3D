#include "GameApplication.h"

int main()
{
	WindowProps windowProps = {
		.Width = 1280,
		.Height = 720,
		.Title = "3D Project"
	};

	GameApplication game(windowProps);
	game.Start();
}
