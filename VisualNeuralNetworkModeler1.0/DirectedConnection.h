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
		
		//pge->DrawLine(startNodePosition, endNodePosition, matrixNode->GetColor());
		//DrawThickDirectedLine(startNodePosition, endNodePosition, matrixNode->GetColor(), 3);
		DrawBezierCurve(startNodePosition, olc::vf2d(-1, 0), endNodePosition, olc::vf2d(1, 0), matrixNode->GetColor(), 3);
		if ((point - closestPointToMouse).mag2() <= 64)
			//pge->DrawLine(closestPointToMouse, matrixNodePosition, matrixNode->GetColor(), 0x44444444);
			//DrawThickDirectedLine(matrixNodePosition, closestPointToMouse, matrixNode->GetColor(), 3);
			DrawBezierCurve(matrixNodePosition, olc::vf2d(0, -1), closestPointToMouse, olc::vf2d(0, -1), matrixNode->GetColor(), 3);
	}

private:
	olc::PixelGameEngine* pge;
	
	TextBox* startNode;
	TextBox* endNode;
	TextBox* matrixNode;

	/*void DrawThickDirectedLine(const olc::vi2d& p1, const olc::vi2d& p2, const olc::Pixel& color, uint8_t thickness) {
		olc::vf2d delta = p2 - p1;
		float length = delta.mag();
		if (length == 0) return;
		olc::vf2d normDelta = delta / length * thickness * 0.5f;
		olc::vf2d perpDelta = normDelta.perp();
		olc::vf2d shift = -normDelta * 0.866025403784f * 10;
		olc::vf2d p1m = p1 - perpDelta;
		olc::vf2d p2p = p2 + perpDelta + shift;
		olc::vf2d p2m = p2 - perpDelta + shift;
		pge->FillTriangle(p1 + perpDelta, p1m, p2p, color);
		pge->FillTriangle(p2m, p1m, p2p, color);

		olc::vf2d tri1 = p2 + perpDelta * 0.5f * 10;
		olc::vf2d tri2 = p2 - perpDelta * 0.5f * 10;
		pge->FillTriangle(p2, tri1 + shift, tri2 + shift, color);
	}*/

    void DrawBezierCurve(olc::vf2d p0, olc::vf2d normal0, olc::vf2d p3, olc::vf2d normal3, olc::Pixel color, uint8_t thickness = 1) {
		olc::vf2d p1 = p0 + 128 * normal0;
		olc::vf2d p2 = p3 - 128 * normal3;
        olc::Pixel underColor = color * 0.7f;
        olc::Pixel drawingColor;

        const olc::vf2d shift = olc::vf2d(0, thickness * 0.5f);
        olc::vf2d pastPointTop = p0 + shift;
        olc::vf2d pastPointBottom = p0 - shift;

        olc::vf2d centerPoint, newPointTop, newPointBottom;

        float t = 0;
        float e1, e2, e3, c0, c1, c2, c3;
        for (uint8_t i = 0; i < 10; ++i) {
            t += 0.1f;
            e1 = 3 * t;
            e2 = e1 * t;
            e3 = e2 * t;

            c0 = 1 - 0.3333f * e3 + e2 - e1;
            c1 = e3 - 2 * e2 + e1;
            c2 = e2 - e3;
            c3 = 0.3333f * e3;

            centerPoint = p0 * c0 + p1 * c1 + p2 * c2 + p3 * c3;
            newPointTop = centerPoint + shift;
            newPointBottom = centerPoint - shift;

            drawingColor = color;
            if (newPointTop.x < pastPointTop.x)
                drawingColor = underColor;

            pge->FillTriangle(pastPointTop, pastPointBottom, newPointTop, drawingColor);
            pge->FillTriangle(newPointBottom, pastPointBottom, newPointTop, drawingColor);

            pastPointTop = newPointTop;
            pastPointBottom = newPointBottom;
        }
    }

    /*void DrawAnimatedArrowBezierCurve(olc::vf2d p0, olc::vf2d p3, olc::Pixel color, uint8_t thickness = 1) {
        olc::vf2d p1 = p0 + olc::vf2d(128, 0);
        olc::vf2d p2 = p3 - olc::vf2d(128, 0);
        olc::Pixel underColor = color * 0.7f;
        olc::Pixel drawingColor;

        const olc::vf2d shift = olc::vf2d(0, thickness * 0.5f);
        olc::vf2d pastPointTop = p0 + shift;
        olc::vf2d pastPointBottom = p0 - shift;


        float t = 0;// animationTimer;
        float e1, e2, e3, c0, c1, c2, c3;
        olc::vf2d centerPoint;
        for (uint8_t i = 0; i < 96; ++i) {
            t += 0.01f;
            e1 = 3 * t;
            e2 = e1 * t;
            e3 = e2 * t;

            c0 = 1 - 0.3333f * e3 + e2 - e1;
            c1 = e3 - 2 * e2 + e1;
            c2 = e2 - e3;
            c3 = 0.3333f * e3;

            centerPoint = p0 * c0 + p1 * c1 + p2 * c2 + p3 * c3;

            drawingColor = color;
            if (centerPoint.x < pastPointTop.x)
                drawingColor = underColor;

            if ((i & 3) == 2) {
                FillTriangle(pastPointTop, pastPointBottom, centerPoint, drawingColor);
            }

            pastPointTop = centerPoint + shift;
            pastPointBottom = centerPoint - shift;
        }
    }*/
};