#include "MainWindow.h"

#include <QMenuBar>
#include <QAction>
#include <QLabel>
#include <QFileDialog>
#include <QFuture>
#include <QtConcurrent>
#include <QResizeEvent>
#include "AboutDialog.h"
#include "RunFilterDialog.h"
#include "CpuFilter.h"
#include "GpuFilter.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setMinimumSize(408, 266);
	setWindowTitle("Filtering Spatial Domain");

	QMenuBar* menu = new QMenuBar(this);
	setMenuBar(menu);

	QMenu* file = new QMenu(menu);
	file->setTitle("File");
	menu->addAction(file->menuAction());

	openImage = new QAction(file);
	openImage->setText("Open Image");
	file->addAction(openImage);

	saveImage = new QAction(file);
	saveImage->setText("Save Image");
	saveImage->setDisabled(true);
	file->addAction(saveImage);

	file->addSeparator();

	QAction* exit = new QAction(file);
	exit->setText("Exit");
	file->addAction(exit);

	run = new QAction(menu);
	run->setText("Run");
	run->setDisabled(true);
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
	connect(runFilterDialog, &RunFilterDialog::sendValue, this, &MainWindow::applyFilter);
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

	workSpace->setCursor(Qt::BusyCursor);
	workSpace->setText("");
	openImage->setDisabled(true);
	saveImage->setDisabled(true);
	run->setDisabled(true);

	QFuture<void> th = QtConcurrent::run([&](){ img.load(path); });

	while (!th.isFinished())
		QCoreApplication::processEvents();

	scaleImg(size());
	workSpace->setText("");
	openImage->setDisabled(false);
	saveImage->setDisabled(false);
	run->setDisabled(false);
	workSpace->setCursor(Qt::ArrowCursor);

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

	workSpace->setCursor(Qt::BusyCursor);
	openImage->setDisabled(true);
	saveImage->setDisabled(true);
	run->setDisabled(true);

	QFuture<void> th = QtConcurrent::run([&](){ img.save(path); });

	while (!th.isFinished())
		QCoreApplication::processEvents();

	openImage->setDisabled(false);
	saveImage->setDisabled(false);
	run->setDisabled(false);

	workSpace->setCursor(Qt::ArrowCursor);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	if (img.isNull())
		return;

	scaleImg(event->size());
}

void MainWindow::scaleImg(QSize _size)
{
	QSize sizeImg_S = img.size();
	sizeImg_S.scale(_size, Qt::KeepAspectRatio);

	QSize sizeImg_F = img.size();
	sizeImg_F.scale(sizeImg_S * 1.75, Qt::KeepAspectRatio);

	QImage scaledImg = img
		.scaled(
			sizeImg_F,
			Qt::KeepAspectRatio,
			Qt::FastTransformation)
		.scaled(
			sizeImg_S,
			Qt::KeepAspectRatio,
			Qt::SmoothTransformation);

	workSpace->setPixmap(QPixmap::fromImage(scaledImg));
}

void MainWindow::applyFilter(std::size_t dim, ComputeMode computeMode, std::vector<double>& vec)
{
	switch (computeMode)
	{
	case ComputeMode::CPU:
		img = CpuFilter()(img, dim, vec);
		break;
	case ComputeMode::GPU:
		img = GpuFilter()(img, dim, vec);
		break;
	default:
		throw;
	}
	scaleImg(size());
}