#pragma once
#include "TextBox.h"

class DirectedConnection {
public:

	DirectedConnection(olc::PixelGameEngine* pge) {
		this->pge = pge;
	}

	void SetStartNode(TextBox* startNode) {
		this->startNode = startNode;
	}

	void SetEndNode(TextBox* endNode) {
		this->endNode = endNode;
	}

	void SetMatrixNode(TextBox* matrixNode) {
		this->matrixNode = matrixNode;
	}

	void Render(olc::vi2d point) {
		olc::vi2d startNodePosition = startNode->GetPosition() + startNode->GetDisplayedSize() / 2;
		olc::vi2d endNodePosition = endNode->GetPosition() + endNode->GetDisplayedSize() / 2;
		olc::vi2d matrixNodePosition = matrixNode->GetPosition() + matrixNode->GetDisplayedSize() / 2;
		
		olc::vi2d delta = endNodePosition - startNodePosition;
		float squaredLength = delta.mag2();

		float t = (point.x - startNodePosition.x) * delta.x + (point.y - startNodePosition.y) * delta.y;
		
		olc::vi2d closestPointToMouse;
		if (t < 0) {
			closestPointToMouse = startNodePosition;
		}
		else if (t > squaredLength) {
			closestPointToMouse = endNodePosition;
		}
		else {
			closestPointToMouse = startNodePosition + delta * t / squaredLength;
		}
		
		DrawCubicBezierCurve(startNodePosition, olc::vf2d(-1, 0), endNodePosition, olc::vf2d(1, 0), matrixNode->GetColor(), 1);
		if ((point - closestPointToMouse).mag2() <= 64)
			DrawCubicBezierCurve(matrixNodePosition, olc::vf2d(0, -1), closestPointToMouse, olc::vf2d(0, 1), matrixNode->GetColor(), 1, 100);
	}

private:
	olc::PixelGameEngine* pge;
	
	TextBox* startNode;
	TextBox* endNode;
	TextBox* matrixNode;

	void DrawThickDirectedLine(const olc::vf2d& p1, const olc::vf2d& p2, const olc::Pixel& color, uint8_t radius = 1) {
		olc::vf2d delta = p2 - p1;
		float sqrLength = delta.mag2();
		if (sqrLength == 0) return;
		olc::vf2d perpDelta = delta.perp() / std::sqrt(sqrLength) * radius;
		olc::vf2d p1m = p1 - perpDelta;
		olc::vf2d p2p = p2 + perpDelta;
		pge->FillTriangle(p1 + perpDelta, p1m, p2p, color);
		pge->FillTriangle(p2 - perpDelta, p1m, p2p, color);
	}

	void DrawCubicBezierCurve(olc::vf2d p0, olc::vf2d normal0, olc::vf2d p3, olc::vf2d normal3, olc::Pixel color, uint8_t radius = 1, uint16_t segments = 10) {
		olc::vf2d p1 = p0 + 128 * normal0;
		olc::vf2d p2 = p3 + 128 * normal3;
		olc::vf2d pastPoint = p0;
        olc::vf2d newPoint;

        float t = 0;
		float step = 1.0f / segments;
        float e1, e2, e3, c0, c1, c2, c3;
        for (uint8_t i = 0; i < segments; ++i) {
            t += step;
            e1 = 3 * t;
            e2 = e1 * t;
            e3 = e2 * t;

            c0 = 1 - 0.3333f * e3 + e2 - e1;
            c1 = e3 - 2 * e2 + e1;
            c2 = e2 - e3;
            c3 = 0.3333f * e3;

            newPoint = p0 * c0 + p1 * c1 + p2 * c2 + p3 * c3;
			DrawThickDirectedLine(pastPoint, newPoint, color, radius);
			pastPoint = newPoint;
        }
    }
};