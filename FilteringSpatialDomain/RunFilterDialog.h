#ifndef RUN_FILTER_DIALOG_H
#define RUN_FILTER_DIALOG_H

#include <QDialog>
#include <QTableView>
#include "ComputeMode.h"

class QRadioButton;
class QTableWidget;
class QTableWidgetItem;

class RunFilterDialog : public QDialog
{
	Q_OBJECT

public:

	RunFilterDialog(QWidget* parent = 0);
	~RunFilterDialog();

signals:

	void sendValue(std::size_t dim, ComputeMode computeMode, std::vector<double>& vec);

private:

	ComputeMode computeMode = ComputeMode::CPU;
	QTableWidget* table = nullptr;
	std::size_t dim = 3;
	QRadioButton* cpuRB = nullptr;
	QRadioButton* dim3RB = nullptr;
	
	void exitWithoutSave();
	void exitWithSave();
	void refresh();
	void checkCell(QTableWidgetItem* evt);

	void closeEvent(QCloseEvent* e);

};

#endif // RUN_FILTER_DIALOG_H