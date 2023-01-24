#include "edit_tab.h"

#include "../global_state.h"
#include "../grid.h"
#include "graphics_area.h"

EditTab::EditTab(QWidget* parent)
    : QWidget(parent)
{
  // Create the layout.
  layout = new QVBoxLayout(this);

  // Create set cell section.
  QGroupBox* setCellGroupBox = new QGroupBox("Set Cell", this);
  QVBoxLayout* setCellLayout = new QVBoxLayout(setCellGroupBox);

  // Create set cell buttons section.
  QGridLayout* setCellButtonsLayout = new QGridLayout();
  QPushButton* setEmptyButton = new QPushButton("Cost", setCellGroupBox);
  QObject::connect(
      setEmptyButton, &QPushButton::clicked, this, &EditTab::setEmptyButtonClicked);
  QPushButton* setWallButton = new QPushButton("Wall", setCellGroupBox);
  QObject::connect(
      setWallButton, &QPushButton::clicked, this, &EditTab::setWallButtonClicked);
  QPushButton* setStartButton = new QPushButton("Start", setCellGroupBox);
  QObject::connect(
      setStartButton, &QPushButton::clicked, this, &EditTab::setStartButtonClicked);
  QPushButton* setGoalButton = new QPushButton("Goal", setCellGroupBox);
  QObject::connect(
      setGoalButton, &QPushButton::clicked, this, &EditTab::setGoalButtonClicked);
  setCellButtonsLayout->addWidget(setEmptyButton, 0, 0);
  setCellButtonsLayout->addWidget(setWallButton, 0, 1);
  setCellButtonsLayout->addWidget(setStartButton, 1, 0);
  setCellButtonsLayout->addWidget(setGoalButton, 1, 1);
  setCellLayout->addLayout(setCellButtonsLayout);

  // Create set cell cost section.
  QHBoxLayout* setCellCostLayout = new QHBoxLayout();
  QLabel* setCellCostLabel = new QLabel("Cost:", setCellGroupBox);
  setCellCostSpinBox = new QSpinBox(setCellGroupBox);
  setCellCostSpinBox->setMinimum(GlobalState::MIN_CELL_COST);
  setCellCostSpinBox->setMaximum(GlobalState::MAX_CELL_COST);
  setCellCostLayout->addWidget(setCellCostLabel);
  setCellCostLayout->addWidget(setCellCostSpinBox);
  setCellCostLayout->addStretch();
  setCellLayout->addLayout(setCellCostLayout);

  // Add set cell section to layout.
  layout->addWidget(setCellGroupBox);

  // Create the reset grid section.
  resetGridGroupBox = new QGroupBox("Reset Grid", this);
  resetGridLayout = new QVBoxLayout(resetGridGroupBox);
  resetGridSizeLayout = new QHBoxLayout();
  resetGridWidthSpinBox = new QSpinBox(resetGridGroupBox);
  resetGridWidthSpinBox->setMinimum(1);
  resetGridWidthSpinBox->setMaximum(100);
  resetGridSizeLabelX = new QLabel("x", resetGridGroupBox);
  resetGridHeightSpinBox = new QSpinBox(resetGridGroupBox);
  resetGridHeightSpinBox->setMinimum(1);
  resetGridHeightSpinBox->setMaximum(100);
  resetGridSizeLabel = new QLabel("cells", resetGridGroupBox);
  resetGridSizeLayout->addWidget(resetGridWidthSpinBox);
  resetGridSizeLayout->addWidget(resetGridSizeLabelX);
  resetGridSizeLayout->addWidget(resetGridHeightSpinBox);
  resetGridSizeLayout->addWidget(resetGridSizeLabel);
  resetGridSizeLayout->addStretch();
  resetGridLayout->addLayout(resetGridSizeLayout);
  resetGridButton = new QPushButton("Reset Grid", resetGridGroupBox);
  resetGridLayout->addWidget(resetGridButton);
  layout->addWidget(resetGridGroupBox);

  // Set the layout.
  layout->addStretch();
  setLayout(layout);
}

EditTab::~EditTab() { }

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