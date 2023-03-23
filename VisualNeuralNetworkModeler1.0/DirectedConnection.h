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
		DrawThickDirectedLine(startNodePosition, endNodePosition, matrixNode->GetColor(), 3);
		if ((point - closestPointToMouse).mag2() <= 64)
			//pge->DrawLine(closestPointToMouse, matrixNodePosition, matrixNode->GetColor(), 0x44444444);
			DrawThickDirectedLine(matrixNodePosition, closestPointToMouse, matrixNode->GetColor(), 3);
	}

private:
	olc::PixelGameEngine* pge;
	
	TextBox* startNode;
	TextBox* endNode;
	TextBox* matrixNode;

	void DrawThickDirectedLine(const olc::vi2d& p1, const olc::vi2d& p2, const olc::Pixel& color, uint8_t thickness) {
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
	}
};