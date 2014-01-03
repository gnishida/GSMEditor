#include "RoadBoxList.h"
#include "MainWindow.h"

RoadBoxList::RoadBoxList(MainWindow* mainWin, std::vector<QString> filelist, float roadSize, bool isLargeMap) : QWidget((QWidget*)mainWin) {
	this->mainWin = mainWin;

	QVBoxLayout* layout = new QVBoxLayout();
	this->setLayout(layout);

	for (int i = 0; i < filelist.size(); i++) {
		RoadBox* box = new RoadBox(mainWin, filelist[i].toUtf8().data(), roadSize, isLargeMap);
		this->layout()->addWidget(box);
		references.push_back(box);
	}
}

RoadBoxList::~RoadBoxList() {
}
