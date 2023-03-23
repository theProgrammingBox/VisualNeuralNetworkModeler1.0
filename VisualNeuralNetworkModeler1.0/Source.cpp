#include "DirectedConnection.h"

/*
TODO
0. work out connections
*/

class VisualNeuralNetworkModeler : public olc::PixelGameEngine {
public:
    VisualNeuralNetworkModeler() {
		sAppName = "Visual Neural Network Modeler";
    }

    bool OnUserCreate() override {
        pastMousePos = GetMousePos();
        clickedTextBoxIndex = -1;
        textBoxHoverIndex = -1;

		TextBox* submitButton = new TextBox(this);
		submitButton->SetLabelable(false);
		submitButton->SetLabel("Submit");
		submitButton->SetMovable(false);
		submitButton->SetPosition(olc::vf2d(ScreenWidth() - 168, 0));
		submitButton->SetScale(3);
		submitButton->SetPadding(olc::vf2d(4, 4));
		submitButton->SetColor(olc::Pixel(30, 30, 30));
		textBoxes.push_back(submitButton);

        TextBox* upperPannel = new TextBox(this);
        upperPannel->SetTextHug(false);
        upperPannel->SetLabel("Component Name");
        upperPannel->SetMovable(false);
        upperPannel->SetScale(3);
        upperPannel->SetPadding(olc::vf2d(4, 4));
		upperPannel->SetBaseSize(olc::vf2d((ScreenWidth() - 190) * 0.33333333f, 8));
        upperPannel->SetColor(olc::Pixel(30, 30, 30));
        textBoxes.push_back(upperPannel);

        TextBox* lowerPannel = new TextBox(this);
        lowerPannel->SetTextHug(false);
        lowerPannel->SetLabelable(false);
        lowerPannel->SetMovable(false);
        lowerPannel->SetPosition(olc::vf2d(0, ScreenHeight() - 32));
        lowerPannel->SetScale(3);
        lowerPannel->SetPadding(olc::vf2d(4, 4));
        lowerPannel->SetBaseSize(olc::vf2d(ScreenWidth() * 0.5f, 8));
        lowerPannel->SetGlowable(false);
        lowerPannel->SetColor(olc::Pixel(30, 30, 30));
        textBoxes.push_back(lowerPannel);

        TextBox* defaultInputNode = new TextBox(this);
        defaultInputNode->SetTextHug(false);
        defaultInputNode->SetLabelable(false);
        defaultInputNode->SetMovable(false);
        defaultInputNode->SetPosition(olc::vf2d(16, 80));
        defaultInputNode->SetScale(2);
        defaultInputNode->SetBaseSize(olc::vf2d(8, 8));
        defaultInputNode->SetColor(olc::Pixel(30, 30, 30));
        textBoxes.push_back(defaultInputNode);

        TextBox* defaultOutputNode = new TextBox(this);
        defaultOutputNode->SetTextHug(false);
        defaultOutputNode->SetLabelable(false);
        defaultOutputNode->SetMovable(false);
		defaultOutputNode->SetPosition(olc::vf2d(ScreenWidth() - 32, 80));
        defaultOutputNode->SetScale(2);
        defaultOutputNode->SetBaseSize(olc::vf2d(8, 8));
        defaultOutputNode->SetColor(olc::Pixel(30, 30, 30));
        textBoxes.push_back(defaultOutputNode);
        
        TextBox* defaultInputMatrix = new TextBox(this);
		defaultInputMatrix->SetPosition(olc::vf2d(ScreenWidth() * 0.5f, ScreenHeight() * 0.5f));
        defaultInputMatrix->SetLabel("Input Matrix");
		defaultInputMatrix->SetScale(2);
        defaultInputMatrix->SetPadding(olc::vf2d(4, 4));
        defaultInputMatrix->SetHSL((float)rand() / RAND_MAX);
        textBoxes.emplace_back(defaultInputMatrix);

		DirectedConnection* defaultConnection = new DirectedConnection(this);
		defaultConnection->SetStartNode(textBoxes[3]);
		defaultConnection->SetEndNode(textBoxes[4]);
		defaultConnection->SetMatrixNode(textBoxes[5]);
		directedConnections.push_back(defaultConnection);
        
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        MouseHandle();
		TextHandle(fElapsedTime);
        Render();

        return true;
    }

private:
    std::vector<TextBox*> textBoxes;
    std::vector<DirectedConnection*> directedConnections;
    int32_t clickedTextBoxIndex;
    int32_t textBoxHoverIndex;
    int32_t directedConnectionHoverIndex;
    olc::vf2d pastMousePos;
    olc::vf2d mouseClickPos;
    float pastTextEntryCursorPos;
    float blinkTimer;

