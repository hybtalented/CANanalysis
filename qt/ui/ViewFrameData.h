#ifndef VIEWFRAMEDATA_H
#define VIEWFRAMEDATA_H
#include <QtWidgets/QMainWindow>
#include "ui_ViewFrameData.h"
class ViewFrameData : public QDialog
{
	Q_OBJECT
public:
	ViewFrameData(QWidget *parent = Q_NULLPTR);
	void setFrameName(const QString&s) { ui.framename->setText(s);  }
	void clearAddData();
	void setData(int i, const QString&s);
	void setCANID(const QString&s) { ui.canid->setText(s); }
	void setState(const QString&s) { ui.state->setText(s); }
	void setIndex(const QString&s) { ui.Index->setText(s); }
	void setAdditionText(int index, const QString&label, const QString &text);
	void setAddtionState(int index, const QString&label, int state);
	void setAddtionStateName(const QString &name) { ui.groupBox_3->setTitle(name); }
	void setFrame(int value, int mi = -1, int ma = -1) { frame = value; ui.frame->setValue(value); if(mi>=0&&ma>mi)setFrameMinMax(mi, ma); }
	void setFrameMinMax(int mi, int ma) { max = ma; min = mi; ui.frame->setRange(mi, ma); }
private slots:
	void frameBefore() {
		if (frame - 1 >= min&& frame - 1 >=0) {
			ui.frame->setValue(--frame);
			emit frameChanged(frame, this);
		}
	
	}
	void frameAfter() {
		if (frame + 1 < max&&frame+1>=0) {
			ui.frame->setValue(++frame);
			emit frameChanged(frame, this);
		}
	}
	void changeFrame(int i) {
		if (frame != i) {
			if (min <= i && i < max) {
				emit frameChanged(frame = i,this);
			}
			else {
				ui.frame->setValue(frame);
			}
		}
	}
signals:
	void frameChanged(int, ViewFrameData*);
private:
	int frame,max,min;
	Ui::Dialog ui;
	QList<QWidget*> adds;
};
#endif//VIEWFRAMEDATA_H