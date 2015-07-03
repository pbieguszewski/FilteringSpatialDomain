#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class QLabel;

class MainWindow final : public QMainWindow
{
	Q_OBJECT

public:

	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:

	QLabel* workSpace = nullptr;

	void open();
	void save();

};

#endif // MAIN_WINDOW_H