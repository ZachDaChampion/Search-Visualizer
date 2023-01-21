#ifndef SIM_PARAMS_H
#define SIM_PARAMS_H

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

#include "../global_state.h"

class SimParams : public QWidget {
  Q_OBJECT

  public slots:

  /**
   * Update the draw check box.
   *
   * \param checked Whether or not the check box is checked.
   */
  void drawSlot(bool checked);

  /**
   * Update the value of the speed slider.
   *
   * \param value The value of the speed slider.
   */
  void speedSlot(int value);

  /**
   * Update the simulation state.
   *
   * \param state The simulation state.
   */
  void simStateSlot(GlobalState::SimState state);

  /**
   * Update the simulation type.
   *
   * \param type The simulation type.
   */
  void simTypeSlot(GlobalState::SimType type);

  private slots:

  /**
   * Draw check box clicked.
   *
   * \param state Checkbox state.
   */
  void drawClicked(int state);

  /**
   * Speed slider value changed.
   *
   * \param value The value of the speed slider.
   */
  void speedChanged(int value);

  /**
   * Speed slider released.
   */
  void speedReleased();

  /**
   * Start button clicked.
   */
  void startClicked();

  /**
   * Pause button clicked.
   */
  void pauseClicked();

  /**
   * Resume button clicked.
   */
  void resumeClicked();

  /**
   * Reset button clicked.
   */
  void resetClicked();

  public:

  /**
   * Construct a new Sim Params.
   *
   * \param simType The type of simulation that the parameters are for.
   * \param parent The parent widget.
   */
  SimParams(GlobalState::SimType simType, QWidget* parent = nullptr);

  /**
   * Destroy a Sim Params.
   */
  ~SimParams();

  private:

  /**
   * Update the layout for the simulation state.
   *
   * \param state The simulation state.
   */
  void updateLayoutSimState(GlobalState::SimState state);

  /**
   * Update the layout for the simulation type.
   *
   * \param type The simulation type.
   */
  void updateLayoutSimType(GlobalState::SimType type);

  /*
   * Data.
   */

  GlobalState::SimType simType; // The type of simulation that the parameters are for.
  int simSpeed; // The speed of the simulation.

  /*
   * Widgets.
   */

  QGroupBox* groupBox; // The group box for the simulation parameters.
  QVBoxLayout* layout; // The layout of the groupbox.
  QLabel* otherSimLabel; // The label to display when a different simulation is running.
  QWidget* paramsWidget; // The widget for the simulation parameters.
  QVBoxLayout* paramsLayout; // The layout for the simulation parameters.
  QCheckBox* drawCheckBox; // Whether or not to draw a running simulation.
  QLabel* speedLabel; // The label for the simulation speed.
  QSlider* speedSlider; // The slider for the simulation speed.
  QHBoxLayout* buttonLayout; // The layout for the simulation buttons.
  QPushButton* startButton; // The button to start the simulation.
  QPushButton* pauseButton; // The button to stop the simulation.
  QPushButton* resumeButton; // The button to resume the simulation.
  QPushButton* resetButton; // The button to reset the simulation.
};

#endif // SIM_PARAMS_H