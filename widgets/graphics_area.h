#ifndef GRAPHICS_AREA_H
#define GRAPHICS_AREA_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QWidget>
#include <QMouseEvent>

#include <memory>
#include <vector>
#include <set>

#include "grid.h"

/**
 * The graphics area.
 */
class GraphicsArea : public QGraphicsView {
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
   * (Re)draw the grid.
   * This will clear the scene and redraw the grid.
   */
  void drawGrid();

  /**
   * Get the grid.
   *
   * \return The grid.
   */
  std::shared_ptr<Grid> getGrid() const;

  /*
   * Event handlers.
   */

  protected:

  void resizeEvent(QResizeEvent* event) override;

  void showEvent(QShowEvent* event) override;

  void mousePressEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

  private:

  /*
   * Graphics items.
   */

  struct CellGraphicsItem {
    QGraphicsRectItem* rect = nullptr; // The rectangle representing the cell.
    QGraphicsTextItem* text = nullptr; // The text to display inside the cell.
  };

  CellGraphicsItem* cellGraphicsItems
      = nullptr; // The graphics items representing the cells in the grid.

  /**
   * Update the graphics of a cell.
   * 
   * \param x The x coordinate of the cell.
   * \param y The y coordinate of the cell.
   * \param cell The cell.
   * \param cellGraphicsItem The graphics item representing the cell.
   */
  void updateCellGraphics(int x, int y, Grid::Cell* cell, CellGraphicsItem* graphics);

  /*
   * Data.
   */

  std::shared_ptr<Grid> grid = nullptr; // The grid.
  int cellDisplaySize = 24; // The size of each cell in the grid (in pixels).

  bool editMode = true; // True if the edit mode is active, false otherwise.
  std::set<std::shared_ptr<Grid::Cell>> selected; // The selected cells.

  /*
   * Widgets.
   */

  QGraphicsScene* graphicsScene; // The graphics scene.
};

#endif // GRAPHICS_AREA_H