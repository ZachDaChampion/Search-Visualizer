#include "grid.h"

#include <stdexcept>

Grid::Grid(int width, int height)
    : width(width)
    , height(height)
{
  // Allocate the grid.
  grid = new std::shared_ptr<Cell>[width * height];

  // Initialize the grid.
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      grid[y * width + x] = std::make_shared<Cell>();
      grid[y * width + x]->x = x;
      grid[y * width + x]->y = y;
    }
  }
}

Grid::~Grid()
{
  // Deallocate the grid.
  delete[] grid;
}

int Grid::getWidth() const { return width; }

int Grid::getHeight() const { return height; }

std::shared_ptr<Grid::Cell> Grid::getCell(int x, int y) const
{
  // Check if the cell is in the grid.
  if (x < 0 || x >= width || y < 0 || y >= height) {
    throw std::out_of_range("Cell coordinates out of range.");
  }

  // Return the cell.
  return grid[y * width + x];
}