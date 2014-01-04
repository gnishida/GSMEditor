#include "ClipBoard.h"
#include "GraphUtil.h"

ClipBoard::ClipBoard() {
	isEmpty = false;
}

ClipBoard::~ClipBoard() {
}

void ClipBoard::clear() {
	roads.clear();

	isEmpty = true;
}

bool ClipBoard::empty() const {
	return isEmpty;
}

void ClipBoard::copy(RoadGraph* roads) {
	GraphUtil::copyRoads(roads, &this->roads);
}

void ClipBoard::copy(RoadGraph* roads, const AbstractArea& area) {
	GraphUtil::copyRoads(roads, &this->roads);

	GraphUtil::extractRoads(&this->roads, area, true);
	GraphUtil::clean(&this->roads);
}

RoadGraph* ClipBoard::paste() {
	return GraphUtil::copyRoads(&roads);
}
