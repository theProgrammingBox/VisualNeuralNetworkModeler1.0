#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"

class TextBox
{
public:
	olc::vi2d padding = { 2, 2 };
	olc::vi2d pos;
	std::string text;
	int width;
	olc::PixelGameEngine* pge;
	float scale;

	void Reset()
	{
		pos = { 0, 0 };
		text = "";
		width = 1;
		scale = 10;
	}

	void Render()
	{
		pge->FillRect(pos - padding * scale, olc::vi2d{ width, 8 } * scale + 2 * padding * scale, olc::DARK_GREY);
		pge->DrawString(pos, text, olc::WHITE, scale);
	}
};

class VisualNeuralNetworkModeler : public olc::PixelGameEngine
{
public:
	olc::TransformedView view;
	olc::vi2d mouse;
	TextBox textBox;
	std::vector<TextBox> textBoxes;
	
	VisualNeuralNetworkModeler()
	{
		sAppName = "Visual Neural Network Modeler Compiler";
	}
	
	bool OnUserCreate() override
	{
		view.Initialise({ ScreenWidth(), ScreenHeight() });
		mouse = view.ScreenToWorld(olc::vi2d(GetMouseX(), GetMouseY()));
		textBox.pge = this;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		view.HandlePanAndZoom();

		Clear(olc::BLACK);
		
		mouse = view.ScreenToWorld(olc::vi2d(int(GetMouseX() * 0.1f + 0.5f) * 10, int(GetMouseY() * 0.1f + 0.5f) * 10));
		
		view.DrawCircle(mouse, 5, olc::RED);

		if (GetMouse(0).bPressed)
		{
			textBox.Reset();
			textBox.pos = mouse;
			TextEntryEnable(true);
		}

		if (IsTextEntryEnabled())
		{
			textBox.width = TextEntryGetString().size() * 8;
			textBox.text = TextEntryGetString();
			textBox.Render();
		}
		else
		{
			DrawString(0, 8, "Click to enter text", olc::WHITE, 1);
		}

		for (auto& textBox : textBoxes)
		{
			textBox.Render();
		}
		
		return true;
	}

	void OnTextEntryComplete(const std::string& text) override
	{
		textBoxes.push_back(textBox);
	}
};

int main()
{
	VisualNeuralNetworkModeler visualNeuralNetworkModeler;
	if (visualNeuralNetworkModeler.Construct(1440, 810, 1, 1))
		visualNeuralNetworkModeler.Start();
	return 0;
}