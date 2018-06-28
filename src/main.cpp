#include<qapplication.h>
#include"setdataform.h"
#include"chartconfig.h"
int main(int argc, char*argv[]) {
	QApplication app(argc, argv);
	QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	app.setOrganizationName(PROJECT_AUTHOR);
	app.setApplicationName(PROJECT_NAME);
	app.setApplicationVersion(PROJECT_VERSION);
	app.setStyleSheet("QPushButton {color:white}");
	QString filename;
	if (app.arguments().count() > 1) {
		filename = app.arguments()[1];
		if (!filename.endsWith(".cht"))
			filename = QString::null;
	}
	SetDataForm *cf = new SetDataForm(filename,0);
	cf->setAttribute(Qt::WA_QuitOnClose, true);
	cf->show();
	app.connect(qApp, SIGNAL(lastWindowClosed()), cf, SLOT(fileQuit()));
	return app.exec();
}