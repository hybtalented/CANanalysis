#ifndef SETDATAFORM_H
#define SETDATAFORM_H
#include <QtWidgets/qmainwindow.h>
#include<QtWidgets/qtablewidget.h>
#include<QtWidgets/qdialog.h>
#include "chartform.h"
#include"canelement.h"
#include<QtWidgets/qcombobox.h>
#include<QtWidgets/qlineedit.h>
#include<QtCore/qtimer.h>
class QPushButton;
class QTableWidget;
class ChartForm;
class QActionGroup;
class ViewFrameData;
class ViewMultiFrame;
class ClickTimer;
class ItemRow {
public:
	enum ItemType { Multiframe, Singleframe } type;
	ItemRow(ItemType t= Multiframe, int r=0) :type(t), row(r) {}
	int row;

};
class SetDataForm :public QMainWindow
{
	Q_OBJECT
		friend class ChartForm;
public:
	void insertRow(int row);
	SetDataForm(const QString&filename, int decimalPaces, QWidget*parent=0, const char* name = "set data form", bool modal = true, Qt::WindowFlags f = 0);
	~SetDataForm();
public slots:
	void setColor();
	void setColor(int row, int col);
	void currentChanged(int row, int col);
	void valueChanged(int row, int col);
	void load(const QString&filename);
	void chartformclose() { chartform = 0; }
	bool okToClear();
protected slots:
	void accept();
	void showItemWindow(const QModelIndex&index);
	void ClickOnce(const QModelIndex&);
	void changeFrame(int i, ViewFrameData* view);
	void changeFrame(int i, ViewMultiFrame* view);
public slots:
	void fileNew();
	void fileOpen();
	void fileOpenRecent(int index);
	void updateChartType(QAction*);
	void fileSave();
	void fileSaveAs();
	void editAddRow();
	//void filePrint();
	void fileQuit();
	CANVector& elementvector() { return m_elements; }
	/*void optionsSetData();
	void optionsSetFont();
	void optionsSetoptions();
	void helpHelp();
	void helpAbout();
	void helpAboutQt();*/
	void saveOptions();
private slots:
	void getRowNum(QString);
	void setShowMultiFrame(bool i) {
		m_showMultiFrame = i;
		init_table();
	}
private:
	QTableWidget * table;
	QList<QTableWidgetItem*> initOneRow(int row,bool enable = true);
	QPushButton* colorPushButton,*okPushButton, *cancelPushButton;
	QMenu *fileMenu;
	QMenu *optionMenu;
	QMenu *editMenu;
	QActionGroup* chartGroup;
	QAction * optionsPieChartAction;
	QAction * optionsHorizontalBarChartAction;
	QAction * optoinsVerticalBarChartAction;
	ClickTimer * timer;
private:
	void updateRecentFiles(const QString&filename);
	void updateRecentFilesMenu();
	void setColumnGroupData(const QList<QTableWidgetItem*>&,bool direct=true);
	enum { MAX_RECENTFILES = 9 };
	void init_multiFrame();
	void delete_elementdata(){
		for (CANVector::iterator it = m_elements.begin(); it != m_elements.end(); ++it) {
			delete (*it).CANframe();
		}
		m_multiFrame.clear();
		m_elementMap.clear();
		m_elements.clear();
	}
	void init_table();
	void init_menutoolbar();
	void init();
	bool m_showMultiFrame;
	bool m_changed;
	QVector<Multi_frame*>m_multiFrame;
	QVector<ItemRow> m_elementMap;
	CANVector m_elements;
	QString m_filename;
	ChartForm * chartform;
	int m_decimalPaces;
	QStringList m_recentFiles;
};

#define std2qstring(a) QString((a).c_str())

class TableItem: public QObject {
	Q_OBJECT
public:
	TableItem(int r, int c,QTableWidget* table):col(c),row(r){
		QObject::connect(this, SIGNAL(ValueChanged(int, int)), table, SIGNAL(cellChanged(int, int)));
	}
	~TableItem(){}
public slots:
	void ChangeValue( int a= 0) {
		emit ValueChanged(row, col);
	}
	void ChangeValue(const QString&) {
		ChangeValue();
	}
signals:
	void ValueChanged(int , int);
private:
	int col, row;
};
class ComboBoxItem :  public QComboBox {
	Q_OBJECT
public:
	ComboBoxItem(int row, int col, QTableWidget *parent) :QComboBox(parent),place(row, col,parent) {
	}
	void enable() {
		connect(this, SIGNAL(currentIndexChanged(int)), &place, SLOT(ChangeValue(int)));
	}
private:
	TableItem place;
}; 
class LineEditItem :public QLineEdit {
	Q_OBJECT
public:
	LineEditItem(int row, int col, QTableWidget *parent) :QLineEdit(parent), place(row, col, parent) {
		
	}
	void enable() {
		connect(this, SIGNAL(textChanged(const QString &)), &place, SLOT(ChangeValue(const QString&)));
	}
private:
	TableItem place;
};
class InsertNumDialog:public QDialog {
	Q_OBJECT
public:
	InsertNumDialog(const QString & text,const QString & title=QString::null, QWidget*parent=0);
	~InsertNumDialog() {}
public slots:
	void pressOk() {
		emit returnValue(numRow.text());
		quit();
	}
	void quit() { destroy(); }
signals:
	void returnValue(QString);
private:
	QLineEdit numRow;
};
class ClickTimer :public QTimer {
	Q_OBJECT
public:
		ClickTimer(QWidget* parent) :QTimer(parent), m_data() { connect(this, SIGNAL(timeout()), this, SLOT(timeOut())); }
public slots:
	void Start(const QModelIndex&d) {
		if (isActive() && d == m_data) {
			emit clickTwice(m_data);
			stop();
		}
		else {
			start(1000);
		}
		m_data = d;
	}
private slots:
	void timeOut() {
		emit clickOnce(m_data);
		stop();
	}
signals:
	void clickOnce(const QModelIndex&);
	void clickTwice(const QModelIndex&);
private:
	QModelIndex  m_data;
};
#endif//SETDATAFORM_H