#ifndef ASTAR_H
#define ASTAR_H

#include <memory>
#include <queue>
#include <vector>

#include "../global_state.h"
#include "../grid.h"
#include "../widgets/graphics_area.h"
#include "search_algorithm.h"

namespace SearchAlgorithms {

class AStar : public SearchAlgorithm {

  public:

  /**
   * The search data associated with a cell.
   */
  struct SearchData {
    unsigned int g = 0; // The cost to get to the cell.
    unsigned int h = 0; // The heuristic cost to get to the goal.
    unsigned int f = 0; // The total cost of the cell.
    std::shared_ptr<Grid::Cell> parent = nullptr; // The parent of the cell.
    bool closed = false; // Whether the cell is in the closed list.
  };

  /**
   * Construct a new AStar.
   *
   * \param grid The grid to step through.
   * \param start The start cell.
   * \param goal The goal cell.
   * \param parent The parent object.
   */
  AStar(std::shared_ptr<Grid> grid, const std::shared_ptr<Grid::Cell> start,
      const std::shared_ptr<Grid::Cell> goal);

  /**
   * Destroy a AStar.
   */
  ~AStar();

  /**
   * Step through the AStar algorithm.
   * Cells will be updated in this function.
   * A vector of cells that were updated will be returned, so that the graphics area can
   * update them.
   * An empty vector will be returned if the algorithm is finished.
   *
   * \return A vector of cells that were updated.
   */
  std::vector<std::shared_ptr<Grid::Cell>> step();

  /**
   * Get the path found by the search algorithm.
   * This function should only be called after the algorithm has finished.
   * An empty vector will be returned if no path was found or the algorithm has not
   * finished.
   *
   * \return A vector of cells that are in the path.
   */
  std::vector<std::shared_ptr<Grid::Cell>> path();

  private:

  /*
   * Data.
   */

  std::shared_ptr<Grid> grid_; // The grid to step through.
  std::shared_ptr<Grid::Cell> startCell_; // The start cell.
  std::shared_ptr<Grid::Cell> goalCell_; // The goal cell.
  std::vector<std::shared_ptr<Grid::Cell>> path_; // The path found by the algorithm.

  /**
   * Custom comparator for cells.
   */
  struct CompareCells {
    bool operator()(const std::shared_ptr<Grid::Cell> lhs,
        const std::shared_ptr<Grid::Cell> rhs) const;
  } compareCells;

  /*
   * Data.
   */

  public:

  std::priority_queue<std::shared_ptr<Grid::Cell>,
      std::vector<std::shared_ptr<Grid::Cell>>,
      CompareCells>
      pQueue; // The queue of cells to visit.
};
}

#endif