#include <qfiledialog.h>
#include <qmessagebox.h>
#include <iostream>
#include "../Headers/FBXViewer.h"


FbxViewer::FbxViewer(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	// 新式或老式写法都不管 public/protected/private，但老式写法要求声明为 public/protected/private slots
	connect(ui.action_open, &QAction::triggered, this, &FbxViewer::OpenFbxFile);	// 新式写法
	//connect(ui.action_open, SIGNAL(triggered()), this, SLOT(OpenFbxFile()));	// 老式写法
	connect(ui.action_exit, &QAction::triggered, this, &FbxViewer::ExitFbxViewer);
}

FbxViewer::~FbxViewer()
{}

void FbxViewer::OpenFbxFile()
{
	QString lFileName = QFileDialog::getOpenFileName(this, "Open FBX File", "C:/", "*.fbx");
	//qDebug() << lFileName;
	//QMessageBox::information(nullptr, "Title", lFileName, QMessageBox::Yes);
}

void FbxViewer::ExitFbxViewer()
{
	close();
}