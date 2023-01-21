#ifndef GRID_H
#define GRID_H

#include <memory>

/**
 * A grid of cells.
 */
class Grid {
  public:

  /**
   * A cell in the grid.
   */
  struct Cell {

    static constexpr unsigned int WALL_COST
        = (unsigned int)-1; // The cost of a wall cell.

    /**
     * The visualization state of a cell.
     * This is used to determine how the cell should be displayed.
     */
    enum class VisualizationState {
      WALL, // The cell is a wall.
      UNVISITED, // The cell has not been visited.
      OPEN_LIST, // The cell is in the open list.
      CLOSED_LIST, // The cell is in the closed list.
      START, // The cell is the start cell.
      GOAL, // The cell is the goal cell.
      PATH // The cell is part of the path.
    };

    unsigned int cost = 1; // The cost of traversing the cell.
    void* searchData = nullptr; // The search data associated with the cell.
    VisualizationState vis
        = VisualizationState::UNVISITED; // The visualization state of the cell.
  };

  /**
   * Construct a new Grid.
   *
   * \param width The width of the grid.
   * \param height The height of the grid.
   */
  Grid(int width, int height);

  /**
   * Destroy a Grid.
   */
  ~Grid();

  /**
   * Get the width of the grid.
   *
   * \return The width of the grid.
   */
  int getWidth() const;

  /**
   * Get the height of the grid.
   *
   * \return The height of the grid.
   */
  int getHeight() const;

  /**
   * Get a cell in the grid.
   * 
   * \param x X coordinate of the cell.
   * \param y Y coordinate of the cell.
   * \return The cell at the given coordinates.
   */
  std::shared_ptr<Cell> getCell(int x, int y) const;

  /*
   * Data.
   */

  std::shared_ptr<Cell>* grid; // The grid.

  private:

  /*
   * Config.
   */

  int width; // The width of the grid.
  int height; // The height of the grid.
};

#endif // GRID_H