#include "MainWindow.h"
#include "GraphUtil.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);

	statusLebelL = new QLabel();
	statusLebelL->setStyleSheet("border: 1px solid grey");
	statusLabelR = new QLabel();
	statusLabelR->setFixedWidth(160);
	statusLabelR->setStyleSheet("border: 1px solid grey");

	// setup UI
	ui.actionModeBasic->setChecked(true);
	ui.statusBar->addWidget(statusLebelL, 1);
	ui.statusBar->addWidget(statusLabelR, 0);

	// setup the docking widgets
	controlWidget = new ControlWidget(this);

	// set up the large road list widget
	std::vector<QString> largeRoads;
	largeRoads.push_back("osm\\3x3\\beijing.gsm");
	largeRoads.push_back("osm\\3x3\\canberra.gsm");
	largeRoads.push_back("osm\\3x3\\london.gsm");
	largeRoads.push_back("osm\\3x3\\madrid.gsm");
	largeRoads.push_back("osm\\3x3\\new-york.gsm");
	largeRoads.push_back("osm\\3x3\\paris.gsm");
	/*
	largeRoads.push_back("osm\\15x15\\beijing.gsm");
	largeRoads.push_back("osm\\15x15\\canberra.gsm");
	largeRoads.push_back("osm\\15x15\\london.gsm");
	*/
	dockLargeRoadBoxList = new QDockWidget(tr("Roads Database"), this);
	largeRoadBoxList = new RoadBoxList(this, largeRoads, 3000.0f, true);
	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	dockLargeRoadBoxList->setWidget(scrollArea);
	scrollArea->setWidget(largeRoadBoxList);
	dockLargeRoadBoxList->hide();

	// set up the small road list widget
	std::vector<QString> smallRoads;
	smallRoads.push_back("osm\\1x1\\beijing.gsm");
	smallRoads.push_back("osm\\1x1\\canberra.gsm");
	smallRoads.push_back("osm\\1x1\\london.gsm");
	/*
	smallRoads.push_back("osm\\3x3\\beijing.gsm");
	smallRoads.push_back("osm\\3x3\\canberra.gsm");
	smallRoads.push_back("osm\\3x3\\london.gsm");
	*/
	dockSmallRoadBoxList = new QDockWidget(tr("Roads Database"), this);
	smallRoadBoxList = new RoadBoxList(this, smallRoads, 1000.0f, false);
	dockSmallRoadBoxList->setWidget(smallRoadBoxList);
	dockSmallRoadBoxList->hide();

	// register the menu's "AboutToShow" handlers
	connect(ui.menuMode, SIGNAL(aboutToShow()), this, SLOT(onMenuMode()));
	connect(ui.menuEdit, SIGNAL(aboutToShow()), this, SLOT(onMenuEdit()));

	// register the menu's action handlers
	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(onNew()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.actionOpenToAdd, SIGNAL(triggered()), this, SLOT(onOpenToAdd()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(onSave()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionModeBasic, SIGNAL(triggered()), this, SLOT(onModeBasic()));
	connect(ui.actionModeDatabase, SIGNAL(triggered()), this, SLOT(onModeDatabase()));
	connect(ui.actionModeSketch, SIGNAL(triggered()), this, SLOT(onModeSketch()));
	connect(ui.actionSelectAll, SIGNAL(triggered()), this, SLOT(onSelectAll()));
	connect(ui.actionUndo, SIGNAL(triggered()), this, SLOT(onUndo()));
	connect(ui.actionCut, SIGNAL(triggered()), this, SLOT(onCut()));
	connect(ui.actionCopy, SIGNAL(triggered()), this, SLOT(onCopy()));
	connect(ui.actionPaste, SIGNAL(triggered()), this, SLOT(onPaste()));
	connect(ui.actionDeleteEdge, SIGNAL(triggered()), this, SLOT(onDeleteEdge()));
	connect(ui.actionVoronoi, SIGNAL(triggered()), this, SLOT(onVoronoi()));
	connect(ui.actionShowArea, SIGNAL(triggered()), this, SLOT(onShowArea()));
	connect(ui.actionControlWidget, SIGNAL(triggered()), this, SLOT(onShowControlWidget()));
	connect(ui.actionLargeRoadDatabase, SIGNAL(triggered()), this, SLOT(onLargeRoadDatabase()));

	// setup the GL widget
	glWidget = new GLWidget(this);
	setCentralWidget(glWidget);

	// setup the event filter
	glWidget->installEventFilter(this);
	controlWidget->installEventFilter(this);
}

MainWindow::~MainWindow() {
}

void MainWindow::setStatusMessage(QString message1, QString message2) {
	statusLebelL->setText(message1);
	statusLabelR->setText(message2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers

void MainWindow::keyPressEvent(QKeyEvent* e) {
	glWidget->keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
	glWidget->keyReleaseEvent(e);
}

void MainWindow::onMenuMode() {
	ui.actionModeBasic->setChecked(glWidget->editor->mode == RoadGraphEditor::MODE_BASIC);
	ui.actionModeDatabase->setChecked(glWidget->editor->mode == RoadGraphEditor::MODE_DATABASE);
	ui.actionModeSketch->setChecked(glWidget->editor->mode == RoadGraphEditor::MODE_SKETCH);
}

void MainWindow::onMenuEdit() {
	ui.actionUndo->setDisabled(glWidget->editor->history.empty());
	ui.actionCut->setEnabled(glWidget->editor->mode == RoadGraphEditor::MODE_BASIC_AREA_SELECTED);
	ui.actionCopy->setEnabled(glWidget->editor->mode == RoadGraphEditor::MODE_BASIC_AREA_SELECTED);
	ui.actionPaste->setDisabled(glWidget->editor->clipBoard.empty());
	ui.actionDeleteEdge->setDisabled(glWidget->editor->selectedEdge == NULL);
}

void MainWindow::onNew() {
	glWidget->editor->clear();
	glWidget->updateGL();
}

void MainWindow::onOpen() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open StreetMap file..."), "", tr("StreetMap Files (*.gsm)"));

	if (filename.isEmpty()) {
		printf("Unable to open file\n");
		return;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	glWidget->editor->openRoad(filename);
	glWidget->updateGL();
	QApplication::restoreOverrideCursor();
}

void MainWindow::onOpenToAdd() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open StreetMap file..."), "", tr("StreetMap Files (*.gsm)"));

	if (filename.isEmpty()) {
		printf("Unable to open file\n");
		return;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	glWidget->editor->openToAddRoad(filename);
	glWidget->showStatusMessage();
	glWidget->updateGL();
	QApplication::restoreOverrideCursor();
}

void MainWindow::onSave() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save StreetMap file..."), "", tr("StreetMap Files (*.gsm)"));

	if (filename.isEmpty()) {
		printf("Unable to open file\n");
		return;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	glWidget->editor->saveRoad(filename);
	QApplication::restoreOverrideCursor();
}

void MainWindow::onModeBasic() {
	glWidget->editor->mode = RoadGraphEditor::MODE_BASIC;

	ui.actionModeBasic->setChecked(true);
	ui.actionModeDatabase->setChecked(false);
	ui.actionModeSketch->setChecked(false);

	glWidget->showStatusMessage();
}

void MainWindow::onModeDatabase() {
	glWidget->editor->mode = RoadGraphEditor::MODE_DATABASE;

	ui.actionModeBasic->setChecked(false);
	ui.actionModeDatabase->setChecked(true);
	ui.actionModeSketch->setChecked(false);

	glWidget->showStatusMessage();
}

void MainWindow::onModeSketch() {
	glWidget->editor->mode = RoadGraphEditor::MODE_SKETCH;

	ui.actionModeBasic->setChecked(false);
	ui.actionModeDatabase->setChecked(false);
	ui.actionModeSketch->setChecked(true);

	glWidget->showStatusMessage();
}

void MainWindow::onSelectAll() {
	glWidget->editor->selectAll();
	glWidget->updateGL();
}

void MainWindow::onUndo() {
	glWidget->editor->undo();
	glWidget->updateGL();
}

void MainWindow::onCut() {
	glWidget->editor->cut();
	glWidget->updateGL();
}

void MainWindow::onCopy() {
	glWidget->editor->copy();
	glWidget->updateGL();
}

void MainWindow::onPaste() {
	glWidget->editor->paste();
	glWidget->updateGL();
}

void MainWindow::onDeleteEdge() {
	qDebug() << "onDeleteEdge";
	glWidget->editor->deleteEdge();
	glWidget->updateGL();
}

void MainWindow::onVoronoi() {
	glWidget->editor->voronoi();
}

void MainWindow::onVoronoiCut() {
	glWidget->editor->voronoiMerge2();
}

void MainWindow::onShowArea() {
	glWidget->showArea = true;
}

void MainWindow::onShowControlWidget() {
	controlWidget->show();
	addDockWidget(Qt::LeftDockWidgetArea, controlWidget);
}

void MainWindow::onLargeRoadDatabase() {
	dockSmallRoadBoxList->hide();

	dockLargeRoadBoxList->show();
	addDockWidget(Qt::RightDockWidgetArea, dockLargeRoadBoxList);
}
