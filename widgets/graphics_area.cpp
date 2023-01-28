#include "graphics_area.h"
#include "../algorithms/dijkstra.h"
#include "../game_loop.h"
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
  setRenderHint(QPainter::Antialiasing);
  graphicsScene = new QGraphicsScene(this);
  setScene(graphicsScene);

  // Connect signals and slots.
  GlobalState& globalState = GlobalState::singleton();
  GameLoop& gameLoop = GameLoop::singleton();
  connect(&globalState, &GlobalState::simTypeChanged, this, &GraphicsArea::simTypeSlot);
  connect(&gameLoop, &GameLoop::updateGraphics, this, &GraphicsArea::updateCells);
}

GraphicsArea::~GraphicsArea() { }

/*
 * Public slots.
 */

void GraphicsArea::updateCells(std::vector<std::shared_ptr<Grid::Cell>> cells)
{
  // Update the cells.
  for (auto& cell : cells) {
    updateCellGraphics(
        cell.get(), &cellGraphicsItems[cell->y * grid->getWidth() + cell->x]);
  }
}

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

  // Create pen for highlighting.
  constexpr int bw = GlobalState::CELL_BORDER_WIDTH;
  QPen highlightPen(Qt::yellow);
  highlightPen.setWidth(bw);

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

      // Setup highlight.
      item.highlight = new QGraphicsRectItem(x * cellDisplaySize + bw / 2,
          y * cellDisplaySize + bw / 2, cellDisplaySize - bw, cellDisplaySize - bw,
          item.rect);
      item.highlight->setPen(highlightPen);
      item.highlight->setBrush(Qt::NoBrush);
      item.highlight->setZValue(1);
      item.highlight->hide();

      // Setup cost text.
      item.text = new QGraphicsTextItem(item.rect);
      item.text->setZValue(3);

      // Setup name.
      item.name = new QGraphicsTextItem(item.rect);
      item.name->setZValue(3);

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
  // Enable/disable editing mode.
  editMode = (type == GlobalState::SimType::NONE);

  // Loop endtire grid and update graphics.
  for (int x = 0; x < grid->getWidth(); ++x) {
    for (int y = 0; y < grid->getHeight(); ++y) {
      auto cell = grid->getCell(x, y);
      cell->selected = false; // Deselect all cells.

      // Clear path and list visualization.
      switch (cell->vis) {
      case Cell::VisualizationState::PATH:
      case Cell::VisualizationState::OPEN_LIST:
      case Cell::VisualizationState::CLOSED_LIST:
        cell->vis = Cell::VisualizationState::UNVISITED;
      default:
        break;
      }

      updateCellGraphics(cell.get(), &cellGraphicsItems[y * grid->getWidth() + x]);
    }
  }

  // Create search algorithm.
  GameLoop& gameLoop = GameLoop::singleton();

  switch (type) {
  case GlobalState::SimType::NONE:
    gameLoop.setSim(nullptr);
    break;
  case GlobalState::SimType::DIJKSTRA:
    gameLoop.setSim(
        std::make_shared<SearchAlgorithms::Dijkstra>(grid, startCell, goalCell));
    break;
  default:
    break;
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

void GraphicsArea::showEvent(QShowEvent* event) { resizeEvent(nullptr); }

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
   * The 'W' key should set the cell type of all selected cells to WALL.
   */

  if (key == Qt::Key_W) {
    setCostSelectedCells(Cell::WALL_COST);
    return;
  }

  /*
   * The 'S' key should set the cell type of all selected cells to START.
   */

  if (key == Qt::Key_S) {
    setStartCellSelected();
    return;
  }

  /*
   * The 'G' key should set the cell type of all selected cells to GOAL.
   */

  if (key == Qt::Key_G) {
    setGoalCellSelected();
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
  // If this offset is between 1 and 9, it is the numeric equivalent of the key that
  // was pressed. For instance, if Qt::Key_5 is pressed, offset will be 5.
  int offset = key - Qt::Key_0;
  if (offset > 9) { // We already know that offset is at least 1.
    return;
  }

  // Update all selected cells with new cost.
  for (std::shared_ptr<Cell> cell : selected) {
    if (cell == startCell || cell == goalCell) {
      continue;
    } else {
      cell->cost = offset;
      cell->vis = Cell::VisualizationState::UNVISITED;
    }
    updateCellGraphics(
        cell.get(), &cellGraphicsItems[cell->y * grid->getWidth() + cell->x]);
  }
}

/*
 * Private.
 */

void GraphicsArea::updateCellGraphics(Cell* cell, CellGraphicsItem* graphics)
{
  // Set highlight pen.
  constexpr int bw = GlobalState::CELL_BORDER_WIDTH;
  QPen highlightPen(Qt::yellow);
  highlightPen.setWidth(bw);

  /*
   * Set the color of the cell.
   * Walls should be black, start and goal cells should be green and red, and
   * other cells should be scaled between green and red based on the cost.
   */

  if (cell->vis == Cell::VisualizationState::WALL) {
    graphics->rect->setBrush(QBrush(Qt::black));
    graphics->name->setDefaultTextColor(Qt::white);
    graphics->text->setDefaultTextColor(Qt::white);
    graphics->text->setPlainText("W");
  } else if (cell->vis == Cell::VisualizationState::START) {
    graphics->rect->setBrush(QBrush(Qt::green));
    graphics->name->setDefaultTextColor(Qt::black);
    graphics->text->setDefaultTextColor(Qt::black);
    graphics->text->setPlainText("S");
  } else if (cell->vis == Cell::VisualizationState::GOAL) {
    graphics->rect->setBrush(QBrush(Qt::magenta));
    graphics->name->setDefaultTextColor(Qt::black);
    graphics->text->setDefaultTextColor(Qt::black);
    graphics->text->setPlainText("G");
  } else {

    // Scale the color between green and red based on the cost.
    // Color codes are mapped from [MIN_CELL_COST, MAX_CELL_COST to [75, 175].
    // This allows start and goal cells to still be distinguishable.
    // Red is used for high cost, green is used for low cost.
    float ratio = (float)(cell->cost - GlobalState::MIN_CELL_COST)
        / (float)(GlobalState::MAX_CELL_COST - GlobalState::MIN_CELL_COST);
    int red = 100 * ratio + 75;
    int green = 100 * (1.0 - ratio) + 75;

    graphics->rect->setBrush(QBrush(QColor(red, green, 0)));
    graphics->name->setDefaultTextColor(Qt::black);
    graphics->text->setDefaultTextColor(Qt::black);
    graphics->text->setPlainText(QString::number(cell->cost));
  }

  /*
   * In edit mode, we want to highlight the cell in yellow if it is selected.
   * Otherwise, we want to highlight the cell based on its visualization state.
   */

  if (editMode) {
    if (cell->selected) {
      graphics->highlight->setPen(highlightPen);
      graphics->highlight->show();
    } else {
      graphics->highlight->hide();
    }
  } else {
    switch (cell->vis) {
    case Cell::VisualizationState::WALL:
      graphics->highlight->hide();
      break;

    case Cell::VisualizationState::UNVISITED:
      graphics->highlight->hide();
      break;

    case Cell::VisualizationState::OPEN_LIST:
      highlightPen.setColor(Qt::blue);
      graphics->highlight->setPen(highlightPen);
      graphics->highlight->show();
      break;

    case Cell::VisualizationState::CLOSED_LIST:
      highlightPen.setColor(Qt::red);
      graphics->highlight->setPen(highlightPen);
      graphics->highlight->show();
      break;

    case Cell::VisualizationState::PATH:
      highlightPen.setColor(Qt::green);
      graphics->highlight->setPen(highlightPen);
      graphics->highlight->show();
      break;
    }
  }

  // Set font size.
  QFont font = graphics->text->font();
  font.setPointSize(cellDisplaySize / 4);
  graphics->text->setFont(font);

  // Center the cost text in the cell.
  QRectF cellBounds = graphics->rect->boundingRect();
  int x = cellBounds.x();
  int y = cellBounds.y();
  QRectF rect = graphics->text->boundingRect();
  graphics->text->setPos(x + (cellDisplaySize - rect.width()) / 2,
      y + (cellDisplaySize - rect.height()) / 2);

  // Set cell name.
  font.setPointSize(cellDisplaySize / 5);
  graphics->name->setPlainText(QString::fromStdString(cell->name));
  graphics->name->setFont(font);

  // Put cell name in top left corner.
  graphics->name->setPos(x - 1, y - 2);
}