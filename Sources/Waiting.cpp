#include <qfileinfo.h>
#include "../Headers/Waiting.h"


Waiting::Waiting(QWidget* parent)
	: QWidget(parent)
{
	mUi.setupUi(this);
	this->setWindowOpacity(0.8);	// ͸����
	setWindowFlags(Qt::Dialog);	// ����Ϊȥ���߿�ĶԻ�����
	setWindowModality(Qt::WindowModal);
	mUi.image->setStyleSheet("background-color: transparent;");
	mMovie = new QMovie("Forms/Waiting.gif");
	mUi.image->setMovie(mMovie);
	mUi.image->setScaledContents(true);
	mMovie->start();
}

Waiting::~Waiting()
{
	mMovie->stop();
	delete mMovie;
}
