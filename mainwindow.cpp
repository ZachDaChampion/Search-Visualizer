#include "mainwindow.h"

#include "game_loop.h"
#include "global_state.h"

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
  astarTab = new AStarTab(this);
  dijkstraTab = new DijkstraTab(this);

  // Add the tabs to the tab widget.
  tabWidget->addTab(editTab, "Edit");
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
  graphicsArea->initGrid(GlobalState::GRID_WIDTH, GlobalState::GRID_HEIGHT);
  graphicsArea->drawGrid();

  // Connect signals and slots.
  QObject::connect(editTab, &EditTab::setCostSelectedCells, graphicsArea,
      &GraphicsArea::setCostSelectedCells);
  QObject::connect(editTab, &EditTab::setStartCellSelected, graphicsArea,
      &GraphicsArea::setStartCellSelected);
  QObject::connect(editTab, &EditTab::setGoalCellSelected, graphicsArea,
      &GraphicsArea::setGoalCellSelected);
  QObject::connect(
      editTab, &EditTab::resetGrid, graphicsArea, &GraphicsArea::resetGrid);

  // Make sure game loop is initialized.
  GameLoop::singleton();
}

MainWindow::~MainWindow() { }
