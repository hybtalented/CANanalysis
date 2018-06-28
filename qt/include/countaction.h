#pragma once

#include<qaction.h>

class CountAction :public QAction {
	Q_OBJECT
public:
	CountAction(QString & text, QWidget*parent = nullptr, int count = 0) :QAction(text, parent), m_count(count) { connect(this, SIGNAL(ttriggered()), this, SLOT(countSend())); }
	CountAction(QString && text, QWidget*parent = nullptr, int count = 0) :QAction(text, parent), m_count(count) { connect(this, SIGNAL(triggered()), this, SLOT(countSend())); }
	
	CountAction(QIcon&icon,QString & text, QWidget*parent = nullptr, int count = 0) :QAction(icon,text, parent),m_count(count){ connect(this, SIGNAL(triggered()), this, SLOT(countSend())); }
	CountAction(QIcon&icon,QString && text, QWidget*parent = nullptr, int count = 0) :QAction(icon,text, parent),m_count(count){ connect(this, SIGNAL(triggered()), this, SLOT(countSend())); }
	void countSet(int count) { m_count = count; }
	int count() { return m_count; }
public slots:
	void countSend() {
		emit setCount(m_count);
	}
signals:
	void setCount(int);
private:
	int m_count;
};