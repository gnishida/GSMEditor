#include "RoadBox.h"
#include "MainWindow.h"
#include "GraphUtil.h"
//#include "ExampleRoadGraph.h"
#include <qlayout.h>
#include <qpushbutton.h>

RoadBox::RoadBox(MainWindow* mainWin, const char* filename, float roadSize, bool isLargeMap) : QWidget((QWidget*)mainWin) {
	this->mainWin = mainWin;
	this->setMinimumSize(250, 250);

	QVBoxLayout* layout = new QVBoxLayout();
	this->setLayout(layout);

	view = new RoadView(mainWin, roadSize, isLargeMap);

	view->load(filename);
	pushButtonSelect = new QPushButton(tr("Select"));

	layout->addWidget(view);
	layout->addWidget(pushButtonSelect);

	// register the event handlers
	connect(pushButtonSelect, SIGNAL(clicked()), this, SLOT(select()));
}

RoadBox::~RoadBox() {
}

void RoadBox::select() {
	if (mainWin->glWidget->editor->selectedRoads) delete mainWin->glWidget->editor->selectedRoads;

	// copy the road from the database
	mainWin->glWidget->editor->selectRoads(view->results[0]->instantiateRoads());

	// clear the sketch
	mainWin->glWidget->editor->sketch.clear();

	mainWin->glWidget->updateGL();
}
