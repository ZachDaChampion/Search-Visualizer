#ifndef BFS_TAB_H
#define BFS_TAB_H

#include <QWidget>

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
};

#endif // BFS_TAB_H