#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QImage>
#include "ComputeMode.h"

class QLabel;

class MainWindow final : public QMainWindow
{
	Q_OBJECT

public:

	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:

	QLabel* workSpace = nullptr;
	QImage img;

	QAction* openImage = nullptr;
	QAction* saveImage = nullptr;
	QAction* run = nullptr;

	void open();
	void save();
	void resizeEvent(QResizeEvent *event);
	void scaleImg(QSize _size);
	void applyFilter(std::size_t dim, ComputeMode computeMode, std::vector<double>& vec);

};

#endif // MAIN_WINDOW_H