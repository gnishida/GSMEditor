#pragma once

#include <QGraphicsView>
#include "RoadGraph.h"
#include "BFSTree.h"
#include "RoadGraphDatabase.h"
#include "RoadGraphDatabaseResult.h"

class MainWindow;

class RoadView : public QGraphicsView {
public:
	static float LARGE_SIZE;

public:
	MainWindow* mainWin;
	float size;
	bool isLargeMap;
	QGraphicsScene* scene;

	RoadGraphDatabase db;
	//RoadGraph* orig_roads;
	RoadGraph* view_roads;
	//RoadGraph* generated_roads;
	QVector2D center;
	QList<RoadGraphDatabaseResultPtr> results;

	//QMap<RoadVertexDesc, BFSTree*> trees;

public:
	RoadView(MainWindow* parent, float size, bool isLargeMap);
	~RoadView();

	void load(const QString& filename);
	void showSimilarity(RoadGraph* roads);
	void updateView(RoadGraph* roads, bool showPairness = false);
};

