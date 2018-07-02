#include "ViewMultiFrame.h"
#include <QtWidgets/qmessagebox.h>
#include<QtCore/qregexp.h>
#include<CANdata.h>
#include<QtWidgets/qcheckbox.h>
#include<CANdata.h>
#include"setdataform.h"
#pragma execution_character_set("utf-8")
ViewMultiFrame::ViewMultiFrame(QWidget *parent)
	: QDialog(parent),frame(-1),max(-1),min(-1)
{
	ui.setupUi(this);
	connect(ui.frame, SIGNAL(valueChanged(int)), this, SLOT(changeFrame(int)));
	connect(ui.next, SIGNAL(clicked(bool)), this, SLOT(frameAfter()) );
	connect(ui.before, SIGNAL(clicked(bool)), this, SLOT(frameBefore()));
}

void ViewMultiFrame::setAdditionText(int index, const QString & label, const QString & text)
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

void ViewMultiFrame::setFrame(int row, Multi_frame* frame) {
	ui.framename->setText(frame->get_frame_name());
	ui.state->setText(frame->getState());
	ui.Index->setText(QString("%1").arg(frame->frame(0)->getIndex()));
	ui.canid->setText(QString("%1").arg(frame->getPGN()));
	QStringList colheader, rowlabel;
	colheader << tr("时间(ms)") << tr("发送方") << tr("帧名") << tr("内容");

	colheader.clear();
	ui.tableWidget->setRowCount(frame->frameCount());
	ui.tableWidget->setColumnCount(4);
	ui.tableWidget->setHorizontalHeaderLabels(colheader);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	for (int i = 0; i < frame->frameCount(); ++i) {
		rowlabel << QString("%1").arg(i);
		QList<QTableWidgetItem*> cols;
		for (int j = 0; j < 4; ++j) {
			cols.push_back(new QTableWidgetItem());
			ui.tableWidget->setItem(i, j, cols[j]);
		}
		cols[0]->setText(QVariant(frame->frame(i)->getTime() / 10).toString());
		if (frame->frame(i)->direct()) {
			cols[1]->setText("充电桩(C)");
		}
		else {
			cols[1]->setText("蓄电池(B)");
		}
		stringstream ss;
		int a = frame->getPGN();
		ss << hex << a;
		string s;
		ss >> s;
		cols[2]->setText(QString("%1(%2)").arg(frame->get_frame_name()).arg(frame->typeName().c_str()));
		cols[3]->setText(std2qstring(frame->frame(i)->get_detail()));
		ui.tableWidget->setRowHeight(i, 50);
	}
	for (int i = 0,indexadd1 = 0, indexadd2 = 0; i < frame->detailCount(); ++i) {
		QString s = frame->getDetail(i).c_str();
		QString name = frame->detailName(i);
		if (s == DETAIL_BOOLS_NAME) {
			setAddtionStateName(name);
		}
		else if (s == DETAIL_TRUE || s == DETAIL_FALSE || s == DETAIL_UNKNOW) {
			int state;
			if (s == DETAIL_TRUE) {
				state = 1;
			}
			else if (s == DETAIL_FALSE) {
				state = 0;
			}
			else {
				state = -1;
			}
			setAddtionState(indexadd2++, name, state);
		}
		else {
			setAdditionText(indexadd1++, name + ": ", s);
		}
	}
}
void ViewMultiFrame::setAddtionState(int index, const QString & label, int state)
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
void ViewMultiFrame::clearAddData() {
	for (int i = 0; i < adds.count(); ++i) {
		delete adds[i];
	}
	adds.clear();
}