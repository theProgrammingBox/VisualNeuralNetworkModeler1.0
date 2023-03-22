#include "TextBox.h"

/*
TODO
1. work out lower panel
*/

class VisualNeuralNetworkModeler : public olc::PixelGameEngine {
public:
    VisualNeuralNetworkModeler() {
		sAppName = "Visual Neural Network Modeler";
    }

    bool OnUserCreate() override {
        pastMousePos = GetMousePos();
        
        /*TextBox upperPannel(this);
		upperPannel.SetTextHug(false);
		upperPannel.SetMovable(false);
		upperPannel.SetLabel("Component Name");
		upperPannel.SetSize(olc::vf2d(ScreenWidth(), 20));
		upperPannel.SetColor(olc::Pixel(30, 30, 30));
		upperPannel.SetPadding(olc::vf2d(4, 4));
		upperPannel.SetScale(4);
		upperPannel.SetPosition(olc::vf2d(20, 20));
		rectangles.push_back(upperPannel);*/
        return true;
    }

    void Render() {
        Clear(olc::Pixel(40, 40, 40));

        for (auto& rect : rectangles) {
            rect.Render();
        }

        if (IsTextEntryEnabled()) {
            if (textEntryGetCursor != TextEntryGetCursor()) {
                timer = 0.0f;
                textEntryGetCursor = TextEntryGetCursor();
            }
            if (timer < 0.5f) {
                float x = TextEntryGetCursor() * 8 + 3;
                FillRect(rectangles[selectedIndex].GetPosition() + olc::vf2d(x, 2), olc::vf2d(1, 12), olc::WHITE);
            }
        }

        /*FillRect(rectangles[selectedIndex].GetPosition() + olc::vf2d(x, 2), olc::vf2d(1, 12), olc::WHITE);
        FillRect(rectangles[selectedIndex].GetPosition() + olc::vf2d(x, 2), olc::vf2d(1, 12), olc::WHITE);*/
    }

    bool OnUserUpdate(float fElapsedTime) override {
        MouseHandle();
		TextHandle(fElapsedTime);
        Render();

        return true;
    }

private:
    std::vector<TextBox> rectangles;
    int32_t selectedIndex = -1;
    olc::vf2d pastMousePos;
    olc::vf2d mouseClickPos;
    float textEntryGetCursor;
    float timer;

    void MouseHandle() {
        if (GetMouse(0).bPressed) {
            selectedIndex = -1;
            if (!IsTextEntryEnabled()) {
                for (size_t i = rectangles.size(); i--;) {
                    if (rectangles[i].Contains(GetMousePos())) {
                        selectedIndex = static_cast<int32_t>(i);
                        mouseClickPos = GetMousePos();
                        break;
                    }
                }
            }
            else {
                TextEntryEnable(false);
            }
        }
        else if (GetMouse(0).bHeld && selectedIndex != -1) {
            olc::vf2d delta = GetMousePos() - pastMousePos;
            rectangles[selectedIndex].Move(delta);
        }
        else if (GetMouse(0).bReleased && mouseClickPos == GetMousePos()) {
            timer = 0.0f;
            TextEntryEnable(true);
            textEntryGetCursor = TextEntryGetCursor();
        }

        pastMousePos = GetMousePos();
    }

    void TextHandle(float fElapsedTime) {
        if (IsTextEntryEnabled())
        {
            timer += fElapsedTime;
            timer -= (timer >= 1.0f);
            rectangles[selectedIndex].SetLabel(TextEntryGetString());
        }

        if (GetMouse(1).bPressed) {
            if (!IsTextEntryEnabled()) {
                TextBox rect(this);
                rect.SetPosition(GetMousePos());
                rect.SetLabel("Default");
                rect.SetPadding(olc::vf2d(4, 4));
                rect.SetHSL((float)rand() / RAND_MAX, 0.75f, 0.5f);
                rectangles.emplace_back(rect);
            }
            else {
                TextEntryEnable(false);
            }
        }
    }
};

int main() {
    VisualNeuralNetworkModeler demo;
    if (demo.Construct(640, 360, 2, 2))
        demo.Start();

    return 0;
}