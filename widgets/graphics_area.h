#ifndef GRAPHICS_AREA_H
#define GRAPHICS_AREA_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>

#include <memory>
#include <vector>

#include "grid.h"

/**
 * The graphics area.
 */
class GraphicsArea : public QWidget {
  Q_OBJECT

  public slots:

  /**
   * Update the existing cells in the grid.
   */
  void updateCells();

  /**
   * Update the current interaction mode.
   *
   * \param editMode True if the edit mode is active, false otherwise.
   */
  void updateInteractionMode(bool editMode);

  signals:

  /**
   * Signal that the grid has been updated.
   */
  void gridUpdated();

  public:

  /**
   * Construct a new Graphics Area.
   *
   * \param parent The parent widget.
   */
  GraphicsArea(int minWidth, int minHeight, QWidget* parent = nullptr);

  /**
   * Destroy a Graphics Area.
   */
  ~GraphicsArea();

  /**
   * Initialize the new grid.
   * If a grid already exists, it will be dereferenced and a new one will be created.
   *
   * \param width The width of the grid.
   * \param height The height of the grid.
   */
  void initGrid(int width, int height);

  /**
   * Get the grid.
   *
   * \return The grid.
   */
  std::shared_ptr<Grid> getGrid() const;

  /**
   * Set size (in pixels) of each cell in the grid when displayed.
   *
   * \param size The size of each cell in the grid (in pixels).
   */
  void setCellDisplaySize(int size);

  /**
   * Get size (in pixels) of each cell in the grid when displayed.
   *
   * \return The size of each cell in the grid (in pixels).
   */
  int getCellDisplaySize() const;

  /**
   * (Re)draw the grid.
   * This will clear the scene and redraw the grid.
   */
  void drawGrid();

  private:

  /*
   * Data.
   */

  std::shared_ptr<Grid> grid = nullptr; // The grid.
  int cellDisplaySize = 0; // The size of each cell in the grid (in pixels).

  bool editMode = true; // True if the edit mode is active, false otherwise.
  std::vector<std::shared_ptr<Grid::Cell>> selected; // The selected cells.

  /*
   * Widgets.
   */

  QGraphicsView* graphicsView; // The graphics view holding the scene.
  QGraphicsScene* graphicsScene; // The graphics scene.

  /*
   * Graphics items.
   */

  struct CellGraphicsItem {
    QGraphicsRectItem* rect; // The rectangle representing the cell.
    QGraphicsTextItem* text; // The text to display inside the cell.
  };

  CellGraphicsItem* cellGraphicsItems
      = nullptr; // The graphics items representing the cells in the grid.
};

#endif // GRAPHICS_AREA_H