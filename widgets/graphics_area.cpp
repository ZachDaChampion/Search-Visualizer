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

      /*
       * Setup text.
       */

      // Create the text item.
      item.text = new QGraphicsTextItem(item.rect);
      item.text->setZValue(3);
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
 * Event handlers.
 */

void GraphicsArea::resizeEvent(QResizeEvent* event)
{
  fitInView(graphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void GraphicsArea::showEvent(QShowEvent* event)
{
  fitInView(graphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

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

  // Check mouse button.
  auto buttons = event->buttons();
  if (buttons) {

    // Check left mouse button.
    if (buttons & Qt::LeftButton) {
      std::shared_ptr<Grid::Cell> selectedCell = grid->getCell(x, y);
      if (!selectedCell->selected) {
        std::cout << "Selecting cell " << x << ", " << y << std::endl;
        selectedCell->selected = true;
        updateCellGraphics(
            selectedCell.get(), &cellGraphicsItems[y * grid->getWidth() + x]);
        selected.insert(selectedCell);
      }
    }

    // Check right mouse button.
    if (buttons & Qt::RightButton) {
      std::shared_ptr<Grid::Cell> selectedCell = grid->getCell(x, y);
      if (selectedCell->selected) {
        std::cout << "Deselecting cell " << x << ", " << y << std::endl;
        selectedCell->selected = false;
        updateCellGraphics(
            selectedCell.get(), &cellGraphicsItems[y * grid->getWidth() + x]);
        selected.erase(selectedCell);
      }
    }

    // Update last position.
    lastX = x;
    lastY = y;
  }
}

void GraphicsArea::mouseMoveEvent(QMouseEvent* event) { mousePressEvent(event); }

/*
 * Private.
 */

void GraphicsArea::updateCellGraphics(Cell* cell, CellGraphicsItem* graphics)
{
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
  QRectF cellBounds = graphics->rect->boundingRect();
  int x = cellBounds.x();
  int y = cellBounds.y();
  QRectF rect = graphics->text->boundingRect();
  graphics->text->setPos(x + (cellDisplaySize - rect.width()) / 2,
      y + (cellDisplaySize - rect.height()) / 2);
}