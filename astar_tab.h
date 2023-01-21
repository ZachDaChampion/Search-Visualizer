#ifndef ASTAR_TAB_H
#define ASTAR_TAB_H

#include <QWidget>

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
};

#endif // ASTAR_TAB_H