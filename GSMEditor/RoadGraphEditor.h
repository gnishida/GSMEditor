#pragma once

#include "RoadGraph.h"
#include "Sketch.h"
#include "BBox.h"
#include "ClipBoard.h"
#include "RoadGraphDatabase.h"
#include "VoronoiDiagram.h"
#include <boost/polygon/voronoi.hpp>

class MainWindow;

class RoadGraphEditor {
public:
	static enum {
		MODE_BASIC = 0,
		MODE_BASIC_VERTEX_SELECTED,
		MODE_BASIC_VERTEX_MOVING,
		MODE_BASIC_EDGE_SELECTED,
		MODE_BASIC_DEFINING_AREA,
		MODE_BASIC_AREA_SELECTED,
		MODE_BASIC_AREA_MOVING,
		MODE_BASIC_AREA_RESIZING_TL,
		MODE_BASIC_AREA_RESIZING_TR,
		MODE_BASIC_AREA_RESIZING_BL,
		MODE_BASIC_AREA_RESIZING_BR,
		MODE_BASIC_AREA_DISTORTING_TL,
		MODE_BASIC_AREA_DISTORTING_TR,
		MODE_BASIC_AREA_DISTORTING_BL,
		MODE_BASIC_AREA_DISTORTING_BR,

		MODE_DATABASE,
		MODE_DATABASE_SKETCHING,

		MODE_SKETCH,
		MODE_SKETCH_SKETCHING,
	};

public:
	MainWindow* mainWin;
	RoadGraph* roads;
	RoadGraph* roadsOrig;

	int mode;

	AbstractAreaPtr selectedArea;				// the selected area
	RoadVertexPtr selectedVertex;
	RoadVertexDesc selectedVertexDesc;
	RoadEdgePtr selectedEdge;
	RoadEdgeDesc selectedEdgeDesc;

	RoadGraph* selectedRoads;
	RoadGraph* selectedRoadsOrig;
	std::vector<RoadGraph*> interpolatedRoads;

	Sketch sketch;
	std::vector<RoadGraphDatabase*> largeRoadDB;
	std::vector<RoadGraphDatabase*> smallRoadDB;
	QList<RoadGraphDatabaseResultPtr> shadowRoads;

	VoronoiDiagram voronoiDiagram;

	ClipBoard clipBoard;

	std::vector<RoadGraph*> history;

public:
	RoadGraphEditor(MainWindow* mainWin);
	~RoadGraphEditor();

	void clear();
	void clearInterpolatedRoads();
	void openRoad(const QString& filename);
	void openToAddRoad(const QString& filename);
	void saveRoad(const QString& filename);
	void undo();
	void cut();
	void copy();
	void paste();
	bool deleteEdge();
	void simplify(float threshold);
	void reduce();
	void removeShortDeadend(float threshold);

	void selectAll();

	// MODE_BASIC DEFINING_AREA
	void startDefiningArea(const QVector2D& pt);
	void defineArea(const QVector2D& pt);
	void stopDefiningArea();

	// MODE_BASIC_AREA_DISTORTING
	void startDistortingArea(int type);
	void distortArea(const QVector2D& pt);
	void stopDistortingArea();

	// MODE_BASIC_AREA_MOVING
	void startMovingArea();
	void moveArea(float dx, float dy);
	void stopMovingArea();

	// MODE_BASIC_AREA_RESIZING_XX
	void startResizingArea(int type);
	void resizeArea(const QVector2D& pt);
	void stopResizingArea();

	bool selectVertex(const QVector2D& pt);
	bool selectEdge(const QVector2D& pt);

	// MODE_BASIC_VERTEX_SELECTED | MODE_BASIC_VERTEX_MOVING
	void startMovingVertex();
	void moveVertex(const QVector2D& pt);
	void moveVertex(const QVector2D& pt, float snap_threshold);
	void stopMovingVertex();
	void stopMovingVertex(float snap_threshold);

	void unselectRoads();
	void connectRoads();
	void interpolate();
	void showInterpolatedRoads(int ratio);
	void finalizeInterpolation(int ratio);
	bool splitEdge(const QVector2D& pt);

	// MODE_SKETCH | MODE_SKETCHING
	void startSketching(const QVector2D& pt, float snap_threshold);
	void sketching(const QVector2D& pt);
	void stopSketching(int type, int subtype, float simplify_threshold, float snap_threshold);
	void instantiateShadowRoads();
	void selectRoads(RoadGraph* new_roads);

	// Voronoi
	void voronoi();
	void voronoiMerge();
	void voronoiMerge2();
	void voronoiMerge2(const AbstractArea& area);
	void voronoiMerge3();

	// ICP
	void icp();

	// Simple connect
	void simpleConnect();
};

