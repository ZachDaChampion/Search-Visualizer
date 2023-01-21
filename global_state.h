#ifndef APPLICATION_STATE_H
#define APPLICATION_STATE_H

#include <QObject>

class GlobalState : public QObject {
  Q_OBJECT

  public:

  /**
   * A state that the simulation can be in.
   */
  enum class SimState {
    STOPPED, // The simulation is stopped.
    RUNNING, // The simulation is running.
    PAUSED // The simulation is paused.
  };

  /**
   * A type of simulation.
   */
  enum class SimType {
    NONE, // No simulation type.
    BFS, // Breadth-first search.
    DFS, // Depth-first search.
    DIJKSTRA, // Dijkstra's algorithm.
    ASTAR // A* algorithm.
  };

  signals:

  /**
   * Emit a signal to update the simulation state.
   *
   * \param state The simulation state.
   */
  void simStateChanged(SimState state);

  /**
   * Emit a signal to update the simulation type.
   *
   * \param type The simulation type.
   */
  void simTypeChanged(SimType type);

  /**
   * Emit a signal to update the simulation speed.
   *
   * \param speed The simulation speed.
   */
  void simSpeedChanged(int speed);

  /**
   * Emit a signal to update whether or not to draw the simulation.
   *
   * \param draw Whether or not to draw the simulation.
   */
  void drawChanged(bool draw);

  public:

  /**
   * Get the singleton instance of the global state.
   *
   * \return The singleton instance of the global state.
   */
  static GlobalState& singleton();

  /**
   * Construct a new Global State.
   */
  GlobalState();

  /**
   * Don't allow the global state to be copied.
   */
  GlobalState(GlobalState const&) = delete;

  /**
   * Don't allow the global state to be assigned.
   */
  void operator=(GlobalState const&) = delete;

  /*
   * Getters.
   */

  /**
   * Get the simulation state.
   *
   * \return The simulation state.
   */
  SimState simState() const;

  /**
   * Get the simulation type.
   *
   * \return The simulation type.
   */
  SimType simType() const;

  /**
   * Get the simulation speed.
   *
   * \return The simulation speed.
   */
  int simSpeed() const;

  /**
   * Get whether or not to draw the simulation.
   *
   * \return Whether or not to draw the simulation.
   */
  bool draw() const;

  /*
   * Setters.
   */

  /**
   * Set the simulation state.
   *
   * \param state The simulation state.
   */
  void setSimState(SimState state);

  /**
   * Set the simulation type.
   *
   * \param type The simulation type.
   */
  void setSimType(SimType type);

  /**
   * Set the simulation speed.
   *
   * \param speed The simulation speed.
   */
  void setSimSpeed(int speed);

  /**
   * Set whether or not to draw the simulation.
   *
   * \param draw Whether or not to draw the simulation.
   */
  void setDraw(bool draw);

  private:

  /*
   * Data.
   */

  SimState simState_ = SimState::STOPPED; // The simulation state.
  SimType simType_ = SimType::NONE; // The simulation type.
  int simSpeed_ = 1; // The simulation speed.
  bool draw_ = true; // Whether or not to draw the simulation.
};

#endif // APPLICATION_STATE_H