#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

/*
TODO
1. work out lower panel
*/

class RectangleText {
public:
    RectangleText(olc::PixelGameEngine* pge) {
		this->pge = pge;
		label = "";
		position = olc::vf2d(0, 0);
		size = olc::vf2d(0, 0);
		padding = olc::vf2d(0, 0);
		hue = 0.0f;
		saturation = 0.0f;
		lightness = 0.0f;
		color = olc::Pixel(0, 0, 0);
    }

	void SetLabel(std::string label) {
		this->label = label;
		SetSize(olc::vf2d(label.size() * 8, 8));
	}

	void SetPosition(olc::vf2d position) {
		this->position = position;
	}

	void SetSize(olc::vf2d size) {
        this->size = size;
		UpdateBoundingBox();
	}

	void SetPadding(olc::vf2d padding) {
		this->padding = padding;
		UpdateBoundingBox();
	}

    void UpdateBoundingBox() {
        size += padding * 2;
    }

	void SetHSL(float h, float s, float l) {
		this->hue = h;
		this->saturation = s;
		this->lightness = l;
        
        auto hue2rgb = [](float p, float q, float t) {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 0.166666667f) return p + (q - p) * 6 * t;
            if (t < 0.5f) return q;
            if (t < 0.666666667f) return p + (q - p) * (0.666666667f - t) * 6;
            return p;
        };

        if (s == 0) {
            uint8_t gray = static_cast<uint8_t>(l * 255.0f);
            color = olc::Pixel(gray, gray, gray);
        }

        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        float r = hue2rgb(p, q, h + 1.0 / 3);
        float g = hue2rgb(p, q, h);
        float b = hue2rgb(p, q, h - 1.0 / 3);

        color = olc::Pixel(static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255));
	}

    bool Contains(olc::vf2d point) const {
        return point.x >= position.x && point.x <= position.x + size.x &&
            point.y >= position.y && point.y <= position.y + size.y;
    }

	void Move(olc::vf2d delta) {
		position += delta;
	}

    void Render() {
        pge->FillRect(position, size, color);
        pge->DrawString(position + padding, label, olc::WHITE);
    }

	olc::vf2d GetPosition() const {
		return position;
	}

private:
    olc::PixelGameEngine* pge;
    std::string label;
    olc::vf2d position;
    olc::vf2d size;
    olc::vf2d padding;
    float hue;
    float saturation;
    float lightness;
    olc::Pixel color;
};

class RectangleDetector : public olc::PixelGameEngine {
public:
    RectangleDetector() {
        sAppName = "RectangleText Detector";
    }

    bool OnUserCreate() override {
        initialMousePos = GetMousePos();
        selectMousePos = GetMousePos();
        timer = 0;
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::Pixel(40, 40, 40));

        if (GetMouse(0).bPressed) {
            selectedIndex = -1;
            if (!IsTextEntryEnabled()) {
                std::cout << "Mouse pressed" << std::endl;
                for (size_t i = rectangles.size(); i--;) {
                    if (rectangles[i].Contains(initialMousePos)) {
                        selectedIndex = static_cast<int32_t>(i);
                        selectMousePos = GetMousePos();
                        break;
                    }
                }
            }
            else {
                TextEntryEnable(false);
            }
        }
        else if (GetMouse(0).bHeld && selectedIndex != -1) {
            olc::vf2d delta = GetMousePos() - initialMousePos;
			rectangles[selectedIndex].Move(delta);
        }
        else if (GetMouse(0).bReleased && selectMousePos == GetMousePos()) {
            timer = 0.0f;
            TextEntryEnable(true);
            textEntryGetCursor = TextEntryGetCursor();
        }

        if (IsTextEntryEnabled())
        {
            timer += fElapsedTime;
            timer -= (timer >= 1.0f);
			rectangles[selectedIndex].SetLabel(TextEntryGetString());
        }

        if (GetMouse(1).bPressed) {
			RectangleText rect(this);
			rect.SetPosition(GetMousePos());
			rect.SetLabel("Default");
			rect.SetPadding(olc::vf2d(4, 4));
            rect.SetHSL((float)rand() / RAND_MAX, 0.75f, 0.5f);
			rectangles.emplace_back(rect);
        }

        for (auto& rect : rectangles) {
            rect.Render();
        }

        if (IsTextEntryEnabled()) {
            if (textEntryGetCursor != TextEntryGetCursor()) {
                timer = 0.0f;
                textEntryGetCursor = TextEntryGetCursor();
            }
            if (timer < 0.5f) {
                float x = TextEntryGetCursor() * 8 + 4;
                FillRect(rectangles[selectedIndex].GetPosition() + olc::vf2d(x, 2), olc::vf2d(2, 12), olc::WHITE);
            }
        }

        initialMousePos = GetMousePos();

        return true;
    }

private:
    std::vector<RectangleText> rectangles;
    int32_t selectedIndex = -1;
    olc::vf2d initialMousePos;
    olc::vf2d selectMousePos;
    float textEntryGetCursor;
    float timer;
};

int main() {
    RectangleDetector demo;
    if (demo.Construct(640, 360, 2, 2))
        demo.Start();

    return 0;
}