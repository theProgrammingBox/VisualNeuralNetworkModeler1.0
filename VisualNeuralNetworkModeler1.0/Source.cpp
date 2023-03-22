#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class RectangleText {
public:
    olc::vf2d position;
    olc::vf2d size;
    olc::Pixel color;
    std::string label;

    RectangleText(olc::vf2d pos, olc::vf2d sz, const olc::Pixel& clr, const std::string& lbl)
        : position(pos), size(sz), color(clr), label(lbl) {}

    bool Contains(olc::vf2d point) const {
        return point.x >= position.x && point.x <= position.x + size.x &&
            point.y >= position.y && point.y <= position.y + size.y;
    }
};

class RectangleDetector : public olc::PixelGameEngine {
public:
    RectangleDetector() {
        sAppName = "RectangleText Detector";
    }

    bool OnUserCreate() override {
        rectangles.emplace_back(olc::vf2d(10, 10), olc::vf2d(100, 30), olc::RED, "Default");
        rectangles.emplace_back(olc::vf2d(10, 10), olc::vf2d(100, 30), olc::RED, "Default");
        rectangles.emplace_back(olc::vf2d(10, 10), olc::vf2d(100, 30), olc::RED, "Default");
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLACK);

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

        for (const auto& rect : rectangles) {
            FillRect(rect.position, rect.size, rect.color);
            DrawRect(rect.position, rect.size, olc::WHITE);
            DrawString(rect.position + olc::vf2d(5, 5), rect.label, olc::WHITE);
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
