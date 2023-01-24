#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
  // Create the graphics area.
  graphicsArea = new GraphicsArea(400, 400, this);

  // Create the tab widget.
  tabWidget = new QTabWidget(this);
  tabWidget->setUsesScrollButtons(false);

  // Create the tabs.
  editTab = new EditTab(this);
  bfsTab = new BFSTab(this);
  dfsTab = new DFSTab(this);
  astarTab = new AStarTab(this);
  dijkstraTab = new DijkstraTab(this);

  // Add the tabs to the tab widget.
  tabWidget->addTab(editTab, "Edit");
  tabWidget->addTab(bfsTab, "BFS");
  tabWidget->addTab(dfsTab, "DFS");
  tabWidget->addTab(astarTab, "A*");
  tabWidget->addTab(dijkstraTab, "Dijkstra");

  // Create the layout.
  layout = new QHBoxLayout(this);

  // Add the graphics area and tab widget to the layout.
  layout->addWidget(graphicsArea, 1);
  layout->addWidget(tabWidget, 0);

  // Set the layout.
  setLayout(layout);

  // Init graphics area.
  graphicsArea->initGrid(10, 10);
  graphicsArea->drawGrid();

  // Connect signals and slots.
  QObject::connect(editTab, &EditTab::setCostSelectedCells, graphicsArea,
      &GraphicsArea::setCostSelectedCells);
  QObject::connect(editTab, &EditTab::setStartCellSelected, graphicsArea,
      &GraphicsArea::setStartCellSelected);
  QObject::connect(editTab, &EditTab::setGoalCellSelected, graphicsArea,
      &GraphicsArea::setGoalCellSelected);
}

MainWindow::~MainWindow() { }
