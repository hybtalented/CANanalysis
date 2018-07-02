#include"setdataform.h"
#include<QtCore/qfile.h>
#include<QtWidgets/qstatusbar.h>
#include<fstream>
#include"chartconfig.h"
#include<QtWidgets/qmessagebox.h>
#include<QtWidgets/qfiledialog.h>
#include<QtWidgets/qapplication.h>
#include<QtCore/qtextcodec.h>
#pragma execution_character_set("utf-8")
void SetDataForm::load(const QString&filename) {
	QTextCodec *code = QTextCodec::codecForName("system");
	std::string name = code->fromUnicode(filename).data();
	ifstream file(name);
	if (!file) {
		statusBar()->showMessage(tr("���ļ�ʧ�� \"%1\"").arg(filename), 2000);
		return;
	}
	CANElement element;
	CANREAD can;
	stdStringList temp;
	can.read_data(file);
	delete_elementdata();
	while (!can.atEnd()) {
		can >> element;
		m_elements.push_back(element);
	}
	file.close();
	setWindowIconText(filename);
	updateRecentFiles(filename);
	init_multiFrame();
	m_changed = false;
}
void SetDataForm::fileSave() {
}

bool SetDataForm::okToClear() {
	if (m_changed) {
		QString msg;
		if (m_filename.isEmpty())
			msg = tr("δѡ���ļ�");
		else
			msg = QString("%1 '%2'\n").arg(PROJECT_NAME).arg(m_filename);
		msg += tr("�Ѿ������ı�");
		switch (QMessageBox::information(this, QString(PROJECT_NAME)+ QString(" -- ")+tr("��δ��������ݣ�"), msg, "���棿", "ȡ��", 0, 1)) {
		case 0:
			fileSave();
			break;
		case 1:
		default:
			return false;
			break;
		}
	}
	return true;
}
void SetDataForm::fileNew() {
}

void SetDataForm::fileOpen() {
	if (!okToClear()) {
		return;
	}
	QString filename = QFileDialog::getOpenFileName(this, QString(PROJECT_NAME)+QString(" -- ") +tr("���ļ�"), windowFilePath(), tr("�ı��ļ�")+" (*.txt)");
	if (!filename.isEmpty()) {
		load(filename);
		init_table();
	}
	else {
		statusBar()->showMessage(tr("���ļ�ʧ��"), 2000);
	}
}
void SetDataForm::fileSaveAs() {
	QString filename = QFileDialog::getSaveFileName(this, QString("%1 -- %2").arg(PROJECT_NAME).arg(tr("�ļ����Ϊ")), windowFilePath(),QString("%1 (*.txt)").arg(PROJECT_NAME));
	if (!filename.isEmpty()) {
		int answer = 0;
		m_filename = filename;
		updateRecentFiles(filename);
		fileSave();
		return;
	}
	statusBar()->showMessage(tr("�޷����� !"), 2000);
}
void SetDataForm::fileOpenRecent(int index) {
	if (!okToClear())
		return;
	load(m_recentFiles[index]);
	init_table();
}
void SetDataForm::fileQuit() {
	if (okToClear()) {
		saveOptions();
		if (m_changed) {
			switch (QMessageBox::warning(this, "The file changd but did't saved.\n Do you want to save it?", "&Save", "&Save as", "&Quit with out save"), 0, 2) {
			case 0:
				fileSave();
				break;
			case 1:
				fileSaveAs();
				break;
			case 2:
			default:
				break;
			}
		}
		if (chartform) {
			chartform->destroy();
		}
		qApp->exit(0);
	}
}