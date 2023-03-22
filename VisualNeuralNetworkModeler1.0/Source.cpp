#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class RectangleText {
public:
	olc::PixelGameEngine* pge;
    olc::vf2d position;
    olc::vf2d size;
    olc::Pixel color;
    std::string label;
	float colorValue;

	RectangleText(olc::PixelGameEngine* pge, olc::vf2d pos = olc::vf2d(10, 10), olc::vf2d sz = olc::vf2d(100, 30), olc::Pixel clr = olc::BLANK, const std::string& lbl = "Default", float clrVal = -1.0f)
		: position(pos), size(sz), color(clr), label(lbl), pge(pge), colorValue(clrVal) {
		if (clrVal == -1.0f) {
			if (color == olc::BLANK) {
				colorValue = rand() / static_cast<float>(RAND_MAX);
				color = GetColorFromValue(colorValue);
			}
		} else {
            color = GetColorFromValue(colorValue);
        }
    }

    bool Contains(olc::vf2d point) const {
        return point.x >= position.x && point.x <= position.x + size.x &&
            point.y >= position.y && point.y <= position.y + size.y;
    }

	void Render() {
		pge->FillRect(position, size, color);
		pge->DrawRect(position, size, olc::WHITE);
		pge->DrawString(position + olc::vf2d(5, 5), label, olc::WHITE);
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
        float saturation = 0.8f;
        float lightness = 0.6f;
        return HSLtoRGB(hue, saturation, lightness);
    }
};

class RectangleDetector : public olc::PixelGameEngine {
public:
    RectangleDetector() {
        sAppName = "RectangleText Detector";
    }

    bool OnUserCreate() override {
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::Pixel(40, 40, 40));

        if (GetMouse(0).bPressed) {
            selectedIndex = -1;
            for (size_t i = rectangles.size(); i--;) {
                if (rectangles[i].Contains(GetMousePos())) {
                    selectedIndex = static_cast<int32_t>(i);
                    initialMousePos = GetMousePos();
                    break;
                }
            }
        }

        if (GetMouse(0).bHeld && selectedIndex != -1) {
            olc::vf2d delta = GetMousePos() - initialMousePos;
            rectangles[selectedIndex].position += delta;
            initialMousePos = GetMousePos();
        }
        
		if (GetMouse(1).bPressed) {
			rectangles.emplace_back(RectangleText(this, GetMousePos()));
		}
        
		for (auto& rect : rectangles) {
			rect.Render();
		}

        if (selectedIndex != -1) {
            DrawRect(rectangles[selectedIndex].position - olc::vf2d(2, 2),
                rectangles[selectedIndex].size + olc::vf2d(4, 4), olc::YELLOW);
        }

        return true;
    }

private:
    std::vector<RectangleText> rectangles;
    int32_t selectedIndex = -1;
    olc::vf2d initialMousePos;
};

int main() {
    RectangleDetector demo;
    if (demo.Construct(256, 240, 4, 4))
        demo.Start();

    return 0;
}
