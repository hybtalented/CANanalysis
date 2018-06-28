#ifndef CHARTFORM_H
#define CHARTFORM_H
#include <QtWidgets/qmainwindow.h>
#include<QtCharts/qchart.h>
#include"canelement.h"
using QtCharts::QChart;
class ChartView;
class QPrinter;
class SetDataForm;
class ChartForm :public QMainWindow {
	Q_OBJECT
		friend class SetDataForm;
public:
	enum { MAX_ELEMENTS = 100 };
	void closeEvent(QCloseEvent*event) {
		saveOptions();
	}
	enum ChartType { PIE, VERTICAL_BAR, HORIZONTAL_BAR };
	enum AddValuesType { NO, YES, AS_PERCENTAGE };
	ChartForm(SetDataForm*);
	~ChartForm(){}
	int chartType(){return m_chartType;}
	void drawElements();
public slots:
	void saveOptions();
	void quitWindow() {
		destroy(true,true);
	}
private:
	void init();
	void init_menutoolbar();

	void drawPieChart();
	void drawHorizontalBarChart();
	void drawVerticalBarChart();
	//QString valueLabel(const QString&label, double value, double total);

	void setChartType(ChartType chartType) {
		m_chartType = chartType;
	}

	QMenu *optionMenu;
	QMenu *fileMenu;
	QMenu * plotMenu;

	QChart * m_chart;
	ChartView* m_chartView;
	CANVector* m_elements;
	QPrinter *m_printer;
	ChartType m_chartType;
	AddValuesType m_addValues;
	int m_decimalPlaces;
	QFont m_font;

	QString title;
};


#endif//CHARTFORM_H