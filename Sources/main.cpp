#include "../Headers/FBXViewer.h"
#include <QtWidgets/QApplication>


int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	FbxViewer w;
	w.show();
	return a.exec();
}
