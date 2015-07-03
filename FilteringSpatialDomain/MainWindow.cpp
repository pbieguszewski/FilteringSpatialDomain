#include "MainWindow.h"

#include <QMenuBar>
#include <QAction>
#include <QLabel>
#include <QFileDialog>
#include "AboutDialog.h"
#include "RunFilterDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setMinimumSize(408, 266);
	setWindowTitle("Filtering Spatial Domain");

	QMenuBar* menu = new QMenuBar(this);
	setMenuBar(menu);

	QMenu* file = new QMenu(menu);
	file->setTitle("File");
	menu->addAction(file->menuAction());

	QAction* openImage = new QAction(file);
	openImage->setText("Open Image");
	file->addAction(openImage);

	QAction* saveImage = new QAction(file);
	saveImage->setText("Save Image");
	saveImage->setDisabled(true);
	file->addAction(saveImage);

	file->addSeparator();

	QAction* exit = new QAction(file);
	exit->setText("Exit");
	file->addAction(exit);

	QAction* run = new QAction(menu);
	run->setText("Run");
	//run->setDisabled(true);
	menu->addAction(run);

	QAction* about = new QAction(menu);
	about->setText("About");
	menu->addAction(about);

	workSpace = new QLabel(this);
	workSpace->setFont(QFont("Arial", 10, QFont::Bold));
	workSpace->setAlignment(Qt::AlignCenter);
	workSpace->setText("Please, load image.");
	setCentralWidget(workSpace);

	AboutDialog* aboutDialog = new AboutDialog(this);
	RunFilterDialog* runFilterDialog = new RunFilterDialog(this);

	connect(openImage, &QAction::triggered, this, &MainWindow::open);
	connect(saveImage, &QAction::triggered, this, &MainWindow::save);
	connect(exit, &QAction::triggered, this, &MainWindow::close);
	connect(about, &QAction::triggered, aboutDialog, &RunFilterDialog::exec);
	connect(run, &QAction::triggered, runFilterDialog, &RunFilterDialog::exec);

}

MainWindow::~MainWindow() { ; }

void MainWindow::open()
{
	QString path = QFileDialog::getOpenFileName(
		this,
		tr("Open Image"),
		QDir::currentPath(),
		"BMP (*.bmp);;"
		"JPEG (*.jpg; *.jpeg);"
		";PNG (*.png)");

	if (path.isNull())
		return;
}

void MainWindow::save()
{
	QString path = QFileDialog::getSaveFileName(
		this,
		tr("Save Image"),
		QDir::currentPath(),
		"BMP (*.bmp);;"
		"JPEG (*.jpg; *.jpeg);;"
		"PNG (*.png)");

	if (path.isEmpty())
		return;
}