#ifndef DFS_TAB_H
#define DFS_TAB_H

#include <QWidget>

#include "sim_params.h"

class DFSTab : public QWidget {
  Q_OBJECT

  public:
  /**
   * Construct a new DFS Tab.
   *
   * \param parent The parent widget.
   */
  DFSTab(QWidget* parent = nullptr);

  /**
   * Destroy a DFS Tab.
   */
  ~DFSTab();

  private:

  /*
   * Widgets.
   */

  SimParams* simParams; // The simulation parameters.
};

#endif // DFS_TAB_H