#include "graphics_area.h"
#include "../global_state.h"
#include "edit_tab.h"

#include <QApplication>
#include <QGraphicsColorizeEffect>
#include <QHBoxLayout>
#include <iostream>

using Cell = Grid::Cell;

GraphicsArea::GraphicsArea(int minWidth, int minHeight, QWidget* parent)
    : QGraphicsView(parent)
{
  setMinimumSize(minWidth, minHeight);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphicsScene = new QGraphicsScene(this);
  setScene(graphicsScene);

  // Connect signals and slots.
  GlobalState& globalState = GlobalState::singleton();
  connect(&globalState, &GlobalState::simTypeChanged, this, &GraphicsArea::simTypeSlot);
}

GraphicsArea::~GraphicsArea() { }

/*
 * Public slots.
 */

void GraphicsArea::updateCells() { }

void GraphicsArea::updateInteractionMode(bool editMode) { this->editMode = editMode; }

void GraphicsArea::setCostSelectedCells(int cost)
{
  // Iterate over selected cells and set their cost.
  for (auto& cell : selected) {

    // Do nothing if the cell is a start or goal cell.
    if (cell->vis == Cell::VisualizationState::START
        || cell->vis == Cell::VisualizationState::GOAL) {
      continue;
    }

    // Update the cell cost.
    cell->cost = cost;

    // Update the visualization state.
    if (cost == Cell::WALL_COST) {
      cell->vis = Cell::VisualizationState::WALL;
    } else {
      cell->vis = Cell::VisualizationState::UNVISITED;
    }

    // Update the graphics.
    updateCellGraphics(
        cell.get(), &cellGraphicsItems[cell->y * grid->getWidth() + cell->x]);
  }
}

void GraphicsArea::setStartCellSelected()
{
  // Do nothing if no cells are selected.
  if (selected.empty()) {
    return;
  }

  // Get the first selected cell.
  std::shared_ptr<Cell> cell = *selected.begin();

  // Do nothing if the cell is the start or goal cell.
  if (cell == startCell || cell == goalCell) {
    return;
  }

  // Update the visualization state of the old start cell.
  startCell->vis = Cell::VisualizationState::UNVISITED;
  updateCellGraphics(startCell.get(),
      &cellGraphicsItems[startCell->y * grid->getWidth() + startCell->x]);

  // Update the visualization state of the new start cell.
  cell->vis = Cell::VisualizationState::START;
  updateCellGraphics(
      cell.get(), &cellGraphicsItems[cell->y * grid->getWidth() + cell->x]);

  // Update the start cell pointer.
  startCell = cell;
}

void GraphicsArea::setGoalCellSelected()
{
  // Do nothing if no cells are selected.
  if (selected.empty()) {
    return;
  }

  // Get the first selected cell.
  std::shared_ptr<Cell> cell = *selected.begin();

  // Do nothing if the cell is the start or goal cell.
  if (cell == startCell || cell == goalCell) {
    return;
  }

  // Update the visualization state of the old goal cell.
  goalCell->vis = Cell::VisualizationState::UNVISITED;
  updateCellGraphics(
      goalCell.get(), &cellGraphicsItems[goalCell->y * grid->getWidth() + goalCell->x]);

  // Update the visualization state of the new goal cell.
  cell->vis = Cell::VisualizationState::GOAL;
  updateCellGraphics(
      cell.get(), &cellGraphicsItems[cell->y * grid->getWidth() + cell->x]);

  // Update the goal cell pointer.
  goalCell = cell;
}

void GraphicsArea::resetGrid(int width, int height)
{
  // Initialize the grid.
  initGrid(width, height);

  // Update the graphics.
  drawGrid();

  // Update the graphics area size.
  graphicsScene->setSceneRect(0, 0, width * cellDisplaySize, height * cellDisplaySize);
  resizeEvent(nullptr);
}

void GraphicsArea::initGrid(int width, int height)
{
  // Delete the old graphics items.
  graphicsScene->clear();
  if (cellGraphicsItems != nullptr) {
    delete[] cellGraphicsItems;
  }
  cellGraphicsItems = nullptr;

  // Create a new grid.
  grid = std::make_shared<Grid>(width, height);

  // Set start and goal cells.
  startCell = grid->getCell(0, 0);
  goalCell = grid->getCell(width - 1, height - 1);
  startCell->vis = Cell::VisualizationState::START;
  goalCell->vis = Cell::VisualizationState::GOAL;
}

