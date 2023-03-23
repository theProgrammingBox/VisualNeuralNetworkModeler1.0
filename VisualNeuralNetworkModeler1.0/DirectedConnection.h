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
		
		pge->DrawLine(startNodePosition, endNodePosition, matrixNode->GetColor());
		if ((point - closestPointToMouse).mag2() <= 16)
			pge->DrawLine(closestPointToMouse, matrixNodePosition, matrixNode->GetColor(), 0x55555555);
	}

	void Contains() {
	}

private:
	olc::PixelGameEngine* pge;
	
	TextBox* startNode;
	TextBox* endNode;
	TextBox* matrixNode;
};