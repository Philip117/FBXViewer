﻿#include <QtWidgets/QApplication>
#include "MainWindow/MainWindow.h"


int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
