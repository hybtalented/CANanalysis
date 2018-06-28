#include "ViewFrameData.h"
#include <QtWidgets/qmessagebox.h>
#include<QtCore/qregexp.h>
#include<CANdata.h>
#include<QtWidgets/qcheckbox.h>
#pragma execution_character_set("utf-8")
ViewFrameData::ViewFrameData(QWidget *parent)
	: QDialog(parent),frame(-1),max(-1),min(-1)
{
	ui.setupUi(this);
	connect(ui.frame, SIGNAL(valueChanged(int)), this, SLOT(changeFrame(int)));
	connect(ui.next, SIGNAL(clicked(bool)), this, SLOT(frameAfter()) );
	connect(ui.before, SIGNAL(clicked(bool)), this, SLOT(frameBefore()));
}
void ViewFrameData::setData(int i, const QString&s) {
	switch (i) {
	case 0:
		ui.data1->setText(s);
		break;
	case 1:
		ui.data2->setText(s);
		break;
	case 2:
		ui.data3->setText(s);
		break;
	case 3:
		ui.data4->setText(s);
		break;
	case 4:
		ui.data5->setText(s);
		break;
	case 5:
		ui.data6->setText(s);
		break;
	case 6:
		ui.data7->setText(s);
		break;
	case 7:
		ui.data8->setText(s);
		break;
	}
}

void ViewFrameData::setAdditionText(int index, const QString & label, const QString & text)
{
	QLabel* tlabel = new QLabel;
	QLineEdit * ttext = new QLineEdit;
	adds.push_back(tlabel);
	adds.push_back(ttext);
	ttext->setReadOnly(true);
	ttext->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
	tlabel->setText(label);
	ttext->setText(text);
	int row = index / 3;
	int col = index % 3;
	ui.gridLayout->addWidget(tlabel, row, col * 2, 1, 1, Qt::AlignLeft | Qt::AlignHCenter);
	ui.gridLayout->addWidget(ttext, row, col * 2 + 1, 1, 1, Qt::AlignCenter | Qt::AlignHCenter);
}

void ViewFrameData::setAddtionState(int index, const QString & label, int state)
{
	QCheckBox * box = new QCheckBox(label);
	adds.push_back(box);
	switch (state) {
	case 0:
		box->setCheckState(Qt::Unchecked);
		break;
	case 1:
		box->setCheckState(Qt::Checked);
		break;
	case -1:
		box->setCheckState(Qt::PartiallyChecked);
	}
	box->setDisabled(true);
	int row = index / 4;
	int col = index % 4;
	ui.gridLayout_2->addWidget(box, row, col, 1, 1, Qt::AlignLeft);
}
void ViewFrameData::clearAddData() {
	for (int i = 0; i < adds.count(); ++i) {
		delete adds[i];
	}
	adds.clear();
}