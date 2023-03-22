#pragma once
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class TextBox {
public:
    TextBox(olc::PixelGameEngine* pge) {
        this->pge = pge;
        label = "";
        position = olc::vf2d(0, 0);
        size = olc::vf2d(8, 8);
        padding = olc::vf2d(0, 0);
        color = olc::Pixel(0, 0, 0);
        hue = 0.0f;
        saturation = 0.0f;
        lightness = 0.0f;
		scale = 1;
        textHug = true;
        movable = true;
        labelable = true;
    }

    void SetLabel(std::string label) {
        this->label = label;
		if (textHug)
            SetSize(olc::vf2d(label.size() * 8, 8));
    }

    void SetPosition(olc::vf2d position) {
        this->position = position;
    }

	void SetScale(float scale) {
		this->scale = scale;
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

	void SetColor(olc::Pixel color) {
		this->color = color;
	}

	void SetTextHug(bool textHug) {
		this->textHug = textHug;
	}
    
	void SetMovable(bool movable) {
		this->movable = movable;
	}

	void SetLabelable(bool labelable) {
		this->labelable = labelable;
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
		pge->DrawString(position + padding * scale, label, olc::WHITE, scale);
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
    olc::Pixel color;
	uint32_t scale;
    float hue;
    float saturation;
    float lightness;
    bool textHug;
	bool movable;
    bool labelable;
};