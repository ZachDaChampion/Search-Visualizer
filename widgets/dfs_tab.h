#ifndef DFS_TAB_H
#define DFS_TAB_H

#include <QWidget>

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
};

#endif // DFS_TAB_H