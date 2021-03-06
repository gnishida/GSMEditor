#pragma once

#include "RoadGraph.h"
#include "BFSTree.h"
#include "RoadGraphDatabaseResult.h"
#include <QMap>
#include <QList>

class RoadGraphDatabase {
public:
	static enum { TYPE_LARGE = 0, TYPE_SMALL };

public:
	int type;
	RoadGraph* roads;
	RoadGraph* roadsForSearch;
	QMap<RoadVertexDesc, BFSTree*> trees;

public:
	RoadGraphDatabase();
	~RoadGraphDatabase();

	void load(int type, const QString& filename);
	void findSimilarRoads(RoadGraph* roads1, int N, QList<RoadGraphDatabaseResultPtr>& results);

private:
	void createTrees(RoadGraph* roads);
	void clearTrees();
};

