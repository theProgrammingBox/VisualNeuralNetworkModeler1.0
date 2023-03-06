#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"

void calculateRectangle(olc::vi2d pos1, olc::vi2d pos2, olc::vi2d& topLeft, olc::vi2d& size)
{
	topLeft.x = std::min(pos1.x, pos2.x);
	topLeft.y = std::min(pos1.y, pos2.y);
	size.x = std::abs(pos1.x - pos2.x);
	size.y = std::abs(pos1.y - pos2.y);
}

struct Node
{
	olc::vi2d position;
};

struct Connection
{
	olc::vi2d position1;
	olc::vi2d position2;
};

class VisualNeuralNetworkModeler : public olc::PixelGameEngine
{
public:
	olc::TransformedView view;
	std::vector<Node> nodes;
	std::vector<Connection> connections;
	olc::vi2d mouse;
	olc::vi2d prevMouse;
	
	VisualNeuralNetworkModeler()
	{
		sAppName = "Visual Neural Network Modeler";
	}
	
	bool OnUserCreate() override
	{
		view.Initialise({ ScreenWidth(), ScreenHeight() });
		mouse = view.ScreenToWorld(olc::vi2d(int(GetMouseX() * 0.1f + 0.5f) * 10, int(GetMouseY() * 0.1f + 0.5f) * 10));
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		view.HandlePanAndZoom();

		Clear(olc::BLACK);

		prevMouse = mouse;
		mouse = view.ScreenToWorld(olc::vi2d(int(GetMouseX() * 0.1f + 0.5f) * 10, int(GetMouseY() * 0.1f + 0.5f) * 10));
		Node node;
		node.position = mouse;
		nodes.push_back(node);

		for (auto& node : nodes)
			view.DrawCircle(node.position, 5, olc::RED);
		/*for (auto& line : lines)
		{
			view.DrawCircle(line.p1, 5, line.color);
			view.DrawLine(line.p1, line.p2, line.color);
			view.DrawCircle(line.p2, 5, line.color);
		}

		if (GetMouse(0).bPressed)
		{
			Line line;
			line.p1 = mouse;
			line.p2 = mouse;
			line.color = olc::RED;
			lines.push_back(line);
		}
		else if (lines.size() > 0 && GetMouse(0).bHeld)
		{
			lines.back().p2 = mouse;
		}*/

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