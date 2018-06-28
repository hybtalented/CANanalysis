#include"chartform.h"
#include<QtCharts/qpieseries.h>
#include<QtCharts/qbarseries.h>
#include<QtCharts/qhorizontalbarseries.h>
#include<QtWidgets/qstatusbar.h>
#include<QtCharts/qbarcategoryaxis.h>
#include<QtCharts/qvalueaxis.h>
#include"items.h"
#include"setdataform.h"
using namespace QtCharts;
typedef QList<QAbstractSeries *> QSeriesList;

void ChartForm::drawElements() {
	m_chart->removeAllSeries();
	if (m_chart->axisX()) {
		m_chart->removeAxis(m_chart->axisX());
	}
	if (m_chart->axisY()) {
		m_chart->removeAxis(m_chart->axisY());
	}
	switch (m_chartType) {
	case PIE:
		drawPieChart();
		break;
	case VERTICAL_BAR:
		drawVerticalBarChart();
		break;
	case HORIZONTAL_BAR:
		drawHorizontalBarChart();
		break;
	default:
		statusBar()->showMessage("Unknowing plot type");
		break;
	}
	m_chart->update();
}

void ChartForm::drawPieChart() {
	QPieSeries* series=new QPieSeries(m_chart);
	for (CANVector::iterator it = m_elements->begin(); it != m_elements->end(); ++it) {
		if (it->isValid()) {
		}
	}
	m_chart->addSeries(series);
}
void ChartForm::drawHorizontalBarChart(){
	Q_ASSERT( m_chartType==HORIZONTAL_BAR);
	QHorizontalBarSeries * series = new QHorizontalBarSeries(m_chart);
	for (CANVector::iterator it = m_elements->begin(); it != m_elements->end(); ++it) {
	}
	QValueAxis *vaxis = new QValueAxis();
	QStringList category;
	category << title;
	QBarCategoryAxis *caxis = new QBarCategoryAxis();
	caxis->setCategories(category);
	caxis->labelsVisible();
	m_chart->addSeries(series);
	m_chart->setAxisY(caxis, series);
	m_chart->setAxisX(vaxis, series);
	vaxis->applyNiceNumbers();
}
void ChartForm::drawVerticalBarChart() {
	Q_ASSERT(m_chartType == VERTICAL_BAR);
	QBarSeries * series = new QBarSeries(m_chart);
	for (CANVector::iterator it = m_elements->begin(); it != m_elements->end(); ++it) {
		if (it->isValid()) {
		}
	}
	QValueAxis *vaxis = new QValueAxis();
	QStringList category;
	category << title;
	QBarCategoryAxis *caxis = new QBarCategoryAxis();
	caxis->setCategories(category);
	caxis->labelsVisible();
	m_chart->addSeries(series);
	m_chart->setAxisX(caxis, series);
	m_chart->setAxisY(vaxis, series);
}