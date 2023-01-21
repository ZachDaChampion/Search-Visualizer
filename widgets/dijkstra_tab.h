#ifndef DIJKSTRA_TAB_H
#define DIJKSTRA_TAB_H

#include <QWidget>

class DijkstraTab : public QWidget {
  Q_OBJECT

  public:
  /**
   * Construct a new Dijkstra Tab.
   *
   * \param parent The parent widget.
   */
  DijkstraTab(QWidget* parent = nullptr);

  /**
   * Destroy a Dijkstra Tab.
   */
  ~DijkstraTab();
};

#endif // DIJKSTRA_TAB_H