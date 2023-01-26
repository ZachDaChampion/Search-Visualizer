#include "grid.h"

#include <algorithm>
#include <stdexcept>

Grid::Grid(int width, int height)
    : width(width)
    , height(height)
{
  // Allocate the grid.
  grid = new std::shared_ptr<Cell>[width * height];

  // Create initial name.
  constexpr int charCount = 26;
  int nameNum = 1;

  // Initialize the grid.
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      grid[y * width + x] = std::make_shared<Cell>();
      grid[y * width + x]->x = x;
      grid[y * width + x]->y = y;

      // Generate name.
      int tmp = nameNum;
      grid[y * width + x]->name = "";
      while (tmp > 0) {
        grid[y * width + x]->name += 'A' + (tmp - 1) % charCount;
        tmp = (tmp - 1) / charCount;
      }

      // Reverse name.
      reverse(grid[y * width + x]->name.begin(), grid[y * width + x]->name.end());

      ++nameNum;
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