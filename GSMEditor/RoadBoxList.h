#pragma once

#include "RoadBox.h"
#include "ui_RoadBoxList.h"
#include <QVBoxLayout>

class MainWindow;

class RoadBoxList : public QWidget {
Q_OBJECT

public:
	Ui::RoadBoxList ui;
	MainWindow* mainWin;
	std::vector<RoadBox*> references;
	std::vector<RoadBox*> largeReferences;
	std::vector<RoadBox*> smallReferences;
	//std::vector<RoadGraph*> largeRoads;
	//std::vector<RoadGraph*> smallRoads;

public:
	RoadBoxList(MainWindow* mainWin, std::vector<QString> filelist, float roadSize, bool isLargeMap);
	~RoadBoxList();

public slots:

};

