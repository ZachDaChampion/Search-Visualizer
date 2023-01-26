#ifndef GRAPHICS_AREA_H
#define GRAPHICS_AREA_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWidget>

#include <memory>
#include <set>
#include <vector>

#include "../global_state.h"
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

  /**
   * Set the cost of the selected cells.
   *
   * \param cost The cost of the selected cells.
   */
  void setCostSelectedCells(int cost);

  /**
   * Set the currently selected cell as the start cell.
   * If multiple cells are selected, a random one will be selected as the start cell.
   */
  void setStartCellSelected();

  /**
   * Set the currently selected cell as the goal cell.
   * If multiple cells are selected, a random one will be selected as the goal cell.
   */
  void setGoalCellSelected();

  /**
   * Resize the grid.
   * This will create a new grid and clear the existing one.
   *
   * \param width The width of the grid.
   * \param height The height of the grid.
   */
  void resetGrid(int width, int height);

  private slots:
  /**
   * Update editability based on simulation type.
   * Editing will be enabled when no simulation is running and disabled otherwise.
   *
   * \param type The simulation type.
   */
  void simTypeSlot(GlobalState::SimType type);

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

  /**
   * Handle a resize event.
   * This will scale the graphics items to fit the new size of the widget.
   *
   * \param event The resize event.
   */
  void resizeEvent(QResizeEvent* event) override;

  /**
   * Handle a show event.
   * This will scale the graphics when the widget is shown.
   *
   * \param event The show event.
   */
  void showEvent(QShowEvent* event) override;

  /**
   * Handle a mouse press event.
   * This will select/deselect cells in the grid.
   *
   * \param event The mouse press event.
   */
  void mousePressEvent(QMouseEvent* event) override;

  /**
   * Handle a mouse move event.
   * This will select/deselect cells in the grid.
   *
   * \param event The mouse move event.
   */
  void mouseMoveEvent(QMouseEvent* event) override;

  /**
   * Handle a key press event.
   * This allows the escape key to be used to deselect all cells.
   *
   * \param event The key press event.
   */
  void keyPressEvent(QKeyEvent* event) override;

  private:

  /*
   * Graphics items.
   */

  struct CellGraphicsItem {
    QGraphicsRectItem* rect = nullptr; // The rectangle representing the cell.
    QGraphicsRectItem* highlight = nullptr; // The rectangle representing the highlight.
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
  void updateCellGraphics(Grid::Cell* cell, CellGraphicsItem* graphics);

  /*
   * Data.
   */

  std::shared_ptr<Grid> grid = nullptr; // The grid.
  int cellDisplaySize = 24; // The size of each cell in the grid (in pixels).

  bool editMode = true; // True if the edit mode is active, false otherwise.
  std::set<std::shared_ptr<Grid::Cell>> selected; // The selected cells.

  std::shared_ptr<Grid::Cell> startCell = nullptr; // The start cell.
  std::shared_ptr<Grid::Cell> goalCell = nullptr; // The goal cell.

  /*
   * Widgets.
   */

  QGraphicsScene* graphicsScene; // The graphics scene.
};

#endif // GRAPHICS_AREA_H