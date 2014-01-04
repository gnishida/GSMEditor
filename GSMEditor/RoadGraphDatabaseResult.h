#pragma once

#include "RoadGraph.h"
#include <QVector2D>
#include <boost/shared_ptr.hpp>

class RoadGraphDatabaseResult {
public:
	RoadGraph* roads;
	RoadGraph* orig_roads;
	float similarity;
	int type;
	QVector2D center;
	float rotation;
	QVector2D translation;

public:
	RoadGraphDatabaseResult(RoadGraph* roads, RoadGraph* orig_roads, float similarity, int type, const QVector2D& center, float rotation, const QVector2D& translation);
	~RoadGraphDatabaseResult();

	void generateMesh();
	void addMeshFromEdge(RenderablePtr renderable, RoadEdgePtr edge, const QColor& color, float height);

	RoadGraph* instantiateRoads();
};

typedef boost::shared_ptr<RoadGraphDatabaseResult> RoadGraphDatabaseResultPtr;