void GraphicsArea::drawGrid()
{
  // Clear old graphics items.
  graphicsScene->clear();
  if (cellGraphicsItems != nullptr) {
    delete[] cellGraphicsItems;
  }

  // Get width and height of the grid.
  const int width = grid->getWidth();
  const int height = grid->getHeight();

  /*
   * Create new graphics items
   */

  cellGraphicsItems = new CellGraphicsItem[width * height];

  // Create a cosmetic pen for the cell borders.
  // This will make the pen width independent of the zoom level.
  QPen pen(Qt::black);
  pen.setWidth(2);
  pen.setCosmetic(true);

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {

      // Create a new graphics item.
      CellGraphicsItem& item = cellGraphicsItems[y * width + x];

      // Setup rectangle.
      item.rect = new QGraphicsRectItem(
          x * cellDisplaySize, y * cellDisplaySize, cellDisplaySize, cellDisplaySize);
      item.rect->setPen(pen);
      item.rect->setBrush(QBrush(Qt::white));
      item.rect->setZValue(0);

      // Setup text.
      item.text = new QGraphicsTextItem(item.rect);
      item.text->setZValue(3);

      // Update the graphics of the cell.
      updateCellGraphics(grid->getCell(x, y).get(), &item);

      // Add the item to the scene.
      graphicsScene->addItem(item.rect);
    }
  }
}

/*
 * Private slots.
 */

void GraphicsArea::simTypeSlot(GlobalState::SimType type)
{

  /*
   * If no sim is running, simply set the editMode flag.
   * If a sim is running, unset the editMode flag and clear the current selection.
   */

  if (type == GlobalState::SimType::NONE) {
    editMode = true;
  } else {
    editMode = false;
    for (std::shared_ptr<Cell> cell : selected) {
      cell->selected = false;
      updateCellGraphics(
          cell.get(), &cellGraphicsItems[cell->y * grid->getWidth() + cell->x]);
    }
    selected.clear();
  }
}

/*
 * Getters.
 */

std::shared_ptr<Grid> GraphicsArea::getGrid() const { return grid; }

/*
 * Resize handlers.
 */

