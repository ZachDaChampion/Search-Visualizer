#include "sim_params.h"

using SimState = GlobalState::SimState;
using SimType = GlobalState::SimType;

SimParams::SimParams(SimType simType, QWidget* parent)
    : simType(simType)
    , QWidget(parent)
{
  // Get global state.
  GlobalState& globalState = GlobalState::singleton();

  // Create other sim label.
  otherSimLabel = new QLabel("Another simulation is currently running.", this);
  otherSimLabel->setWordWrap(true);
  otherSimLabel->setVisible(false);

  // Create checkbox.
  drawCheckBox = new QCheckBox("Draw", this);
  drawCheckBox->setChecked(globalState.draw());
  connect(drawCheckBox, &QCheckBox::stateChanged, this, &SimParams::drawClicked);

  // Create slider.
  int simSpeed = globalState.simSpeed();
  speedLabel = new QLabel("Speed (" + QString::number(simSpeed) + " ticks/sec)", this);
  speedSlider = new QSlider(Qt::Horizontal, this);
  speedSlider->setRange(1, 100);
  speedSlider->setValue(simSpeed);
  connect(speedSlider, &QSlider::valueChanged, this, &SimParams::speedChanged);

  // Create buttons.
  startButton = new QPushButton("Start", this);
  connect(startButton, &QPushButton::clicked, this, &SimParams::startClicked);
  pauseButton = new QPushButton("Pause", this);
  pauseButton->setVisible(false);
  connect(pauseButton, &QPushButton::clicked, this, &SimParams::pauseClicked);
  resumeButton = new QPushButton("Resume", this);
  resumeButton->setVisible(false);
  connect(resumeButton, &QPushButton::clicked, this, &SimParams::resumeClicked);
  resetButton = new QPushButton("Reset", this);
  resetButton->setVisible(false);
  connect(resetButton, &QPushButton::clicked, this, &SimParams::resetClicked);

  // Create button layout.
  buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(startButton);
  buttonLayout->addWidget(pauseButton);
  buttonLayout->addWidget(resumeButton);
  buttonLayout->addWidget(resetButton);

  // Create layout for params widget.
  paramsLayout = new QVBoxLayout();
  paramsLayout->addWidget(drawCheckBox);
  paramsLayout->addWidget(speedLabel);
  paramsLayout->addWidget(speedSlider);
  paramsLayout->addLayout(buttonLayout);

  // Create widget to hold parameters.
  paramsWidget = new QWidget(this);
  paramsWidget->setLayout(paramsLayout);

  // Create layout for group box.
  layout = new QVBoxLayout();
  layout->addWidget(otherSimLabel);
  layout->addWidget(paramsWidget);

  // Create group box.
  groupBox = new QGroupBox("Simulation Parameters", this);
  groupBox->setLayout(layout);

  // Set layout.
  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(groupBox);
  setLayout(mainLayout);

  // Connect signals from global state.
  connect(&globalState, &GlobalState::simStateChanged, this, &SimParams::simStateSlot);
  connect(&globalState, &GlobalState::simTypeChanged, this, &SimParams::simTypeSlot);
  connect(&globalState, &GlobalState::simSpeedChanged, this, &SimParams::speedSlot);
  connect(&globalState, &GlobalState::drawChanged, this, &SimParams::drawSlot);
}

SimParams::~SimParams() { }

/*
 * Public slots.
 */

void SimParams::drawSlot(bool checked) { drawCheckBox->setChecked(checked); }

void SimParams::speedSlot(int value)
{
  speedSlider->setValue(value);
  speedLabel->setText("Speed (" + QString::number(value) + " ticks/sec)");
}

void SimParams::simStateSlot(SimState state)
{
  GlobalState& globalState = GlobalState::singleton();
  updateLayoutSimType(globalState.simType());
  updateLayoutSimState(state);
}

void SimParams::simTypeSlot(GlobalState::SimType type) { updateLayoutSimType(type); }

/*
 * Private slots.
 */

void SimParams::drawClicked(int state)
{
  GlobalState& globalState = GlobalState::singleton();
  globalState.setDraw(state == Qt::Checked);
}

void SimParams::speedChanged(int value)
{
  GlobalState& globalState = GlobalState::singleton();
  globalState.setSimSpeed(value);
}

void SimParams::startClicked()
{
  GlobalState& globalState = GlobalState::singleton();
  globalState.setSimState(SimState::RUNNING);
  globalState.setSimType(simType);
}

void SimParams::pauseClicked()
{
  GlobalState& globalState = GlobalState::singleton();
  globalState.setSimState(SimState::PAUSED);
}

void SimParams::resumeClicked()
{
  GlobalState& globalState = GlobalState::singleton();
  globalState.setSimState(SimState::RUNNING);
}

void SimParams::resetClicked()
{
  GlobalState& globalState = GlobalState::singleton();
  globalState.setSimState(SimState::STOPPED);
  globalState.setSimType(SimType::NONE);
}

/*
 * Private.
 */

void SimParams::updateLayoutSimState(GlobalState::SimState state)
{
  // Update button visibility based on simulation state.
  switch (state) {
  case SimState::RUNNING:
    startButton->setVisible(false);
    pauseButton->setVisible(true);
    resumeButton->setVisible(false);
    resetButton->setVisible(true);
    break;
  case SimState::PAUSED:
    startButton->setVisible(false);
    pauseButton->setVisible(false);
    resumeButton->setVisible(true);
    resetButton->setVisible(true);
    break;
  case SimState::STOPPED:
    startButton->setVisible(true);
    pauseButton->setVisible(false);
    resumeButton->setVisible(false);
    resetButton->setVisible(false);
    break;
  }
}

void SimParams::updateLayoutSimType(GlobalState::SimType type)
{
  /*
   * The parameters should only be visible if the currently running simulation is
   * the same type as the one controlled by the parameters, or if no simulation is
   * running.
   *
   * If a different simulation is running, the parameters should be hidden.
   */

  GlobalState& globalState = GlobalState::singleton();

  if (globalState.simType() == simType
      || globalState.simType() == GlobalState::SimType::NONE) {
    paramsWidget->setVisible(true);
    otherSimLabel->setVisible(false);
  } else {
    paramsWidget->setVisible(false);
    otherSimLabel->setVisible(true);
  }
}