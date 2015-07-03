#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <QDialog>

class AboutDialog final : public QDialog
{
	Q_OBJECT

public:

	AboutDialog(QWidget *parent = 0);
	~AboutDialog();

private:



};

#endif // ABOUT_DIALOG_H