#include "dijkstra.h"

#include <algorithm>
#include <iostream>

using Cell = Grid::Cell;

namespace SearchAlgorithms {

Dijkstra::Dijkstra(std::shared_ptr<Grid> grid, const std::shared_ptr<Cell> start,
    const std::shared_ptr<Cell> goal)
{
  // Init grid
  this->grid_ = grid;
  this->startCell_ = start;
  this->goalCell_ = goal;

  /*
   * Ensure that cells do not have any data left over from previous searches.
   * Search data is cleared and visualization state is reset.
   */

  for (int x = 0; x < grid_->getWidth(); x++) {
    for (int y = 0; y < grid_->getHeight(); y++) {
      auto cell = grid_->getCell(x, y);

      // Clear search data
      if (cell->searchData != nullptr) {
        std::cerr << "Warning: Cell " << x << ", " << y
                  << " has search data left over from a previous search." << std::endl;
        cell->searchData = nullptr;
      }

      // Reset visualization state
      switch (cell->vis) {
      case Cell::VisualizationState::PATH:
      case Cell::VisualizationState::OPEN_LIST:
      case Cell::VisualizationState::CLOSED_LIST:
        cell->vis = Cell::VisualizationState::UNVISITED;
      default:
        break;
      }
    }
  }

  // Add start cell to open list
  auto searchData = new SearchData;
  startCell_->searchData = searchData;
  pQueue.push(startCell_);
}

Dijkstra::~Dijkstra()
{
  // Clear search data
  for (int x = 0; x < grid_->getWidth(); x++) {
    for (int y = 0; y < grid_->getHeight(); y++) {
      auto cell = grid_->getCell(x, y);
      if (cell->searchData != nullptr) {
        auto searchData = (SearchData*)(cell->searchData);
        delete searchData;
        cell->searchData = nullptr;
      }
    }
  }
}

std::vector<std::shared_ptr<Grid::Cell>> Dijkstra::step()
{
  std::vector<std::shared_ptr<Grid::Cell>> updatedCells;

  // Check if the open list is empty
  if (pQueue.empty()) {
    throw NoPathFoundException(); // No path was found
  }

  // Get the cell with the lowest cost
  auto cell = pQueue.top();

  // Get cell search data
  auto searchData = (SearchData*)(cell->searchData);

  // If the goal has been found, the path can be reconstructed.
  // An empty vector will be returned, indicating that the algorithm is finished.
  if (cell == goalCell_) {

    /*
     * To reconstruct the path, we start at the goal cell and work our way back to the
     * start cell. Each cell along the path is added to the `path_` vector. Once we
     * reach the start cell, we reverse the vector so that the path starts at the start
     * cell and ends at the goal cell.
     */

    auto pathCell = goalCell_;
    while (pathCell != startCell_) {
      auto pathSD = (SearchData*)(pathCell->searchData);
      path_.push_back(pathCell);
      if (pathCell != goalCell_) {
        pathCell->vis = Cell::VisualizationState::PATH;
      }
      pathCell = pathSD->parent;
    }
    path_.push_back(startCell_);
    std::reverse(path_.begin(), path_.end());

    std::cout << "Path found!" << std::endl;

    return updatedCells;
  }

  // Remove the cell from the open list.
  pQueue.pop();

  // Add the cell to the closed list and update its visualization state.
  searchData->closed = true;
  if (cell != startCell_) {
    cell->vis = Cell::VisualizationState::CLOSED_LIST;
  }
  updatedCells.push_back(cell);

  // Get the neighbors of the cell.
  std::vector<std::shared_ptr<Grid::Cell>> neighbors = grid_->getNeighbors(cell);

  /*
   * For each neighbor, we first check if it is in the closed list. If it is, we do
   * not need to do anything with it.
   *
   * If it is not in the closed list, we check if it is in the open list. This can be
   * done by checking if its searchdata has been initialized. If it is a nullptr, it
   * is not yet in the open list and must be added. New search data is created.
   *
   * If it is in the open list, we check if the new cost is lower than the current
   * cost and update the cost if it is.
   */

  for (auto neighbor : neighbors) {
    auto neighborSD = (SearchData*)(neighbor->searchData);
    unsigned int newCost = searchData->cost + neighbor->cost;

    // Neighbor is unvisited.
    if (neighborSD == nullptr) {
      neighborSD = new SearchData;

      // If the neighbor is a wall, it is added to the closed list.
      if (neighbor->cost == Cell::WALL_COST) {
        neighborSD->closed = true;
        continue;
      }

      neighbor->text_tr = std::to_string(newCost);
      neighborSD->cost = newCost;
      neighborSD->parent = cell;
      neighbor->searchData = neighborSD;
      pQueue.push(neighbor);
      if (neighbor != goalCell_) {
        neighbor->vis = Cell::VisualizationState::OPEN_LIST;
      }
      updatedCells.push_back(neighbor);
    }

    // Neighbor is in the closed list.
    else if (neighborSD->closed) {
      continue;
    }

    // Neighbor is in the open list and has a higher cost.
    else if (newCost < searchData->cost) {
      // Update the cost of the neighbor.
      neighbor->text_tr = std::to_string(newCost);
      neighborSD->cost = newCost;
      neighborSD->parent = cell;
      updatedCells.push_back(neighbor);
    }
  }

  return updatedCells;
}

std::vector<std::shared_ptr<Grid::Cell>> Dijkstra::path() { return path_; }

bool Dijkstra::CompareCells::operator()(
    const std::shared_ptr<Cell> a, const std::shared_ptr<Cell> b) const
{
  auto aData = (SearchData*)(a->searchData);
  auto bData = (SearchData*)(b->searchData);

  return aData->cost > bData->cost;
}
} // namespace SearchAlgorithms