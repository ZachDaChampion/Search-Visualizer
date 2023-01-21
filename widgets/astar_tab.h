#ifndef ASTAR_TAB_H
#define ASTAR_TAB_H

#include <QWidget>

#include "sim_params.h"

class AStarTab : public QWidget {
  Q_OBJECT

  public:
  /**
   * Construct a new A* Tab.
   *
   * \param parent The parent widget.
   */
  AStarTab(QWidget* parent = nullptr);

  /**
   * Destroy an A* Tab.
   */
  ~AStarTab();

  private:

  /*
   * Widgets.
   */

  SimParams* simParams; // The simulation parameters.
};

#endif // ASTAR_TAB_H