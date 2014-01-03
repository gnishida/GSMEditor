#include "RoadView.h"
#include "MainWindow.h"
#include "GraphUtil.h"
#include "BFSTree.h"
#include <qmap.h>
#include <QLineF>
#include <QGraphicsSimpleTextItem>

float RoadView::LARGE_SIZE = 3000.0f;

RoadView::RoadView(MainWindow* mainWin, float size, bool isLargeMap) : QGraphicsView((QWidget*)mainWin) {
	this->mainWin = mainWin;
	this->size = size;
	this->isLargeMap = isLargeMap;

	// set up the graphics view
	this->setSceneRect(0, 0, size, size);
	this->setFixedSize(220, 220);
	this->scale(200.0f / size, 200.0f / size);
	this->setBackgroundBrush(QColor(233, 229, 220));
	this->setRenderHint(QPainter::Antialiasing);

	// set up the scene
	scene = new QGraphicsScene();
	this->setScene(scene);	

	//orig_roads = new RoadGraph();
	view_roads = new RoadGraph();
	//generated_roads = new RoadGraph();
}

RoadView::~RoadView() {
	//delete orig_roads;
	delete view_roads;
	//delete generated_roads;

	//clearTrees();
}

/**
 * Load a road graph from the file.
 */
void RoadView::load(const QString& filename) {
	view_roads->clear();
	//orig_roads->clear();
	//generated_roads->clear();

	if (isLargeMap) {
		db.load(RoadGraphDatabase::TYPE_LARGE, filename);
	} else {
		db.load(RoadGraphDatabase::TYPE_SMALL, filename);
	}

	updateView(db.roadsForSearch);
}

/**
 * Compute the similarity between this road and the sketch (roads2).
 */
void RoadView::showSimilarity(RoadGraph* roads2) {
	// clear the results
	for (int i = 0; i < results.size(); i++) {
		delete results[i]->roads;
	}
	results.clear();

	db.findSimilarRoads(roads2, 1, results);
	
	if (results.size() > 0) {
		updateView(results[0]->roads, true);

		float similarity = results[0]->similarity;

		QString str;
		str.setNum(similarity);
		QGraphicsSimpleTextItem* score = scene->addSimpleText(str, QFont("Times", size * 0.1f));
		score->setPen(QPen(Qt::blue));
		score->setPos(0, 0);

		update();
	}
}

/**
 * Update the view based on the road graph with matching infromation.
 * If the edge has a corresponding one, color it with red. Otherwise, color itt with black.
 */
void RoadView::updateView(RoadGraph* roads, bool showPairness) {
	scene->clear();

	QPen pen(QColor(0, 0, 255));

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads->graph); ei != eend; ++ei) {
		if (!roads->graph[*ei]->valid) continue;

		if (size >= LARGE_SIZE) {
			if (roads->graph[*ei]->type == 1) continue;
		}

		for (int i = 0; i < roads->graph[*ei]->polyLine.size() - 1; i++) {
			QLineF line(roads->graph[*ei]->polyLine[i].x(), -roads->graph[*ei]->polyLine[i].y(), roads->graph[*ei]->polyLine[i+1].x(), -roads->graph[*ei]->polyLine[i+1].y());
			line.translate(size / 2.0f, size / 2.0f);
			QGraphicsLineItem* item = scene->addLine(line, pen);

			if (showPairness && !roads->graph[*ei]->fullyPaired) {
				item->setOpacity(0.1);
			}
		}
	}

	// Draw the square for the central vertex
	//RoadVertexDesc v1 = GraphUtil::getCentralVertex(roads);
	//scene->addRect(roads->graph[v1]->pt.x() + 4900, roads->graph[v1]->pt.y() + 4900, 200, 200, QPen(Qt::blue));

	scene->update();
}