void GraphicsArea::resizeEvent(QResizeEvent* event)
{
  fitInView(graphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void GraphicsArea::showEvent(QShowEvent* event)
{
  resizeEvent(nullptr);
}

/*
 * Mouse event handlers.
 * These are used to select/deselect cells.
 */

void GraphicsArea::mousePressEvent(QMouseEvent* event)
{
  if (!editMode) {
    return;
  }

  /*
   * When the mouse is pressed, we want to clear the selection and select the
   * cell under the mouse cursor. The latter is done by calling the mouse move event
   * handler, since the code is the same.
   */

  // Clear the selection.
  for (std::shared_ptr<Cell> cell : selected) {
    cell->selected = false;
    updateCellGraphics(
        cell.get(), &cellGraphicsItems[cell->y * grid->getWidth() + cell->x]);
  }
  selected.clear();

  mouseMoveEvent(event);
}

void GraphicsArea::mouseMoveEvent(QMouseEvent* event)
{
  if (!editMode) {
    return;
  }

  // Previous position is stored to avoid unnecessary updates.
  static int lastX = -1;
  static int lastY = -1;

  // Get the position of the mouse in the scene.
  QPointF pos = event->position();
  QPointF scenePos = mapToScene(QPoint(pos.x(), pos.y()));

  // Get the cell coordinates.
  const int x = scenePos.x() / cellDisplaySize;
  const int y = scenePos.y() / cellDisplaySize;

  // Check if the mouse has moved to a new cell.
  if (x == lastX && y == lastY) {
    return;
  }

  // Check if the mouse is within the grid.
  if (x < 0 || x >= grid->getWidth() || y < 0 || y >= grid->getHeight()) {
    return;
  }

  /*
   * If a mouse button is pressed, we want to process it and then update the
   * last position. We also want to select/deselect cells if the left/right
   * mouse buttons are pressed respectively.
   *
   * Selected cells are added to the `selected` set and deselected cells are
   * removed from the set.
   */

  const auto buttons = event->buttons();
  const bool leftMouse = buttons & Qt::LeftButton;

  if (leftMouse) {
    std::shared_ptr<Grid::Cell> selectedCell = grid->getCell(x, y);

    // Check left mouse button without ctrl pressed.
    // Only select if the cell is not already selected.
    if (!selectedCell->selected) {
      std::cout << "Selecting cell " << x << ", " << y << std::endl;
      selectedCell->selected = true;
      selected.insert(selectedCell);

      // Update the graphics of the cell (this will change the color)
      updateCellGraphics(
          selectedCell.get(), &cellGraphicsItems[y * grid->getWidth() + x]);
    }

    // Update last position.
    lastX = x;
    lastY = y;
  }
}

void GraphicsArea::keyPressEvent(QKeyEvent* event)
{
  if (!editMode) {
    return;
  }

  const int key = event->key();

  /*
   * The escape key should clear the selection.
   */

  if (key == Qt::Key_Escape) {
    // Clear the selection.
    for (std::shared_ptr<Cell> cell : selected) {
      cell->selected = false;
      updateCellGraphics(
          cell.get(), &cellGraphicsItems[cell->y * grid->getWidth() + cell->x]);
    }
    selected.clear();
    return;
  }

  /*
   * The number keys 1-9 should set the cell cost of all selected cells.
   */

  // Check if key value is less then 1 key.
  if (key < Qt::Key_1) {
    return;
  }

  // Calculate an offset from the 0 key.
  // If this offset is between 1 and 9, it is the numeric equivalent of the key that was
  // pressed. For instance, if Qt::Key_5 is pressed, offset will be 5.
  int offset = key - Qt::Key_0;
  if (offset > 9) { // We already know that offset is at least 1.
    return;
  }

  // Update all selected cells with new cost.
  for (std::shared_ptr<Cell> cell : selected) {
    cell->cost = offset;
    updateCellGraphics(
        cell.get(), &cellGraphicsItems[cell->y * grid->getWidth() + cell->x]);
  }
}

/*
 * Private.
 */

void GraphicsArea::updateCellGraphics(Cell* cell, CellGraphicsItem* graphics)
{
  // Update the graphics of the cell based on its visualization state.
  switch (cell->vis) {
  case Cell::VisualizationState::WALL:
    graphics->rect->setBrush(QBrush(Qt::black));
    graphics->text->setPlainText("");
    break;

  case Cell::VisualizationState::UNVISITED: {

    // Scale the color between green and red based on the cost.
    // Color codes are mapped from [MIN_CELL_COST, MAX_CELL_COST to [75, 175].
    // This allows start and goal cells to still be distinguishable.
    // Red is used for high cost, green is used for low cost.
    float ratio = (float)(cell->cost - GlobalState::MIN_CELL_COST)
        / (float)(GlobalState::MAX_CELL_COST - GlobalState::MIN_CELL_COST);
    int red = 100 * ratio + 75;
    int green = 100 * (1.0 - ratio) + 75;

    graphics->rect->setBrush(QBrush(QColor(red, green, 0)));
    graphics->text->setDefaultTextColor(Qt::black);
    graphics->text->setPlainText(QString::number(cell->cost));
  } break;

  case Cell::VisualizationState::OPEN_LIST:
    graphics->rect->setBrush(QBrush(Qt::lightGray));
    graphics->text->setDefaultTextColor(Qt::black);
    graphics->text->setPlainText(QString::number(cell->cost));
    break;

  case Cell::VisualizationState::CLOSED_LIST:
    graphics->rect->setBrush(QBrush(Qt::darkGray));
    graphics->text->setDefaultTextColor(Qt::white);
    graphics->text->setPlainText(QString::number(cell->cost));
    break;

  case Cell::VisualizationState::START:
    graphics->rect->setBrush(QBrush(Qt::green));
    graphics->text->setPlainText("");
    break;

  case Cell::VisualizationState::GOAL:
    graphics->rect->setBrush(QBrush(Qt::red));
    graphics->text->setPlainText("");
    break;

  case Cell::VisualizationState::PATH:
    graphics->rect->setBrush(QBrush(Qt::white));
    graphics->text->setDefaultTextColor(Qt::black);
    graphics->text->setPlainText(QString::number(cell->cost));
    break;
  }

  // If the cell is selected, add a yellow color effect.
  if (cell->selected) {
    QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect;
    effect->setColor(Qt::yellow);
    effect->setStrength(0.5);
    graphics->rect->setGraphicsEffect(effect);
    graphics->rect->setZValue(1);
  } else {
    graphics->rect->setGraphicsEffect(nullptr);
    graphics->rect->setZValue(0);
  }

  // Center the text in the cell.
  QRectF cellBounds = graphics->rect->boundingRect();
  int x = cellBounds.x();
  int y = cellBounds.y();
  QRectF rect = graphics->text->boundingRect();
  graphics->text->setPos(x + (cellDisplaySize - rect.width()) / 2,
      y + (cellDisplaySize - rect.height()) / 2);
}