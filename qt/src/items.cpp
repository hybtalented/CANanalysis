#include"items.h"
ItemPie::ItemPie(QString label, double value, QObject* parent ):QPieSlice(label, value, parent) ,savedlabel(label){
	QObject::connect(this, SIGNAL(hovered(bool)), this, SLOT(do_hovered(bool)));
}
void ItemPie::do_hovered(bool state) {
	if (state) {
		setLabel(QString("%1(%2%)").arg(savedlabel).arg(QString::number(percentage()*100,'f',2)));
		setExploded();
	}
	else {
		setLabel(savedlabel);
		setExploded(false);
	}
}
ItemBar::ItemBar(QString label, double value,QObject* parent ):QBarSet(label,parent),savedlabel(label) {
	*this << value;
	QObject::connect(this, SIGNAL(hovered(bool, int)), this, SLOT(do_hovered(bool, int)));

}
void ItemBar::do_hovered(bool state, int index) {
	if (state) {
		setLabel(QString("%1(%2)").arg(savedlabel).arg(QString::number(at(index),'f',2)) );
	}
	else {
		setLabel(savedlabel);
	}
}