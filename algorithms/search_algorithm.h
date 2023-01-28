#ifndef SEARCH_ALGORITHM_H
#define SEARCH_ALGORITHM_H

#include <exception>
#include <memory>
#include <vector>

#include "../grid.h"

namespace SearchAlgorithms {

/**
 * Exception thrown when no path is found.
 */
class NoPathFoundException : public std::exception {
  public:
  const char* what() const throw() { return "No path found."; }
};

/**
 * Base class for search algorithms.
 */
class SearchAlgorithm {
  public:

  /**
   * Step through the search algorithm.
   * Cells will be updated in this function.
   * A vector of cells that were updated will be returned, so that the graphics area can
   * update them.
   * An empty vector will be returned if the algorithm is finished.
   *
   * \return A vector of cells that were updated.
   */
  virtual std::vector<std::shared_ptr<Grid::Cell>> step() = 0;

  /**
   * Get the path found by the search algorithm.
   * This function should only be called after the algorithm has finished.
   * An empty vector will be returned if no path was found or the algorithm has not
   * finished.
   *
   * \return A vector of cells that are in the path.
   */
  virtual std::vector<std::shared_ptr<Grid::Cell>> path() = 0;
};
}

#endif