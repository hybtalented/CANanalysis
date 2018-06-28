#ifndef ELEMENT_H
#define ELEMENT_H
#define INT_TYPE char
#define INT_BIT (sizeof(INT_TYPE) * 8)
#include<canread.h>
class CANElement :public stdStringList {
public:
	CANElement() :stdStringList(), frame(NULL) {  }
	~CANElement() { }
	string detail(int index) { return operator[](index); }
	bool isValid() { return frame != NULL; }
	void setFrame(Basic_CAN_frame * f) {
		frame = f;
	}
	Basic_CAN_frame* CANframe()const { return frame; }
private:
	Basic_CAN_frame* frame;
};
typedef std::vector<CANElement> CANVector;

#endif//ELEMENT_H
