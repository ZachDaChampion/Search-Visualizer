#include "graphics_area.h"

#include <QHBoxLayout>

GraphicsArea::GraphicsArea(QWidget* parent)
    : QWidget(parent)
{
  graphicsView = new QGraphicsView(this);
  graphicsScene = new QGraphicsScene(this);
  graphicsView->setScene(graphicsScene);

  setLayout(new QHBoxLayout(this));
  layout()->addWidget(graphicsView);
}

GraphicsArea::~GraphicsArea() { }

void GraphicsArea::updateCells() { }

void GraphicsArea::updateInteractionMode(bool editMode) { this->editMode = editMode; }

void GraphicsArea::initGrid(int width, int height)
{
  grid = std::make_shared<Grid>(width, height);
}

std::shared_ptr<Grid> GraphicsArea::getGrid() const { return grid; }

void GraphicsArea::setCellDisplaySize(int size) { this->cellDisplaySize = size; }

int GraphicsArea::getCellDisplaySize() const { return cellDisplaySize; }

void GraphicsArea::drawGrid() { }