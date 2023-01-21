#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
  // Create the graphics area.
  graphicsArea = new GraphicsArea(this);

  // Create the tab widget.
  tabWidget = new QTabWidget(this);

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

  // Create the layout.
  layout = new QHBoxLayout(this);

  // Add the graphics area and tab widget to the layout.
  layout->addWidget(graphicsArea, 1);
  layout->addWidget(tabWidget, 0);

  // Set the layout.
  setLayout(layout);
}

MainWindow::~MainWindow() { }
