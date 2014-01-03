#pragma once

#include "RoadView.h"
#include "ui_RoadBox.h"
#include <QPushButton>

class MainWindow;

class RoadBox : public QWidget {
Q_OBJECT

public:
	Ui::RoadBox ui;
	MainWindow* mainWin;
	RoadView* view;
	QPushButton* pushButtonSelect;

public:
	RoadBox(MainWindow* mainWin, const char* filename, float roadSize, bool isLargeMap);
	~RoadBox();

public slots:
	void select();
};

