#include"setdataform.h"
#include<QtCore/qfile.h>
#include<QtWidgets/qstatusbar.h>
#include<QtCore/qtextstream.h>
#include<QtWidgets/qmessagebox.h>
#include<QtWidgets/qfiledialog.h>
#include<QtWidgets/qtablewidget.h>
#include<QtCore/qstringlist.h>
#include<QtWidgets/qcolordialog.h>
#include<QtCore/qdebug.h>
#include<QtGui/qicon.h>
#include<QtGui/qpainter.h>

#include<sstream>
#include"ViewFrameData.h"
#pragma execution_character_set("utf-8")

const char* brushStyleName[] ={ // brush style
	"NoBrush",
	"SolidPattern",
	"Dense1Pattern",
	"Dense2Pattern",
	"Dense3Pattern",
	"Dense4Pattern",
	"Dense5Pattern",
	"Dense6Pattern",
	"Dense7Pattern",
	"HorPattern",
	"VerPattern",
	"CrossPattern",
	"BDiagPattern",
	"FDiagPattern",
	"DiagCrossPattern",
	"LinearGradientPattern",
	"RadialGradientPattern",
	"ConicalGradientPattern",
};
void SetDataForm::setColor() {

}
void SetDataForm::setColor(int row, int col) {

}
void SetDataForm::currentChanged(int row, int col) {

}
void SetDataForm::valueChanged(int row, int col) {
}
void SetDataForm::init_table() {
	table->clear();
	QStringList colheader;
	colheader << tr("时间(ms)") <<tr("发送方")<<tr("接收方")<< tr("充电阶段") << tr("帧名") << tr("内容") ;
	table->setHorizontalHeaderLabels(colheader);
	colheader.clear();
	table->setRowCount(m_elements.size());
	table->setColumnCount(6);
	table->setAutoFillBackground(true);
	QValidator *val0 = new QDoubleValidator(table), *val2 = new QRegExpValidator(QRegExp("\\WE{0-10}$"),table);
	QStringList rowlabel;
	QStringList colors = QColor::colorNames();
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QPixmap pix = QPixmap(20, 20) ;
	for (int i = 0; i < m_elements.size(); i++) {
		rowlabel << std2qstring(m_elements[i][0]);
		QList<QTableWidgetItem*> cols= initOneRow(i, false);
		cols[0]->setText(QVariant(QVariant(std2qstring(m_elements[i][1])).toInt()/10).toString());
		if (m_elements[i].CANframe()->direct()) {
			cols[1]->setText("充电桩(C)");
			cols[2]->setText("蓄电池(B)");
		}
		else {
			cols[1]->setText("蓄电池(B)");
			cols[2]->setText("充电桩(C)");
		}
		cols[3]->setText(std2qstring(m_elements[i][2]));
		stringstream ss;
		int a = m_elements[i].CANframe()->getCANID();
		ss <<hex<< a;
		string s;
		ss >> s;
		cols[4]->setText(QString("%1(%2) - 0x%3").arg(m_elements[i].CANframe()->get_frame_name()).arg(m_elements[i].CANframe()->typeName().c_str()).arg(std2qstring(s)));
		cols[5]->setText(std2qstring(m_elements[i][3]));
		setColumnGroupData(cols, m_elements[i].CANframe()->direct());
		table->setRowHeight(i,120);
	}
	table->setVerticalHeaderLabels(rowlabel);
	table->setColumnWidth(0, table->width()*0.05);
	table->setColumnWidth(1, table->width()*0.05);
	table->setColumnWidth(2, table->width()*0.05);
	table->setColumnWidth(3, table->width()*0.1);
	table->setColumnWidth(4, table->width()*0.1);
	table->setColumnWidth(5, table->width()*0.65);
	m_changed = false;
	update();
}

