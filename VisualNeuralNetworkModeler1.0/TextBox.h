#pragma once
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class TextBox {
public:
    TextBox(olc::PixelGameEngine* pge) {
        this->pge = pge;
        
        textHug = true;
        labelable = true;
        label = "";
        
        movable = true;
		position = olc::vi2d(0, 0);

        scale = 1;
		baseSize = olc::vi2d(0, 0);
		padding = olc::vi2d(0, 0);
		displayedSize = olc::vi2d(0, 0);

        glowable = true;
        glow = false;
		hue = 0;
		saturation = 0;
		lightness = 0;
		color = olc::BLANK;
    }

    void SetTextHug(bool textHug) {
        this->textHug = textHug;
    }

    void SetLabelable(bool labelable) {
        this->labelable = labelable;
    }

    void SetLabel(std::string label) {
        this->label = label;
		if (textHug)
            SetBaseSize(olc::vi2d(label.size() * 8, 8));
    }

    void SetMovable(bool movable) {
        this->movable = movable;
    }

    void SetPosition(olc::vi2d position) {
        this->position = position;
    }

	void SetScale(float scale) {
		this->scale = scale;
        UpdateDisplayedSize();
	}

    void SetBaseSize(olc::vi2d size) {
		this->baseSize = size;
        UpdateDisplayedSize();
    }

    void SetPadding(olc::vi2d padding) {
        this->padding = padding;
        UpdateDisplayedSize();
    }

    void UpdateDisplayedSize() {
        displayedSize = (baseSize + padding * 2) * scale;
    }

    void SetGlowable(bool glowable) {
        this->glowable = glowable;
    }

    void SetGlow(bool glow) {
        this->glow = glow;
    }

    void SetHSL(float h, float s = 0.65f, float l = 0.45f) {
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
        
		SetColor(olc::Pixel(static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255)));
    }

    void SetColor(olc::Pixel color) {
        this->color = color;
    }

    bool Contains(olc::vi2d point) const {
		return point.x >= position.x && point.x <= position.x + displayedSize.x && point.y >= position.y && point.y <= position.y + displayedSize.y;
    }

    void Move(olc::vi2d delta) {
        position += delta;
    }

    void Render() {
        olc::Pixel displayColor = color;
        if (glow) {
            uint8_t red = color.r * 0.7f + 255 * 0.3f;
            uint8_t green = color.g * 0.7f + 255 * 0.3f;
            uint8_t blue = color.b * 0.7f + 255 * 0.3f;
            displayColor = olc::Pixel(red, green, blue);
        }
        pge->FillRect(position, displayedSize, displayColor);
		pge->DrawString(position + padding * scale, label, olc::WHITE, scale);
    }

	bool IsLabelable() const {
		return labelable;
	}

	bool IsMovable() const {
		return movable;
	}

    olc::vi2d GetPosition() const {
        return position;
    }

	uint32_t GetScale() const {
		return scale;
	}

	olc::vi2d GetPadding() const {
		return padding;
	}

    olc::vi2d GetDisplayedSize() const {
        return displayedSize;
    }

    bool IsGlowable() const {
        return glowable;
    }

	olc::Pixel GetColor() const {
		return color;
	}

private:
    olc::PixelGameEngine* pge;
    
    bool textHug;
    bool labelable;
    std::string label;
    
	bool movable;
    olc::vi2d position;

	uint32_t scale;
    olc::vi2d baseSize;
    olc::vi2d padding;
	olc::vi2d displayedSize;
    
    bool glowable;
    bool glow;
    float hue;
    float saturation;
    float lightness;
    olc::Pixel color;
};