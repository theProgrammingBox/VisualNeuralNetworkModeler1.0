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
        
        TextBox upperPannel(this);
		upperPannel.SetPosition(olc::vf2d(0, 0));
		upperPannel.SetLabel("Component Name");
		upperPannel.SetScale(2);
		upperPannel.SetBaseSize(olc::vf2d(ScreenWidth() / upperPannel.GetScale() - 16, 8));
		upperPannel.SetPadding(olc::vf2d(8, 8));
		upperPannel.SetColor(olc::Pixel(30, 30, 30));
		upperPannel.SetTextHug(false);
		upperPannel.SetMovable(false);
		rectangles.push_back(upperPannel);
        return true;
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
        else if (selectedIndex != -1) {
            if (GetMouse(0).bHeld && rectangles[selectedIndex].IsMovable()) {
                olc::vf2d delta = GetMousePos() - pastMousePos;
                rectangles[selectedIndex].Move(delta);
            }
            else if (GetMouse(0).bReleased && mouseClickPos == GetMousePos() && rectangles[selectedIndex].IsLabelable()) {
                timer = 0.0f;
                TextEntryEnable(true);
                textEntryGetCursor = TextEntryGetCursor();
            }
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
                uint32_t scale = rectangles[selectedIndex].GetScale();
                olc::vi2d x = (olc::vf2d(TextEntryGetCursor() * 8 - 1, 0) + rectangles[selectedIndex].GetPadding()) * scale;
                FillRect(rectangles[selectedIndex].GetPosition() + x, olc::vf2d(1, 8) * scale, olc::WHITE);
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