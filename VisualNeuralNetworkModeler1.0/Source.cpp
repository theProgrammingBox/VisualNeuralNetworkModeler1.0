#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class VisualNeuralNetworkModeler : public olc::PixelGameEngine
{
public:
	VisualNeuralNetworkModeler()
	{
		sAppName = "Visual Neural Network Modeler";
	}
	
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
		return true;
	}
};

int main()
{
	VisualNeuralNetworkModeler visualNeuralNetworkModeler;
	if (visualNeuralNetworkModeler.Construct(1440, 810, 1, 1))
		visualNeuralNetworkModeler.Start();
	return 0;
}