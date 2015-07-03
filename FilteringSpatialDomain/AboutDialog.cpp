#include "AboutDialog.h"

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent)
{
	setWindowTitle("About");
	setFixedSize(200, 100);
}

AboutDialog::~AboutDialog() { ; }