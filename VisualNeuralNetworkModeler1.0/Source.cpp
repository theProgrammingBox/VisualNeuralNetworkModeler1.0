#include "TextBox.h"

/*
TODO
0. work out submit button
1. work out lower panel
*/

class VisualNeuralNetworkModeler : public olc::PixelGameEngine {
public:
    VisualNeuralNetworkModeler() {
		sAppName = "Visual Neural Network Modeler";
    }

    bool OnUserCreate() override {
        pastMousePos = GetMousePos();
        clickedIndex = -1;
        HoverIndex = -1;

		TextBox submitButton(this);
		submitButton.SetLabelable(false);
		submitButton.SetLabel("Submit");
		submitButton.SetMovable(false);
		submitButton.SetPosition(olc::vf2d(ScreenWidth() - 112, 0));
		submitButton.SetScale(2);
		submitButton.SetPadding(olc::vf2d(4, 4));
		submitButton.SetColor(olc::Pixel(30, 30, 30));
		textBoxes.push_back(submitButton);

        TextBox upperPannel(this);
        upperPannel.SetTextHug(false);
        upperPannel.SetLabel("Component Name");
        upperPannel.SetMovable(false);
        upperPannel.SetScale(2);
        upperPannel.SetPadding(olc::vf2d(4, 4));
        upperPannel.SetBaseSize(olc::vf2d((ScreenWidth() - 128) * 0.5f, 8));
        upperPannel.SetColor(olc::Pixel(30, 30, 30));
        textBoxes.push_back(upperPannel);

        TextBox lowerPannel(this);
        lowerPannel.SetTextHug(false);
		lowerPannel.SetLabelable(false);
		lowerPannel.SetMovable(false);
		lowerPannel.SetPosition(olc::vf2d(0, ScreenHeight() - 32));
		lowerPannel.SetScale(2);
        lowerPannel.SetPadding(olc::vf2d(4, 4));
        lowerPannel.SetBaseSize(olc::vf2d(ScreenWidth() * 0.5f, 8));
		lowerPannel.SetGlowable(false);
		lowerPannel.SetColor(olc::Pixel(30, 30, 30));
		textBoxes.push_back(lowerPannel);
        
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        MouseHandle();
		TextHandle(fElapsedTime);
        Render();

        return true;
    }

private:
    std::vector<TextBox> textBoxes;
    int32_t clickedIndex;
    int32_t HoverIndex;
    olc::vf2d pastMousePos;
    olc::vf2d mouseClickPos;
    float pastTextEntryCursorPos;
    float blinkTimer;

    void MouseHandle() {
        if (HoverIndex != -1)
            textBoxes[HoverIndex].SetGlow(false);
        HoverIndex = -1;
        if (GetMouse(0).bPressed) {
            clickedIndex = -1;
            if (!IsTextEntryEnabled()) {
                for (int32_t i = textBoxes.size(); i--;) {
                    if (textBoxes[i].Contains(GetMousePos())) {
                        clickedIndex = i;
                        HoverIndex = i;
                        textBoxes[HoverIndex].SetGlow(true);
                        mouseClickPos = GetMousePos();
                        break;
                    }
                }
            }
            else {
                TextEntryEnable(false);
            }
        }
        else {
            for (int32_t i = textBoxes.size(); i--;) {
				if (textBoxes[i].Contains(GetMousePos()) && textBoxes[i].IsGlowable()) {
                    HoverIndex = i;
                    textBoxes[HoverIndex].SetGlow(true);
                    break;
                }
            }
            
            if (clickedIndex != -1) {
                if (GetMouse(0).bHeld && textBoxes[clickedIndex].IsMovable()) {
                    olc::vf2d delta = GetMousePos() - pastMousePos;
                    textBoxes[clickedIndex].Move(delta);
                }
                else if (GetMouse(0).bReleased && mouseClickPos == GetMousePos() && textBoxes[clickedIndex].IsLabelable()) {
                    blinkTimer = 0.0f;
                    TextEntryEnable(true);
                    pastTextEntryCursorPos = TextEntryGetCursor();
                }
            }
        }

        pastMousePos = GetMousePos();

        if (GetMouse(1).bPressed) {
            if (!IsTextEntryEnabled()) {
                TextBox textBox(this);
                textBox.SetPosition(GetMousePos());
                textBox.SetLabel("Default");
                textBox.SetPadding(olc::vf2d(4, 4));
                textBox.SetHSL((float)rand() / RAND_MAX);
                textBoxes.emplace_back(textBox);
            }
            else {
                TextEntryEnable(false);
            }
        }
    }

    void TextHandle(float fElapsedTime) {
        if (IsTextEntryEnabled())
        {
            blinkTimer += fElapsedTime;
            blinkTimer -= (blinkTimer >= 1.0f);
            textBoxes[clickedIndex].SetLabel(TextEntryGetString());
        }
    }

    void Render() {
        Clear(olc::Pixel(50, 50, 50));
        
        for (auto& textBox : textBoxes) {
            textBox.Render();
        }

        if (IsTextEntryEnabled()) {
            if (pastTextEntryCursorPos != TextEntryGetCursor()) {
                blinkTimer = 0.0f;
                pastTextEntryCursorPos = TextEntryGetCursor();
            }
            if (blinkTimer < 0.5f) {
                uint32_t scale = textBoxes[clickedIndex].GetScale();
                olc::vi2d x = (olc::vf2d(TextEntryGetCursor() * 8 - 1, 0) + textBoxes[clickedIndex].GetPadding()) * scale;
                FillRect(textBoxes[clickedIndex].GetPosition() + x, olc::vf2d(1, 8) * scale, olc::WHITE);
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