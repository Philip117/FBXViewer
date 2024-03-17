#include <qfiledialog.h>
#include <qmessagebox.h>
#include <iostream>
#include "../Headers/FBXViewer.h"


FbxViewer::FbxViewer(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	// ��ʽ����ʽд�������� public/protected/private������ʽд��Ҫ������Ϊ public/protected/private slots
	connect(ui.action_open, &QAction::triggered, this, &FbxViewer::OpenFbxFile);	// ��ʽд��
	//connect(ui.action_open, SIGNAL(triggered()), this, SLOT(OpenFbxFile()));	// ��ʽд��
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