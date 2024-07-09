#include "WaitingDialog.h"
#include "../Others/DataType.h"


WaitingDialog::WaitingDialog(QWidget* parent, const bool& enableProgressBar, const bool& enableTips)
	: QDialog(parent),
	mpUi(new Ui::WaitingDialog())
{
	mpUi->setupUi(this);
	this->setWindowModality(Qt::ApplicationModal);
	this->setWindowOpacity(0.8);	// 设置透明度
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);	// 设置为对话框风格，并去掉边框
	mpUi->image->setStyleSheet("background-color: transparent;");
	mMovie = new QMovie(":/VDMocapLink/Waiting.gif");
	mpUi->image->setMovie(mMovie);
	mpUi->image->setScaledContents(true);
	mMovie->start();
	mpUi->progressBar->setVisible(enableProgressBar);
	mpUi->label_tips->setVisible(enableTips);
}

WaitingDialog::~WaitingDialog()
{
	mMovie->stop();
	delete mMovie;
}

void WaitingDialog::SetProgress(const int& progress)
{
	mpUi->progressBar->setValue(progress);
}