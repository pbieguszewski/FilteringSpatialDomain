#include "RunFilterDialog.h"

#include <QGridLayout>
#include <QTableView>
#include <QGroupBox>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidget>

RunFilterDialog::RunFilterDialog(QWidget* parent) : QDialog(parent)
{
	setWindowTitle("Set Filter");
	setFixedSize(389, 199);

	QGridLayout* mainLayout = new QGridLayout(this);
	mainLayout->setSpacing(6);
	mainLayout->setContentsMargins(11, 11, 11, 11);

	table = new QTableWidget(this);
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table->setRowCount(3);
	table->setColumnCount(3);
	mainLayout->addWidget(table, 0, 0, 3, 1);


	QGroupBox* computeModeGroup = new QGroupBox(this);
	computeModeGroup->setTitle("Compute Mode");
	QVBoxLayout* computeModeLayout = new QVBoxLayout(computeModeGroup);
	computeModeLayout->setSpacing(6);
	computeModeLayout->setContentsMargins(11, 11, 11, 11);

	cpuRB = new QRadioButton(computeModeGroup);
	cpuRB->setText("CPU");
	cpuRB->setChecked(true);
	computeModeLayout->addWidget(cpuRB);

	QRadioButton* gpuRB = new QRadioButton(computeModeGroup);
	gpuRB->setText("GPU");
	computeModeLayout->addWidget(gpuRB);
	mainLayout->addWidget(computeModeGroup, 0, 1, 1, 1);


	QGroupBox* filterDimGroup = new QGroupBox(this);
	filterDimGroup->setTitle("Filter Dimension");
	QVBoxLayout* filterDimLayout = new QVBoxLayout(filterDimGroup);
	filterDimLayout->setSpacing(6);
	filterDimLayout->setContentsMargins(11, 11, 11, 11);

	dim3RB = new QRadioButton(filterDimGroup);
	dim3RB->setText("3 x 3");
	dim3RB->setChecked(true);
	filterDimLayout->addWidget(dim3RB);

	QRadioButton* dim5RB = new QRadioButton(filterDimGroup);
	dim5RB->setText("5 x 5");
	filterDimLayout->addWidget(dim5RB);
	mainLayout->addWidget(filterDimGroup, 1, 1, 1, 1);


	QDialogButtonBox* buttons = new QDialogButtonBox(this);
	buttons->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
	mainLayout->addWidget(buttons, 2, 1, 1, 1);
	
	connect(buttons, &QDialogButtonBox::rejected, this, &RunFilterDialog::exitWithoutSave);
	connect(buttons, &QDialogButtonBox::accepted, this, &RunFilterDialog::exitWithSave);
	connect(dim3RB, &QRadioButton::clicked, [&]() { 
		if (table->rowCount() == 3)
			return;
		dim = 3;
		table->setRowCount(3);
		table->setColumnCount(3);
	});
	connect(dim5RB, &QRadioButton::clicked, [&]() {
		if (table->rowCount() == 5)
			return;
		dim = 5; 
		table->setRowCount(5);
		table->setColumnCount(5);
	});
	connect(cpuRB, &QRadioButton::clicked, [&]() { computeMode = ComputeMode::CPU; } );
	connect(gpuRB, &QRadioButton::clicked, [&]() { computeMode = ComputeMode::GPU; } );
	connect(table, &QTableWidget::itemChanged, this, &RunFilterDialog::checkCell);
}

RunFilterDialog::~RunFilterDialog() { ; }

void RunFilterDialog::exitWithoutSave()
{
	refresh();
	close();
}

void RunFilterDialog::exitWithSave()
{
	std::vector<double> vec;

	for (auto i = 0; i < table->columnCount(); i++)
	{
		for (auto j = 0; j < table->rowCount(); j++)
		{
			auto item = table->item(i, j);
			if (item == nullptr)
			{
				QMessageBox::warning(this, "Error",
					QString("(") +
					QString::number(i+1) +
					QString(", ") +
					QString::number(j+1) +
					QString("): Empty value."));
				return;
			}
			auto item2 = item->text().toDouble();
			vec.push_back(item2);
		}
	}

	emit sendValue(dim, computeMode, vec);
	refresh();
	close();
}

void RunFilterDialog::closeEvent(QCloseEvent* e)
{
	refresh();
}

void RunFilterDialog::refresh()
{
	computeMode = ComputeMode::CPU;
	dim = 3;
	cpuRB->setChecked(true);
	cpuRB->repaint();
	dim3RB->setChecked(true);
	dim3RB->repaint();
	table->clear();
	table->setRowCount(3);
	table->setColumnCount(3);
}

void RunFilterDialog::checkCell(QTableWidgetItem* evt)
{
	bool checkNum = false;

	evt->text().toDouble(&checkNum);

	if (checkNum)
		return;

	QMessageBox::warning(this, "Error", "Invalid Value");
	evt->setText(QString::number(0));
}