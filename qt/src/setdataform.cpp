#include<QtWidgets/qboxlayout.h>
#include<QtWidgets/qstyleditemdelegate.h>
#include<QtWidgets/qlineedit.h>
#include<QtCore/qstringlist.h>
#include<QtWidgets/qaction.h>
#include<QtWidgets/qtoolbar.h>
#include<QtWidgets/qmenu.h>
#include<QtWidgets/qmenubar.h>
#include"setdataform.h"
#include"canelement.h"
#include"chartform.h"
#include<QtCore/qsettings.h>
#include<QtWidgets/qapplication.h>
#include<QtWidgets/qlineedit.h>
#include<QtCore/qtimer.h>
#pragma execution_character_set("utf-8")
SetDataForm::SetDataForm(const QString &filename, int decimalPaces, QWidget*parent, const char* name , bool modal, Qt::WindowFlags f ):QMainWindow(parent,f) ,m_filename(filename),chartform(0){
	QSettings settings(QSettings::UserScope, qApp->applicationName(), qApp->organizationName(), this);
	timer = new ClickTimer(this);
	connect(timer, SIGNAL(clickOnce(const QModelIndex&)), this, SLOT(ClickOnce(const QModelIndex&)));
	connect(timer, SIGNAL(clickTwice(const QModelIndex&)), this, SLOT(showItemWindow(const QModelIndex&)));
	int windowX = settings.value("WindowX", 100).toInt();
	int windowY = settings.value("WindowY", 200).toInt();
	int windowWidth = settings.value("WindowWidth", 460).toInt();
	int windowHeight = settings.value("WindowHeight", 530).toInt();
	setGeometry(windowX, windowY, windowWidth, windowHeight);
	setWindowTitle(name);
	m_decimalPaces = decimalPaces;
	table = new QTableWidget(m_elements.size(), 5, this);
	setCentralWidget(table);
	table->show();
	init_menutoolbar();
	for (int i = 0; i < MAX_RECENTFILES; ++i) {
		QString filename = settings.value(QString("File/") + QString::number(i + 1)).toString();
		if (!filename.isEmpty()) {
			m_recentFiles.push_back(filename);
		}
		
	}
	if (m_recentFiles.size())
		updateRecentFilesMenu();
	if (!filename.isEmpty())
		load(filename);
	init_table();
	connect(table, SIGNAL(clicked(const QModelIndex&)), timer, SLOT(Start(const QModelIndex&)));
	connect(chartGroup, SIGNAL(triggered(QAction*)), this, SLOT(updateChartType(QAction*)));
}
void SetDataForm::init() {
	QString title = "Edit Chart	";
	if (m_filename.isEmpty()) {
		title += "*";
	}
	else {
		title += m_filename + "-	";
	}
	setWindowTitle(title);
	setWhatsThis("a program to draw charts");
	m_filename = QString::null;
	m_elements.clear();
	m_changed = false;
}
void SetDataForm::init_menutoolbar() {
	// file operator options
	QAction *fileNewAction;
	QAction *fileOpenAction;
	QAction*fileSaveAction;
	QAction*fileSaveAsAction;
	fileNewAction = new QAction(QPixmap(":/toolbar/filenew.png"), "new Chart", this);
	fileNewAction->setShortcut(Qt::CTRL | Qt::Key_N);
	fileOpenAction = new QAction(QPixmap(":/toolbar/fileopen.png"), "Open Chart", this);
	fileOpenAction->setShortcut(Qt::CTRL | Qt::Key_O);
	fileSaveAction = new QAction(QPixmap(":/toolbar/save.png"), "Save Chart", this);
	fileSaveAction->setShortcut(Qt::CTRL | Qt::Key_S);
	fileSaveAsAction = new QAction(QPixmap(QString(":/toolbar/document-save-as.png")), "保存为", this);
	fileSaveAsAction->setObjectName("保存为");
	QObject::connect(fileNewAction, SIGNAL(triggered()), this, SLOT(fileNew()));
	QObject::connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(fileOpen()));
	QObject::connect(fileSaveAction, SIGNAL(triggered()), this, SLOT(fileSave()));
	connect(fileSaveAsAction, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
	// Edit operation actions
	QAction* addrow = new QAction(QPixmap(":/toolbar/addrow.png"), "Add row", this);
	connect(addrow, SIGNAL(triggered()), this, SLOT(editAddRow()));
	// option operation action
	chartGroup = new QActionGroup(this);
	chartGroup->setExclusive(true);
	optionsPieChartAction = new QAction(QPixmap(":/toolbar/pie.png"), "Pie", chartGroup);
	optionsPieChartAction->setShortcut(Qt::Key_I);
	optionsPieChartAction->setCheckable(true);
	optionsHorizontalBarChartAction = new QAction(QPixmap(":/toolbar/horizontalbar.png"), "Horizontal bar", chartGroup);
	optionsHorizontalBarChartAction->setShortcut(Qt::Key_H);
	optionsHorizontalBarChartAction->setCheckable(true);
	optoinsVerticalBarChartAction = new QAction(QPixmap(":/toolbar/verticalbar.png"), "Vertical bar", chartGroup);
	optoinsVerticalBarChartAction->setShortcut(Qt::Key_V);
	optoinsVerticalBarChartAction->setCheckable(true);
	//	set Toolbar
	QToolBar* fileTools = new QToolBar("file operation", this);
	fileTools->addAction(fileNewAction);
	fileTools->addAction(fileOpenAction);
	fileTools->addAction(fileSaveAction);
	fileTools->addAction(fileSaveAsAction);
	addToolBar(Qt::TopToolBarArea, fileTools);
	//set Edit Bar
	fileTools->addSeparator();
	fileTools->addAction(addrow);
	fileTools->addSeparator();
	//optoin toolbar
	fileTools->addAction(optionsPieChartAction);
	fileTools->addAction(optionsHorizontalBarChartAction);
	fileTools->addAction(optoinsVerticalBarChartAction);
	// items in "file" menu
	fileMenu = new QMenu(tr("File"), this);
	menuBar()->addMenu(fileMenu);
	fileMenu->addAction(fileNewAction);
	fileMenu->addAction(fileOpenAction);
	fileMenu->addAction(fileSaveAction);
	fileMenu->addAction(fileSaveAsAction);
	QAction*qAction = fileMenu->addAction("&Quit", this, SLOT(fileQuit()));
	qAction->setShortcut(Qt::ALT | Qt::Key_F4);
	qAction->setObjectName("Quit");
	// items in "Edit" menu
	editMenu = new QMenu(tr("Edit"),this);
	menuBar()->addMenu(editMenu);
	editMenu->addAction(addrow);
	// items in "option" menu
	optionMenu = new QMenu(tr("Option"), this);
	menuBar()->addMenu(optionMenu);
	QMenu*plotMenu = optionMenu->addMenu("&Plot");
	plotMenu->setObjectName("plotmenu");
	optionMenu->addMenu(plotMenu);
	plotMenu->addAction(optionsPieChartAction);
	plotMenu->addAction(optionsHorizontalBarChartAction);
	plotMenu->addAction(optoinsVerticalBarChartAction);
}
// Show recent file in file menu
#include"countaction.h"
void SetDataForm::updateRecentFilesMenu() {
	const QString fileRecents = "File Recents";
	QMenu* recentMenu = fileMenu->findChild<QMenu*>(fileRecents, Qt::FindDirectChildrenOnly);
	if (recentMenu) {
		recentMenu->clear();
	}
	else {
		recentMenu = new QMenu(fileRecents, fileMenu);
		recentMenu->setObjectName(fileRecents);
		fileMenu->insertMenu(findChild<QAction*>("Quit"), recentMenu);
	}
	for (int i = 0; i < m_recentFiles.count(); ++i) {
		int first = m_recentFiles[i].lastIndexOf("/");
		QString filename = m_recentFiles[i].right(m_recentFiles.length() - first - 1);

		CountAction * caction = new CountAction(QString("&%1: %2").arg(i + 1).arg(filename), recentMenu, i);
		caction->setWhatsThis(m_recentFiles[i]);
		recentMenu->addAction(caction);
		connect(caction, SIGNAL(setCount(int)), this, SLOT(fileOpenRecent(int)));
	}
}
void SetDataForm::updateRecentFiles(const QString&filename) {
	int i;
	for (i = 0; i < m_recentFiles.count(); i++) {
		if (m_recentFiles[i] == filename)
			break;
	}
	if (i < m_recentFiles.count()) {
		m_recentFiles.removeAt(i);
	}
	if (m_recentFiles.count() > MAX_RECENTFILES)
		m_recentFiles.pop_back();
	m_recentFiles.push_front(filename);
	updateRecentFilesMenu();
}
void SetDataForm::saveOptions() {
	QSettings settings(QSettings::UserScope, qApp->applicationName(), qApp->organizationName(), this);
	QRect r = geometry();
	settings.setValue("version", qApp->applicationVersion());
	settings.setValue("WindowX", r.x());
	settings.setValue("WindowY", r.y());
	settings.setValue("WindowWidth", r.width());
	settings.setValue("WindowHeight", r.height());
	settings.setValue("Decimals", m_decimalPaces);
	for (int i = 0; i< MAX_RECENTFILES; i++) {
		if (i < m_recentFiles.count()) {
			settings.setValue(QString("File/") + QString::number(i + 1), m_recentFiles[i]);
		}
		else {
			settings.setValue(QString("File/") + QString::number(i + 1), QString());
		}
		
	}
}
void SetDataForm::updateChartType(QAction*act) {
	if (!chartform) {
		chartform = new ChartForm(this);
		chartform->setAttribute(Qt::WA_QuitOnClose, false);
		chartform->setAttribute(Qt::WA_DeleteOnClose, true);
		connect(chartform, SIGNAL(destroyed()), this, SLOT(chartformclose()));
		chartform->show();
		connect(this, SIGNAL(windowTitleChanged(const QString&)), chartform, SLOT(setWindowTitle(const QString&)) );
	}
	if (act == optionsPieChartAction) {
		chartform->setChartType(chartform->PIE);
	}
	else if (act == optoinsVerticalBarChartAction) {
		chartform->setChartType(chartform->VERTICAL_BAR);
	}
	else if (act == optionsHorizontalBarChartAction) {
		chartform->setChartType(chartform->HORIZONTAL_BAR);
	}

	chartform->drawElements();
}
SetDataForm::~SetDataForm() {
	for (CANVector::iterator it = m_elements.begin(); it != m_elements.end(); ++it) {
		delete (*it).CANframe();
	}
}
#include<QtWidgets/qpushbutton.h>
#include<QtWidgets/qlabel.h>
InsertNumDialog::InsertNumDialog(const QString & text, const QString & title, QWidget*parent) : QDialog(parent),numRow(this) {
	setWindowTitle(title);
	QGridLayout * gridlayout = new QGridLayout(this);
	setLayout(gridlayout);
	QPushButton* ok = new QPushButton(tr("&Ok"), this), *cancel = new QPushButton(tr("&Cancel"), this);
	QLabel * rowlabel = new QLabel(text, this);
	QRegExpValidator * numRowVal = new QRegExpValidator(QRegExp("[0-9]{0,2}"),&numRow);
	numRow.setValidator(numRowVal);
	gridlayout->addWidget(rowlabel, 0, 0, 1, 1, Qt::AlignLeft);
	gridlayout->addWidget(&numRow, 0, 1, 1, 3, Qt::AlignCenter);
	gridlayout->addWidget(ok, 1, 2, 1, 1);
	gridlayout->addWidget(cancel, 1, 3, 1, 1);
	gridlayout->setColumnStretch(0, 2);
	connect(ok, SIGNAL(pressed()), this, SLOT(pressOk()));
	connect(cancel, SIGNAL(pressed()), this, SLOT(quit()));
}
