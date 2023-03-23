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

	void Render() {
		olc::vi2d startNodePosition = startNode->GetPosition() + startNode->GetDisplayedSize() / 2;	// having trouble using float multiplication
		olc::vi2d endNodePosition = endNode->GetPosition() + endNode->GetDisplayedSize() / 2;
		olc::vi2d matrixNodePosition = matrixNode->GetPosition() + matrixNode->GetDisplayedSize() / 2;
		
		olc::Pixel color = matrixNode->GetColor();
		
		pge->DrawLine(startNodePosition, endNodePosition, color);
		pge->DrawLine(endNodePosition, matrixNodePosition, color, 0x44444444);
	}

	bool Contains(olc::vi2d point) const {
		// true if the point is closer than 5 pixels to the line
		olc::vi2d startNodePosition = startNode->GetPosition() + startNode->GetDisplayedSize() / 2;
		olc::vi2d endNodePosition = endNode->GetPosition() + endNode->GetDisplayedSize() / 2;
		olc::vi2d matrixNodePosition = matrixNode->GetPosition() + matrixNode->GetDisplayedSize() / 2;
		
		float distance = DistanceToLine(point, startNodePosition, endNodePosition);
		distance = std::min(distance, DistanceToLine(point, endNodePosition, matrixNodePosition));
	}

private:
	olc::PixelGameEngine* pge;
	
	TextBox* startNode;
	TextBox* endNode;
	TextBox* matrixNode;
};