void SetDataForm::accept() {

}
void SetDataForm::editAddRow() {
	InsertNumDialog * insertrow = new InsertNumDialog("Number of rows", "Insert Row");
	insertrow->setAttribute(Qt::WA_QuitOnClose, false);
	insertrow->show();
	connect(insertrow, SIGNAL(returnValue(QString)), this, SLOT(getRowNum(QString)));
}
void SetDataForm::getRowNum(QString text) {
	bool ok;
	int num = text.toInt(&ok);
	if (ok) {
		insertRow(num);
	}
	else {
		statusBar()->showMessage(QString('\"') + text + "\", is not a number",2000);
	}

}
void SetDataForm::insertRow(int row) {
}
QList<QTableWidgetItem*> SetDataForm::initOneRow(int row, bool enable) {
	QList<QTableWidgetItem*> rowitems;
	for (int i = 0; i < 6; i++) {
		rowitems.push_back(new QTableWidgetItem());
		table->setItem(row, i, rowitems[i]);
	}
	return rowitems;
}

void SetDataForm::setColumnGroupData(const QList<QTableWidgetItem*>&itemlists,bool direct) {
	QColor back;
	switch(Basic_CAN_frame::getState(itemlists[3]->text().toStdString().c_str())){
		case 0:
			back = QColor(Qt::yellow);
			break;
		case 1:
			back = QColor(Qt::cyan);
			break;
		case 2:
			back = QColor(Qt::darkGray);
			break;
		case 3:
			back = QColor(Qt::green);
			break;
		case 4:
			back = QColor(Qt::darkMagenta);
			break;
		case 5:
			back = QColor(Qt::red);
			break;
		case 6:
			back = QColor(Qt::lightGray);
			break;
		case -1:
		default:
			back = QColor(Qt::darkRed);
			break;
	}
	QBrush brush(back);
	if (direct) {
		brush.setStyle(Qt::Dense2Pattern);
	}
	else {
		brush.setStyle(Qt::SolidPattern);
	}
	for (int i = 0; i < itemlists.count(); ++i) {
		itemlists[i]->setBackground(brush);
	}
}
void SetDataForm::showItemWindow(const QModelIndex&index) {
	int row = index.row();
	stringstream ss;
	ViewFrameData* window = new ViewFrameData(this);
	window->setModal(true);
	window->setFrame(row, 0,(int) m_elements.size());
	changeFrame(row,window);
	connect(window, SIGNAL(frameChanged(int, ViewFrameData*)), this, SLOT(changeFrame(int, ViewFrameData*)));
	window->show();

}

void SetDataForm::changeFrame(int row, ViewFrameData* window) {
	stringstream ss;
	window->clearAddData();
	Basic_CAN_frame * can = m_elements[row].CANframe();
	ss << hex << setw(8) << setfill('0') << can->getCANID();
	window->setCANID(ss.str().c_str());
	window->setFrameName(can->typeName().c_str());
	window->setIndex(QString("%1").arg(can->getIndex()));
	window->setState(QString("%1").arg(can->getState()));
	table->setCurrentItem(table->currentItem(), QItemSelectionModel::Deselect | QItemSelectionModel::Columns);
	table->setCurrentItem(table->item(row, 5), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Columns);
	for (int i = 0; i < 8; ++i) {
		ss.str("");
		ss << hex << setw(2) << setfill('0') << UINT(can->getdata(i));
		window->setData(i, ss.str().c_str());
	}
	int indexadd1 = 0, indexadd2 = 0;
	for (int i = 0; i < can->detailCount(); ++i) {
		QString s = can->getDetail(i).c_str();
		QString name = can->detailName(i);
		if (s == DETAIL_BOOLS_NAME) {
			window->setAddtionStateName(name);
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
			window->setAddtionState(indexadd2++, name, state);
		}
		else {
			window->setAdditionText(indexadd1++, name + ": ", s);
		}
	}
}
void SetDataForm::ClickOnce(const QModelIndex&) {

}