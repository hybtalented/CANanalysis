#pragma once
#include<QtCore/qobject.h>
#include<QtCore/qstring.h>
#include<QtCharts/qpieslice.h>
#include<QtCharts/qbarset.h>
using QtCharts::QPieSlice;
using QtCharts::QBarSet;
class ItemPie:public QPieSlice {
	Q_OBJECT
public:
	ItemPie(QString label, double value, QObject* parent = nullptr);
public slots:
	void do_hovered(bool);
private:
	QString savedlabel;
};
class ItemBar :public QBarSet {
	Q_OBJECT
public:
	ItemBar(QString label, double value,QObject* parent = nullptr);
public slots:
	void do_hovered(bool status,int index);
private:
	QString savedlabel;
};
