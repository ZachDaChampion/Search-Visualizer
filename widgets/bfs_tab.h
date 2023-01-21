#ifndef BFS_TAB_H
#define BFS_TAB_H

#include <QWidget>

#include "sim_params.h"

class BFSTab : public QWidget {
  Q_OBJECT

  public:

  /**
   * Construct a new BFS Tab.
   *
   * \param parent The parent widget.
   */
  BFSTab(QWidget* parent = nullptr);

  /**
   * Destroy a BFS Tab.
   */
  ~BFSTab();

  private:

  /*
   * Widgets.
   */

  SimParams* simParams; // The simulation parameters.
};

#endif // BFS_TAB_H