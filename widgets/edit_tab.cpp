#include "edit_tab.h"

#include "../global_state.h"
#include "../grid.h"
#include "graphics_area.h"

EditTab::EditTab(QWidget* parent)
    : QWidget(parent)
{
  // Create the layouts.
  auto layout = new QVBoxLayout(this);
  editWidget = new QWidget(this);
  auto editLayout = new QVBoxLayout(this);
  editWidget->setLayout(editLayout);

  // Create label for when a sim is running.
  noEditLabel
      = new QLabel("A simulation is currently running. Editing is forbidden.", this);
  noEditLabel->setWordWrap(true);
  noEditLabel->setVisible(false);

  // Create set cell section.
  auto setCellGroupBox = new QGroupBox("Set Cell", this);
  auto setCellLayout = new QVBoxLayout(setCellGroupBox);

  // Create set cell buttons section.
  auto setCellButtonsLayout = new QGridLayout();
  auto setEmptyButton = new QPushButton("Normal", setCellGroupBox);
  connect(setEmptyButton, &QPushButton::clicked, this, &EditTab::setEmptyButtonClicked);
  QPushButton* setWallButton = new QPushButton("Wall", setCellGroupBox);
  connect(setWallButton, &QPushButton::clicked, this, &EditTab::setWallButtonClicked);
  QPushButton* setStartButton = new QPushButton("Start", setCellGroupBox);
  connect(setStartButton, &QPushButton::clicked, this, &EditTab::setStartButtonClicked);
  QPushButton* setGoalButton = new QPushButton("Goal", setCellGroupBox);
  connect(setGoalButton, &QPushButton::clicked, this, &EditTab::setGoalButtonClicked);
  setCellButtonsLayout->addWidget(setEmptyButton, 0, 0);
  setCellButtonsLayout->addWidget(setWallButton, 0, 1);
  setCellButtonsLayout->addWidget(setStartButton, 1, 0);
  setCellButtonsLayout->addWidget(setGoalButton, 1, 1);
  setCellLayout->addLayout(setCellButtonsLayout);

  // Create set cell cost section.
  auto setCellCostLayout = new QHBoxLayout;
  auto setCellCostLabel = new QLabel("Cost:", setCellGroupBox);
  setCellCostSpinBox = new QSpinBox(setCellGroupBox);
  setCellCostSpinBox->setMinimum(GlobalState::MIN_CELL_COST);
  setCellCostSpinBox->setMaximum(GlobalState::MAX_CELL_COST);
  setCellCostLayout->addWidget(setCellCostLabel);
  setCellCostLayout->addWidget(setCellCostSpinBox);
  setCellCostLayout->addStretch();
  setCellLayout->addLayout(setCellCostLayout);

  // Add set cell section to layout.
  editLayout->addWidget(setCellGroupBox);

  // Create the reset grid section.
  auto resetGridGroupBox = new QGroupBox("Reset Grid", this);
  auto resetGridLayout = new QVBoxLayout(resetGridGroupBox);
  auto resetGridSizeLayout = new QHBoxLayout();
  resetGridWidthSpinBox = new QSpinBox(resetGridGroupBox);
  resetGridWidthSpinBox->setMinimum(2);
  resetGridWidthSpinBox->setMaximum(50);
  resetGridWidthSpinBox->setValue(GlobalState::GRID_WIDTH);
  auto resetGridSizeLabelX = new QLabel("x", resetGridGroupBox);
  resetGridHeightSpinBox = new QSpinBox(resetGridGroupBox);
  resetGridHeightSpinBox->setMinimum(2);
  resetGridHeightSpinBox->setMaximum(50);
  resetGridHeightSpinBox->setValue(GlobalState::GRID_HEIGHT);
  auto resetGridSizeLabel = new QLabel("cells", resetGridGroupBox);
  resetGridSizeLayout->addWidget(resetGridWidthSpinBox);
  resetGridSizeLayout->addWidget(resetGridSizeLabelX);
  resetGridSizeLayout->addWidget(resetGridHeightSpinBox);
  resetGridSizeLayout->addWidget(resetGridSizeLabel);
  resetGridSizeLayout->addStretch();
  resetGridLayout->addLayout(resetGridSizeLayout);
  resetGridButton = new QPushButton("Reset Grid", resetGridGroupBox);
  resetGridLayout->addWidget(resetGridButton);
  editLayout->addWidget(resetGridGroupBox);
  connect(
      resetGridButton, &QPushButton::clicked, this, &EditTab::resetGridButtonClicked);

  // Set the layout.
  layout->addWidget(noEditLabel);
  layout->addWidget(editWidget);
  layout->addStretch();
  setLayout(layout);

  // Connect slots and signals
  GlobalState& globalState = GlobalState::singleton();
  connect(&globalState, &GlobalState::simTypeChanged, this, &EditTab::simTypeSlot);
}

EditTab::~EditTab() { }

/*
 * Public slots.
 */

void EditTab::simTypeSlot(GlobalState::SimType type)
{
  if (type == GlobalState::SimType::NONE) {
    editWidget->setVisible(true);
    noEditLabel->setVisible(false);
  } else {
    editWidget->setVisible(false);
    noEditLabel->setVisible(true);
  }
}

/*
 * Private slots.
 */

void EditTab::setEmptyButtonClicked()
{
  emit setCostSelectedCells(setCellCostSpinBox->value());
}

void EditTab::setWallButtonClicked()
{
  emit setCostSelectedCells(Grid::Cell::WALL_COST);
}

void EditTab::setStartButtonClicked() { emit setStartCellSelected(); }

void EditTab::setGoalButtonClicked() { emit setGoalCellSelected(); }

void EditTab::resetGridButtonClicked()
{
  emit resetGrid(resetGridWidthSpinBox->value(), resetGridHeightSpinBox->value());
}