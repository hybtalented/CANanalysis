#include"chartform.h"
#include<QtWidgets/qmenu.h>
#include<QtWidgets/qaction.h>
#include<QtWidgets/qactiongroup.h>
#include<QtWidgets/qtoolbar.h>
#include<QtWidgets/qmenubar.h>
#include<QtCore/qsettings.h>
#include<QtWidgets/qapplication.h>
#include"chartview.h"
#include<QtWidgets/qmessagebox.h>
#include<QtWidgets/qfiledialog.h>
#include<QtWidgets/qstatusbar.h>
#include"canelement.h"
#include"setdataform.h"
#include<QtCore/qdebug.h>
ChartForm::ChartForm(SetDataForm*parent) :QMainWindow(parent) {
	init_menutoolbar();
	m_elements = &parent->elementvector();
	QSettings settings(QSettings::UserScope,qApp->applicationName(),qApp->organizationName(),this);
	int windowX = settings.value("ChartFormWindowX", 100).toInt();
	int windowY = settings.value("ChartFormWindowY", 200).toInt();
	int windowWidth = settings.value("ChartFormWindowWidth", 460).toInt();
	int windowHeight = settings.value("ChartFormWindowHeight", 530).toInt();
	setChartType(ChartType(settings.value("ChartType", int(PIE)).toInt()));
	m_font = QFont("Time new Roman", 18,QFont::Bold);
	m_font.fromString(settings.value("Font", m_font.toString()).toString());
	setGeometry(windowX, windowY, windowWidth, windowHeight);
	m_chart = new QChart;
	m_chart->setAnimationOptions(QChart::SeriesAnimations);
	m_chart->resize(size());
	m_chartView = new ChartView(m_chart,m_elements, this);
	setCentralWidget(m_chartView);
	m_chartView->show();
	update();
}
void ChartForm :: init() {
	setWindowTitle("Chart");
	setWhatsThis("a program to draw charts");
}
void ChartForm::init_menutoolbar() {
	SetDataForm * p = qobject_cast<SetDataForm *>(parent());
	if (!p) {
		 qDebug()<<"Unknowing parents, parent mush be class SetDataForm";
	}
	fileMenu = new QMenu(tr("File"), this);
	menuBar()->addMenu(fileMenu);
	QAction*imageSaveAsAction=new QAction(QPixmap(":/toolbar/gallery.png"),"Save As IMAGE",fileMenu);
	//	set Toolbar
	QToolBar* fileTools = new QToolBar("file operation", this);
	addToolBar(Qt::TopToolBarArea, fileTools);
	fileTools->addAction(imageSaveAsAction);
	fileTools->addSeparator();
	//optoin toolbar
	fileTools->addAction(p->optionsPieChartAction);
	fileTools->addAction(p->optionsHorizontalBarChartAction);
	fileTools->addAction(p->optoinsVerticalBarChartAction);
	// items in "file" menu

	menuBar()->addMenu(fileMenu);
	fileMenu->addAction(imageSaveAsAction);
	QAction *mquit=new QAction("&Quit", this);
	mquit->setShortcut(Qt::ALT | Qt::Key_F4);
	fileMenu->addAction(mquit);
	connect(mquit, SIGNAL(triggered()), parent(), SLOT(chartformclose()));
	connect(mquit, SIGNAL(triggered()),this,SLOT(quitWindow()));
	// items in "option" menu
	
	optionMenu = new QMenu(tr("Option"), this);
	menuBar()->addMenu(optionMenu);
	plotMenu = p->optionMenu->findChild<QMenu*>("plotmenu");
	plotMenu->setShortcutEnabled(true);
	plotMenu->grabShortcut(Qt::Key_P);
	menuBar()->addMenu(plotMenu);
	optionMenu->addMenu(plotMenu);
	plotMenu->addAction(p->optionsPieChartAction);
	plotMenu->addAction(p->optionsHorizontalBarChartAction);
	plotMenu->addAction(p->optoinsVerticalBarChartAction);

}

void ChartForm::saveOptions() {
	QSettings settings(QSettings::UserScope, qApp->applicationName(), qApp->organizationName(), this);
	settings.setValue("ChartFormWindowX", x());
	settings.setValue("ChartFormWindowY", y());
	settings.setValue("ChartFormWindowWidth", width());
	settings.setValue("ChartFormWindowHeight", height());
	settings.setValue("ChartType", int(m_chartType));
	settings.setValue("Font", m_font.toString());
	settings.setValue("Decimals", m_decimalPlaces);
}




