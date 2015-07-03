#ifndef RUN_FILTER_DIALOG_H
#define RUN_FILTER_DIALOG_H

#include <QDialog>
#include <QTableView>
#include "ComputeMode.h"

class QStandardItemModel;

class RunFilterDialog : public QDialog
{
	Q_OBJECT

public:

	RunFilterDialog(QWidget *parent = 0);
	~RunFilterDialog();

private:

	ComputeMode computeMode = ComputeMode::CPU;
	QStandardItemModel* filterValue = nullptr;
	std::size_t dim = 3;

};

#endif // RUN_FILTER_DIALOG_H