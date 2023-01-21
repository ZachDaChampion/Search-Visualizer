#include "graphics_area.h"

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
}

GraphicsArea::~GraphicsArea() { }

void GraphicsArea::updateCells() { }

void GraphicsArea::updateInteractionMode(bool editMode) { this->editMode = editMode; }

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

  // Set start and end cells.
  grid->getCell(0, 0)->vis = Cell::VisualizationState::START;
  grid->getCell(width - 1, height - 1)->vis = Cell::VisualizationState::GOAL;
}

void GraphicsArea::drawGrid()
{
  // Clear old graphics items.
  graphicsScene->clear();
  if (cellGraphicsItems != nullptr) {
    delete[] cellGraphicsItems;
  }

  // Get width and height of the grid.
  int width = grid->getWidth();
  int height = grid->getHeight();

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
  fitInView(graphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

/*
 * Mouse event handlers.
 * These are used to select/deselect cells.
 */

void GraphicsArea::mousePressEvent(QMouseEvent* event)
{
  // Previous position is stored to avoid unnecessary updates.
  static int lastX = -1;
  static int lastY = -1;

  // Get the position of the mouse in the scene.
  QPointF pos = event->position();
  QPointF scenePos = mapToScene(QPoint(pos.x(), pos.y()));

  // Get the cell coordinates.
  int x = scenePos.x() / cellDisplaySize;
  int y = scenePos.y() / cellDisplaySize;

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

  auto buttons = event->buttons();
  if (buttons) {
    std::shared_ptr<Grid::Cell> selectedCell = grid->getCell(x, y);

    // Check left mouse button.
    // Only select if the cell is not already selected.
    if (buttons & Qt::LeftButton && !selectedCell->selected) {
      std::cout << "Selecting cell " << x << ", " << y << std::endl;
      selectedCell->selected = true;
      selected.insert(selectedCell);
    }

    // Check right mouse button.
    // Only deselect if the cell is already selected.
    else if (buttons & Qt::RightButton && selectedCell->selected) {
      std::cout << "Deselecting cell " << x << ", " << y << std::endl;
      selectedCell->selected = false;
      selected.erase(selectedCell);
    }

    // Update the graphics of the cell (this will change the color)
    updateCellGraphics(
        selectedCell.get(), &cellGraphicsItems[y * grid->getWidth() + x]);

    // Update last position.
    lastX = x;
    lastY = y;
  }
}

void GraphicsArea::mouseMoveEvent(QMouseEvent* event)
{
  /*
   * Calling `mousePressedEvent` from `mouseMoveEvent` will allow us to
   * select/deselect cells by dragging the mouse.
   */
  mousePressEvent(event);
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
  case Cell::VisualizationState::UNVISITED:
    graphics->rect->setBrush(QBrush(Qt::gray));
    graphics->text->setDefaultTextColor(Qt::black);
    graphics->text->setPlainText(QString::number(cell->cost));
    break;
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