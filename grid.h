#ifndef GRID_H
#define GRID_H

#include <memory>
#include <vector>
#include <string>

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

    int x = 0; // The x coordinate of the cell.
    int y = 0; // The y coordinate of the cell.
    unsigned int cost = 1; // The cost of traversing the cell.
    bool selected = false; // Whether the cell is selected.
    void* searchData = nullptr; // The search data associated with the cell.
    VisualizationState vis
        = VisualizationState::UNVISITED; // The visualization state of the cell.
    std::string name = ""; // The name of the cell.
    std::string text_tr = ""; // The text in the top right of the cell.
    std::string text_bl = ""; // The text in the bottom left of the cell.
    std::string text_br = ""; // The text in the bottom right of the cell.
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

  /**
   * Get the neighbors of a cell.
   * The neighbors are the cells that are adjacent to the given cell.
   *
   * \param cell The cell to get the neighbors of.
   * \return A vector of the neighbors of the given cell.
   */
  std::vector<std::shared_ptr<Cell>> getNeighbors(std::shared_ptr<Cell> cell) const;

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