#include "RunFilterDialog.h"

#include <QGridLayout>
#include <QTableView>
#include <QGroupBox>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QStandardItemModel>

RunFilterDialog::RunFilterDialog(QWidget* parent) : QDialog(parent)
{
	setWindowTitle("Set Filter");
	setFixedSize(389, 199);

	QGridLayout* mainLayout = new QGridLayout(this);
	mainLayout->setSpacing(6);
	mainLayout->setContentsMargins(11, 11, 11, 11);

	QTableView* table = new QTableView(this);
	filterValue = new QStandardItemModel(this);
	filterValue->setRowCount(3);
	filterValue->setColumnCount(3);
	table->setModel(filterValue);
	mainLayout->addWidget(table, 0, 0, 3, 1);


	QGroupBox* computeModeGroup = new QGroupBox(this);
	computeModeGroup->setTitle("Compute Mode");
	QVBoxLayout* computeModeLayout = new QVBoxLayout(computeModeGroup);
	computeModeLayout->setSpacing(6);
	computeModeLayout->setContentsMargins(11, 11, 11, 11);

	QRadioButton* cpuRB = new QRadioButton(computeModeGroup);
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

	QRadioButton* dim3RB = new QRadioButton(filterDimGroup);
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
	

	connect(buttons, &QDialogButtonBox::rejected, [&]() {
		computeMode = ComputeMode::CPU;
		dim = 3;
		//cpuRB->setChecked(true); ?
		//dim3RB->setChecked(true); ?
		close();
	});
	connect(dim3RB, &QRadioButton::clicked, [&]() { dim = 3; } );
	connect(dim5RB, &QRadioButton::clicked, [&]() { dim = 5; } );
	connect(cpuRB, &QRadioButton::clicked, [&]() { computeMode = ComputeMode::CPU; } );
	connect(gpuRB, &QRadioButton::clicked, [&]() { computeMode = ComputeMode::GPU; } );
}

RunFilterDialog::~RunFilterDialog() { ; }
