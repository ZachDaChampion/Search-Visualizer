#ifndef EDIT_TAB_H
#define EDIT_TAB_H

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "../global_state.h"

class EditTab : public QWidget {
  Q_OBJECT

  private slots:

  /**
   * "Set cell to empty" button clicked.
   */
  void setEmptyButtonClicked();

  /**
   * "Set cell to wall" button clicked.
   */
  void setWallButtonClicked();

  /**
   * "Set cell to start" button clicked.
   */
  void setStartButtonClicked();

  /**
   * "Set cell to goal" button clicked.
   */
  void setGoalButtonClicked();

  /**
   * "Reset grid" button clicked.
   */
  void resetGridButtonClicked();

  /**
   * Update widget visibility based on simulation type.
   * If there is no current simulation, edit controls will be show.
   * Otherwise they will be hidden.
   *
   * \param type The simulation type.
   */
  void simTypeSlot(GlobalState::SimType type);

  public:
  /**
   * Construct a new Edit Tab.
   *
   * \param parent The parent widget.
   */
  EditTab(QWidget* parent = nullptr);

  /**
   * Destroy an Edit Tab.
   */
  ~EditTab();

  signals:

  /**
   * Signal that the selected cells should have their cost set.
   */
  void setCostSelectedCells(int cost);

  /**
   * Signal that the selected cells should be set as the start cell.
   */
  void setStartCellSelected();

  /**
   * Signal that the selected cells should be set as the goal cell.
   */
  void setGoalCellSelected();

  /**
   * Signal that the grid should be reset.
   * 
   * \param width Width of the grid.
   * \param height Height of the grid.
   */
  void resetGrid(int width, int height);

  private:

  /*
   * Widgets in the edit tab.
   */
  QWidget* editWidget; // Container widget for all edit controls.
  QLabel* noEditLabel; // Label to be shown when editing is disabled.

  /*
   * Widgets for set cell section.
   */

  QSpinBox* setCellCostSpinBox; // The spin box for the cost of the cell.
  QPushButton* setEmptyButton; // The button for setting an empty cell.
  QPushButton* setWallButton; // The button for setting a wall cell.
  QPushButton* setStartButton; // The button for setting the start cell.
  QPushButton* setGoalButton; // The button for setting the goal cell.

  /*
   * Widgets for grid reset section.
   */

  // Resize grid section.
  QSpinBox* resetGridWidthSpinBox; // The spin box for the width of the grid.
  QSpinBox* resetGridHeightSpinBox; // The spin box for the height of the grid.

  // Reset grid button.
  QPushButton* resetGridButton; // The button for resetting the grid.
};

#endif // EDIT_TAB_H