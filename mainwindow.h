#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHBoxLayout>
#include <QTabWidget>
#include <QWidget>

#include "widgets/astar_tab.h"
#include "widgets/bfs_tab.h"
#include "widgets/dfs_tab.h"
#include "widgets/dijkstra_tab.h"
#include "widgets/edit_tab.h"
#include "widgets/graphics_area.h"

/**
 * The main window of the application.
 */
class MainWindow : public QWidget {
  Q_OBJECT

  public:

  /**
   * Construct a new Main Window.
   *
   * \param parent The parent widget.
   */
  MainWindow(QWidget* parent = nullptr);

  /**
   * Destroy a Main Window.
   */
  ~MainWindow();

  private:

  /*
   * Widgets in the main window.
   */

  QHBoxLayout* layout; // The layout of the main window.
  GraphicsArea* graphicsArea; // The graphics area.
  QTabWidget* tabWidget; // The widget holding the tabs.

  /*
   * Widgets for each tab.
   */

  EditTab* editTab; // The edit tab.
  BFSTab* bfsTab; // The BFS tab.
  DFSTab* dfsTab; // The DFS tab.
  AStarTab* astarTab; // The A* tab.
  DijkstraTab* dijkstraTab; // The Dijkstra tab.
};

#endif // MAINWINDOW_H