    void MouseHandle() {
        if (textBoxHoverIndex != -1)
            textBoxes[textBoxHoverIndex]->SetGlow(false);
        textBoxHoverIndex = -1;
        if (GetMouse(0).bPressed) {
            clickedTextBoxIndex = -1;
            if (!IsTextEntryEnabled()) {
                for (int32_t i = textBoxes.size(); i--;) {
                    if (textBoxes[i]->Contains(GetMousePos())) {
                        clickedTextBoxIndex = i;
                        textBoxHoverIndex = i;
                        textBoxes[textBoxHoverIndex]->SetGlow(true);
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
				if (textBoxes[i]->Contains(GetMousePos()) && textBoxes[i]->IsGlowable()) {
                    textBoxHoverIndex = i;
                    textBoxes[textBoxHoverIndex]->SetGlow(true);
                    break;
                }
            }
            
            if (clickedTextBoxIndex != -1) {
                if (GetMouse(0).bHeld && textBoxes[clickedTextBoxIndex]->IsMovable()) {
                    olc::vf2d delta = GetMousePos() - pastMousePos;
                    textBoxes[clickedTextBoxIndex]->Move(delta);
                }
                else if (GetMouse(0).bReleased && mouseClickPos == GetMousePos() && textBoxes[clickedTextBoxIndex]->IsLabelable()) {
                    blinkTimer = 0.0f;
                    TextEntryEnable(true);
                    pastTextEntryCursorPos = TextEntryGetCursor();
                }
            }
        }

        pastMousePos = GetMousePos();

        if (GetMouse(1).bPressed) {
            if (!IsTextEntryEnabled()) {
                TextBox* textBox = new TextBox(this);
                textBox->SetPosition(GetMousePos());
                textBox->SetLabel("Default");
				textBox->SetScale(2);
                textBox->SetPadding(olc::vf2d(4, 4));
                textBox->SetHSL((float)rand() / RAND_MAX);
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
            textBoxes[clickedTextBoxIndex]->SetLabel(TextEntryGetString());
        }
    }

    void Render() {
        Clear(olc::Pixel(50, 50, 50));

		for (auto& directedConnection : directedConnections) {
			directedConnection->Render(pastMousePos);
		}
        
        for (auto& textBox : textBoxes) {
            textBox->Render();
        }

        if (IsTextEntryEnabled()) {
            if (pastTextEntryCursorPos != TextEntryGetCursor()) {
                blinkTimer = 0.0f;
                pastTextEntryCursorPos = TextEntryGetCursor();
            }
            if (blinkTimer < 0.5f) {
                uint32_t scale = textBoxes[clickedTextBoxIndex]->GetScale();
                olc::vi2d x = (olc::vf2d(TextEntryGetCursor() * 8 - 1, 0) + textBoxes[clickedTextBoxIndex]->GetPadding()) * scale;
                FillRect(textBoxes[clickedTextBoxIndex]->GetPosition() + x, olc::vf2d(1, 8) * scale, olc::WHITE);
            }
        }
    }
};

int main() {
    VisualNeuralNetworkModeler demo;
    if (demo.Construct(1280, 720, 1, 1))
        demo.Start();

    return 0;
}