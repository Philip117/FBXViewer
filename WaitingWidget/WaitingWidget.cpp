#include <qfileinfo.h>
#include "WaitingWidget.h"


WaitingWidget::WaitingWidget(QWidget* parent)
	: QWidget(parent),
	mpUi(new Ui::WaitingWidget())
{
	mpUi->setupUi(this);
	this->setWindowOpacity(0.8);	// 透明度
	setWindowFlags(Qt::Dialog);	// 设置为去掉边框的对话框风格
	mpUi->image->setStyleSheet("background-color: transparent;");
	mMovie = new QMovie(":/FBXViewer/WaitingWidget/Waiting.gif");
	mpUi->image->setMovie(mMovie);
	mpUi->image->setScaledContents(true);
	mMovie->start();
}

WaitingWidget::~WaitingWidget()
{
	mMovie->stop();
	delete mMovie;
}
