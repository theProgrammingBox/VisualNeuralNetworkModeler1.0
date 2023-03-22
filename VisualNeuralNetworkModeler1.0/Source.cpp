#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class RectangleText {
public:
	olc::PixelGameEngine* pge;
    olc::vf2d position;
    olc::vf2d size;
    olc::vf2d padding;
    olc::Pixel color;
    std::string label;
	float colorValue;

	RectangleText(olc::PixelGameEngine* pge, olc::vf2d pos = olc::vf2d(10, 10), olc::vf2d sz = olc::vf2d(54, 8), olc::vf2d pd = olc::vf2d(4, 4), olc::Pixel clr = olc::BLANK, const std::string& lbl = "Default", float clrVal = -1.0f)
		: position(pos), size(sz), padding(pd), color(clr), label(lbl), colorValue(clrVal), pge(pge) {
		if (clrVal == -1.0f) {
			if (color == olc::BLANK) {
				colorValue = rand() / static_cast<float>(RAND_MAX);
				color = GetColorFromValue(colorValue);
			}
		} else {
            color = GetColorFromValue(colorValue);
        }
		size += padding * 2;
    }

    bool Contains(olc::vf2d point) const {
        return point.x >= position.x && point.x <= position.x + size.x &&
            point.y >= position.y && point.y <= position.y + size.y;
    }

	void Render() {
		pge->FillRect(position, size, color);
		pge->DrawString(position + padding, label, olc::WHITE);
	}

private:
    olc::Pixel HSLtoRGB(float h, float s, float l) {
        auto hue2rgb = [](float p, float q, float t) {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 1.0 / 6) return p + (q - p) * 6 * t;
            if (t < 1.0 / 2) return q;
            if (t < 2.0 / 3) return p + (q - p) * (0.666666667f - t) * 6;
            return p;
        };

        if (s == 0) {
            uint8_t gray = static_cast<uint8_t>(l * 255.0f);
            return olc::Pixel(gray, gray, gray);
        }

        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        float r = hue2rgb(p, q, h + 1.0 / 3);
        float g = hue2rgb(p, q, h);
        float b = hue2rgb(p, q, h - 1.0 / 3);

        return olc::Pixel(static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255));
    }

    olc::Pixel GetColorFromValue(float value) {
        float hue = fmod(value, 1.0f);
        float saturation = 0.75f;
        float lightness = 0.5f;
        return HSLtoRGB(hue, saturation, lightness);
    }
};

class RectangleDetector : public olc::PixelGameEngine {
public:
    RectangleDetector() {
        sAppName = "RectangleText Detector";
    }

    bool OnUserCreate() override {
        initialMousePos = GetMousePos();
		selectMousePos = GetMousePos();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::Pixel(40, 40, 40));

        if (GetMouse(0).bPressed) {
			TextEntryEnable(false);
            selectedIndex = -1;
            for (size_t i = rectangles.size(); i--;) {
                if (rectangles[i].Contains(initialMousePos)) {
                    selectedIndex = static_cast<int32_t>(i);
					selectMousePos = GetMousePos();
                    break;
                }
            }
        } else if (GetMouse(0).bHeld && selectedIndex != -1) {
            olc::vf2d delta = GetMousePos() - initialMousePos;
            rectangles[selectedIndex].position += delta;
        } else if (GetMouse(0).bReleased && selectMousePos == GetMousePos()) {
            TextEntryEnable(true);
		}

        if (IsTextEntryEnabled())
        {
            rectangles[selectedIndex].size.x = (TextEntryGetString().size() + 1) * 8;
			rectangles[selectedIndex].label = TextEntryGetString();
        }
        
		if (GetMouse(1).bPressed) {
			rectangles.emplace_back(RectangleText(this, GetMousePos()));
		}
        
		for (auto& rect : rectangles) {
			rect.Render();
		}

        /*if (selectedIndex != -1) {
            DrawRect(rectangles[selectedIndex].position - olc::vf2d(1, 1),
                rectangles[selectedIndex].size + olc::vf2d(1, 1), olc::WHITE);
        }*/
        
        initialMousePos = GetMousePos();

        return true;
    }

private:
    std::vector<RectangleText> rectangles;
    int32_t selectedIndex = -1;
    olc::vf2d initialMousePos;
    olc::vf2d selectMousePos;
};

int main() {
    RectangleDetector demo;
    if (demo.Construct(640, 360, 2, 2))
        demo.Start();

    return 0;
}
