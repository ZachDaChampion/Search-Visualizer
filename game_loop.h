#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <QObject>
#include <QTimer>

#include "../global_state.h"

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
   * Change the speed of the game loop.
   *
   * \param speed The speed of the game loop (ticks/second).
   */
  void setLoopSpeed(int speed);

  signals:

  /**
   * Emit a signal that the game loop has ticked.
   */
  void tick();

  private:

  /*
   * Data.
   */

  QTimer timer_;
};

#endif