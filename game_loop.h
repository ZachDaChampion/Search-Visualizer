#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <QObject>
#include <QTimer>
#include <memory>

#include "algorithms/search_algorithm.h"
#include "global_state.h"

class GameLoop : public QObject {
  Q_OBJECT

  public:

  /**
   * Get the singleton instance of the Game Loop.
   *
   * \return The singleton instance of the Game Loop.
   */
  static GameLoop& singleton();

  /**
   * Construct a new Game Loop.
   */
  GameLoop();

  /**
   * Don't allow the Game Loop to be copied.
   */
  GameLoop(GameLoop const&) = delete;

  /**
   * Don't allow the Game Loop to be assigned.
   */
  void operator=(GameLoop const&) = delete;

  public slots:

  /**
   * Start or resume the game loop.
   */
  void onSimStateChanged(GlobalState::SimState state);

  /**
   * Change the sim type.
   */
  void setSim(std::shared_ptr<SearchAlgorithms::SearchAlgorithm> searchAlgorithm);

  /**
   * Change the speed of the game loop.
   *
   * \param speed The speed of the game loop (ticks/second).
   */
  void setLoopSpeed(int speed);

  private slots:

  /**
   * The game loop tick.
   */
  void localTick();

  signals:

  /**
   * Emit a signal that the game loop has ticked.
   */
  void tick();

  /**
   * Emit a signal that the graphics need to be updated.
   */
  void updateGraphics(std::vector<std::shared_ptr<Grid::Cell>> cells);

  private:

  /*
   * Data.
   */

  std::shared_ptr<SearchAlgorithms::SearchAlgorithm> searchAlgorithm_
      = nullptr; // The search algorithm to use.

  QTimer timer_; // The timer that drives the game loop.
};

#endif