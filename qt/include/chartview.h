#pragma once
#include<QtCharts/qchartview.h>
#include<QtCharts/qchart>
#include"canelement.h"
using QtCharts::QChartView;
using QtCharts::QChart;
class ChartView :public QChartView {
	Q_OBJECT
public:
	ChartView(QChart*chart, CANVector*elements, QWidget*parent, const char *name = "canvas view") :QChartView(chart, parent) {}
	ChartView&operator=(ChartView&m) {
		*this = m;
		return *this;
	}
};