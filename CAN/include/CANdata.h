/*
 * CAN帧的定义 
 */
#ifndef CANDATA_H
#define CANDATA_H
#include<table field.h>
#include<datatype.h>
#include<assert.h>
#include<sstream>
#include<iomanip>

using std::setw;
using std::setfill;




#define CHM_CANID 0x1826f456
#define BHM_CANID 0x182756F4
#define CRM_CANID 0x1801F456
#define CTS_CANID 0x1807F456
#define CML_CANID 0x1808F456
#define BRO_CANID 0x100956F4
#define CRO_CANID 0x100AF456
#define BCL_CANID 0x181056F4
#define CCS_CANID 0x1812F456
#define BSM_CANID 0x181356F4
#define BST_CANID 0x101956F4
#define CST_CANID 0x101AF456
#define BSD_CANID 0x181C56F4
#define CSD_CANID 0x181DF456
#define BEM_CANID 0x081E56F4
#define CEM_CANID 0x081FF456
#define DM1_CANID 0x1820F456
#define DM3_CANID 0x1822F456
#define TP_START_CANID 0x1CEC56F4
#define TP_REPLY_CANID 0x1CECF456
#define TP_DATA_CANID  0x1CEB56F4




#define BRM_PGN	0x02
#define BCP_PGN 0x06
#define BCS_PGN 0x11
#define BMV_PGN 0x15
#define BMT_PGN 0x16
#define BSP_PGN 0x17




#define STATE_HAND_SHK 		0
#define STATE_REC			1
#define STATE_CONFIGURE 	2
#define STATE_CHARGE		3
#define STATE_CHARGE_END	4
#define STATE_FAULT			5
#define STATE_TP			6
#define STATE_UDEF			7



extern UINT DATA_BASE, CANID_BASE, LENGTH_BASE;
#pragma execution_character_set("utf-8")
#define DETAIL_TRUE "true"
#define DETAIL_FALSE "false"
#define DETAIL_BOOLS_NAME "bool name"
#define DETAIL_UNKNOW "unknow"
class Basic_CAN_frame {	
	friend class Multi_frame;
   	friend class CAN_BRM;	
	friend class CAN_BCS;	
	friend class CAN_BCP;
protected:
	DWORD canid;
	UINT index;	
	UINT state;
	static CONST_CHARPTR states[8];
public:
	CONST_CHARPTR getState() { return states[state]; }
	static CONST_CHARPTR getState(int i) { assert(i >= 0 && i < 7); return states[i]; }
	static int getState(const char* str);
	virtual BOOL direct() = 0;
	virtual int detailCount() const = 0;
	virtual string getDetail(int i) const = 0;
	virtual const char* detailName(int i)const = 0;
	virtual BYTE getdata(int i) = 0;
	DWORD getCANID() { return canid; }
	static Basic_CAN_frame *create_frame(DWORD canid);
	VOID setIndex(UINT i) { index = i; }
	UINT getIndex() { return index; }
	Basic_CAN_frame(DWORD id, UINT s) :canid(id), index(0), state(s) {}
	virtual string typeName()const = 0;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&) = 0;
	virtual BYTE getPGN() { return 0xff; }
	virtual BYTE getConsole() { return 0xff; }
	virtual BOOL get_data(Table_field &) = 0;
	virtual string get_detail()const = 0;
	virtual CONST_CHARPTR get_frame_name()const = 0;
	virtual BOOL operator==(Basic_CAN_frame& cf)const { return canid == cf.canid; }
	virtual ~Basic_CAN_frame() {}
};//   基本CAN帧定义 
class CAN_UNDEF :public Basic_CAN_frame {
protected:
	DWORD canid;
	UINT index;
	UINT state;
	static CONST_CHARPTR states[7];
public:
	BOOL direct() {		return  (canid & 0x000000ff) == 0x56;	}
	int detailCount() const {		return 0;	}
	string getDetail(int i)const override {		return "";	}
	const char* detailName(int i)const {		return "";	}
	CAN_UNDEF(DWORD id) :Basic_CAN_frame(id, STATE_UDEF), len(0) {
		for (int i = 0; i < len; i++) {
			compts.byte[i] = 0;
		}
	}
	BYTE getdata(int i) override {
		if (i < len) {
			return  compts.byte[i];
		}
		else {
			return 0;
		}
	}
	string typeName()const {
		return "undefine CAN frame";
	}
	Basic_CAN_frame& operator=(Basic_CAN_frame&cf) {
		CAN_UNDEF& other = dynamic_cast<CAN_UNDEF&>(cf);
		if (cf == *this) {
			len = other.len;
			for (int i = 0; i < 8; ++i) {
				if (i < len) {
					compts.byte[i] = other.compts.byte[i];
				}
				else {
					compts.byte[i] = 0;
				}
			}
		}
		return *this;
	}
	BOOL get_data(Table_field &tf) {
		len = tf.getByteHex(LENGTH_BASE);
		assert(DATA_BASE + 1 + len > tf.getNumField());
		for (int i = 0; i < len; i++) {
			compts.byte[i] = tf.getByteHex(DATA_BASE + i);
		}
		return true;
	}
	string get_detail()const {
		stringstream ss;
		for (int i = 0; i < len; ++i) {
			ss << hex << setw(2) << setfill('0') << compts.byte[i] << ' ';
		}
		return ss.str();
	}	CONST_CHARPTR get_frame_name()const { return "未定义"; }
	BOOL operator==(Basic_CAN_frame& cf)const {
		if (canid != cf.getCANID()) { return false; }
		CAN_UNDEF& other = dynamic_cast<CAN_UNDEF&>(cf);
		if (len != other.len) return false;
		if (compts.word[0] != other.compts.word[0] || compts.word[1] != other.compts.word[1])return false;
		return true;
	}
	virtual ~CAN_UNDEF() {}
private:
	union {
		DWORD word[2];
		BYTE byte[8];
	}compts;
	int len;
};
//--------- 充电握手阶段 ----------//
class CAN_CHM : public Basic_CAN_frame {
	BYTE vmajor;
	WORD vminor;public:
	virtual CONST_CHARPTR get_frame_name()const { return "充电桩握手报文"; }
	BOOL direct() override { return true; }
	CAN_CHM() :Basic_CAN_frame(CHM_CANID, STATE_HAND_SHK), vmajor(0xff), vminor(0xffff) {}
	virtual BOOL get_data(Table_field &);
	virtual string typeName()const { return "CHM"; }
	virtual string get_detail()const;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return vmajor;
		case 1:
			return vminor & 0xff;
		case 2:
			return (vminor >> 8) & 0xff;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 1; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_CHM() {}
};
class CAN_BHM :public Basic_CAN_frame {
	WORD hvol;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "BMS握手报文"; }
	CAN_BHM() :Basic_CAN_frame(BHM_CANID, STATE_HAND_SHK), hvol(0xff) {}
	virtual BOOL get_data(Table_field &);
	BOOL direct()override { return false; }
	virtual string get_detail()const;
	int detailCount() const override {		return 1;	}
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:			return hvol & 0xff;
		case 1:			return  (hvol >> 8) & 0xff;
		default:
			return 0;
		}
	}
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual string typeName()const { return "BHM"; }
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_BHM() {}
};
//----------- 充电辨识阶段 -------------//
class CAN_CRM :public Basic_CAN_frame {
	BYTE rec; 
	DWORD mnum; 
	DWORD mloc; 
public:
	virtual CONST_CHARPTR get_frame_name()const { return "充电桩辨识报文"; }
	virtual string typeName()const { return "CRM"; }
	BOOL direct() override { return true; }
	CAN_CRM() :Basic_CAN_frame(CRM_CANID, STATE_REC), rec(0xff), mnum(0xffffffff), mloc(0xffff) {}
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return rec;
		case 1:
			return mnum & 0xff;
		case 2:
			return mnum >> 8 & 0xff;
		case 3:
			return mnum >> 16 & 0xff;
		case 4:
			return mnum >> 24 & 0xff;
		case 5:
			return mloc & 0x0f;
		case 6:
			return mloc >> 8 & 0xff;
		case 7:
			return mloc >> 16 & 0xff;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 3; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	virtual ~CAN_CRM() {}
};
//------------------ 充电配置阶段 --------------------//
class CAN_CTS :public Basic_CAN_frame {
	BYTE	second, minute, hour, day, month;
	WORD year;//BCD码
public:
	virtual CONST_CHARPTR get_frame_name()const { return "时间同步信息报文"; }
	virtual string typeName()const { return "CTS"; }
	BOOL direct() override { return true; }
	int detailCount() const override { return 1; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return second;
		case 1:
			return minute;
		case 2:
			return hour;
		case 3:
			return day;
		case 4:
			return month;
		case 5:
			return year & 0xff;
		case 6:
			return (year >> 8) & 0xff;
		default:
			return 0;
		}
	}
	CAN_CTS() :Basic_CAN_frame(CTS_CANID, STATE_CONFIGURE), second(0xff), minute(0xff), hour(0xff), day(0xff), month(0xff), year(0xffff) {}
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_CTS() {}
};
class CAN_CML :public Basic_CAN_frame {
	WORD houtvol, loutvol, houtcur, loutcur;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "充电桩最大输出能力报文"; }
	virtual string typeName()const { return "CML"; }
	BOOL direct() override { return true; }
	int detailCount() const override { return 4; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return houtvol & 0xff;
		case 1:
			return houtvol >> 8 & 0xff;
		case 2:
			return loutvol & 0xff;
		case 3:
			return loutvol >> 8 & 0xff;
		case 4:
			return houtcur & 0xff;
		case 5:
			return houtcur >> 8 & 0xff;
		case 6:
			return loutcur & 0xff;
		case 7:
			return loutcur >> 8 & 0xff;
		default:
			return 0;
		}
	}
	CAN_CML() :Basic_CAN_frame(CML_CANID, STATE_CONFIGURE), houtvol(0xffff), loutvol(0xffff), houtcur(0xffff), loutcur(0xffff) {}
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_CML() {}
};
class CAN_BRO :public Basic_CAN_frame {
	BYTE rec;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "BMS准备就绪报文"; }
	virtual string typeName()const { return "BRO"; }
	BOOL direct() override { return false; }
	CAN_BRO() :Basic_CAN_frame(BRO_CANID, STATE_CONFIGURE), rec(0xff) {}
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return rec;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 1; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_BRO() {}
};
class CAN_CRO :public Basic_CAN_frame {
	BYTE rec;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "充电桩准备就绪报文"; }
	CAN_CRO() :Basic_CAN_frame(CRO_CANID, STATE_CONFIGURE), rec(0xff) {}
	BOOL direct() override { return true; }
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return rec;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 1; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual string typeName()const { return "CRO"; }
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_CRO() {}
};
//---------- 充电阶段 ----------//
class CAN_BCL :public Basic_CAN_frame {
	WORD nvol, ncur;
	BYTE cmod;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "BMS充电需求报文"; }
	virtual string typeName()const { return "BCL"; }
	BOOL direct() override { return false; }
	CAN_BCL() :Basic_CAN_frame(BCL_CANID, STATE_CHARGE), nvol(0xffff), ncur(0xffff), cmod(0xff) {}
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:	
		return nvol & 0xff;
		case 1:
			return nvol >> 8 & 0xff;
		case 2:
			return ncur & 0xff;
		case 3:
			return ncur >> 8 & 0xff;
		case 4:
			return cmod;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 3; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_BCL() {}
};
class CAN_CCS :public Basic_CAN_frame {
	WORD ovol, ocur, chargetime;
	BYTE chargeallow;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "充电桩充电状态报文"; }
	virtual string typeName()const { return "CCS"; }
	BOOL direct() override { return true; }
	CAN_CCS() :Basic_CAN_frame(CCS_CANID, STATE_CHARGE), ovol(0xffff), ocur(0xffff), chargetime(0xffff), chargeallow(0xff) {}
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return ovol & 0xff;
		case 1:
			return ovol >> 8 & 0xff;
		case 2:
			return ocur & 0xff;
		case 3:
			return ocur >> 8 & 0xff;
		case 4:
			return chargetime & 0xff;
		case 5:
			return chargetime >> 8 & 0xff;
		case 6:
			return chargeallow;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 4; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_CCS() {}
};
class CAN_BSM :public Basic_CAN_frame {
	BYTE maxvolid, maxtem, maxtemid, mintem, mintemid;
	union {
		struct {
			WORD volstate : 2; 
			WORD chargestate : 2; 
			WORD curstate : 2; 
			WORD temstate : 2; 
			WORD insultstate : 2; 
			WORD OCS : 2; 
			WORD CA : 2;
 		}compts;
		WORD data;
	}chargestate;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "动力蓄电池状态信息报文"; }
	virtual string typeName()const { return "BSM"; }
	BOOL direct() override { return false; }
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return maxvolid;
		case 1:
			return maxtem;
		case 2:
			return maxtemid;
		case 3:
			return mintem;
		case 4:
			return mintemid;
		case 5:
			return chargestate.data & 0xff;
		case 6:
			return chargestate.data >> 8 & 0xff;
		default:
			return 0;
		}
	}
	CAN_BSM() :Basic_CAN_frame(BSM_CANID, STATE_CHARGE), maxvolid(0xff), maxtem(0xff), maxtemid(0xff), mintem(0xff), mintemid(0xff) { chargestate.data = 0xffff; }
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	int detailCount() const override { return 15; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_BSM() {}
};
class CAN_BST :public Basic_CAN_frame {
	union {
		struct {
			BYTE stopbySOC : 2;
			BYTE stopbyVT : 2;
			BYTE stopbyVN : 2;
			BYTE stopbyCM : 2;
		} compts;
		BYTE data;
	}stopinfo;
	union {
		struct {
			WORD insultFault : 2;
			WORD OCTFault : 2;
			WORD BMStemFault : 2;
			WORD CCFault : 2;
			WORD BTFault : 2;
			WORD HVolFault : 2;
			WORD T2VolFault : 2;
			WORD OtherFault : 2;
		}compt;
		BYTE data[2];
	}faultinfo;
	union {
		struct {
			BYTE curfault : 2; 
			BYTE volfault : 2; 
			BYTE : 4;
		}compt;
		BYTE data;
	}stopfaultinfo;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "BMS中止充电报文"; }
	virtual string typeName()const { return "BST"; }
	BOOL direct() override { return false; }
	CAN_BST() :Basic_CAN_frame(BST_CANID, STATE_CHARGE) { stopinfo.data = 0xff; faultinfo.data[0] = 0xff; faultinfo.data[1] = 0xff; stopfaultinfo.data = 0xff; }
	virtual BOOL get_data(Table_field &);	virtual string get_detail()const;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return stopinfo.data;
		case 1:
			return faultinfo.data[0];
		case 2:
			return faultinfo.data[1];
		case 3:
			return stopinfo.data;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 11; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_BST() {}
};
class CAN_CST :public Basic_CAN_frame {
	union {
		struct {
			BYTE stopbyCM : 2;
			BYTE stopbyMan : 2;
			BYTE stopbyFault : 2;
			BYTE stopbyBMS : 2;
		} compts;
		BYTE data;
	}stopinfo;
	union {
		struct {
			WORD HTempFault : 2;
			WORD CCFault : 2;
			WORD CMtemFault : 2;
			WORD CurFault : 2;
			WORD CSFault : 2;
			WORD OtherFault : 2;
			WORD : 4;
		}compt;
		BYTE data[2];
	}faultinfo;
	union {
		struct {
			BYTE curfault : 2; 
			BYTE volfault : 2; 
			BYTE : 4;
		}compt;
		BYTE data;
	}stopfaultinfo;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "充电桩中止充电报文"; }
	virtual string typeName()const { return "CST"; }
	BOOL direct() override { return true; }
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return stopinfo.data;
		case 1:
			return faultinfo.data[0];
		case 2:
			return faultinfo.data[1];
		case 3:
			return stopinfo.data;
		default:
			return 0;
		}
	}
	CAN_CST() :Basic_CAN_frame(CST_CANID, STATE_CHARGE) { stopinfo.data = 0xff; faultinfo.data[1] = 0xff; faultinfo.data[0] = 0xff; stopfaultinfo.data = 0xff; }
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	int detailCount() const override { return 9; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_CST() {}
};
//----------- 充电结束阶段	-------------//
class CAN_BSD :public Basic_CAN_frame {
	BYTE SOC; 
	WORD LVol, HVol;
	BYTE LTem, HTem;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "BMS统计数据报文"; }
	CAN_BSD() :Basic_CAN_frame(BSD_CANID, STATE_CHARGE_END), SOC(0xff), LVol(0xffff), HVol(0xffff), LTem(0xff), HTem(0xff) {}
	virtual BOOL get_data(Table_field &);	BOOL direct() override { return false; }
	virtual string get_detail()const;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return SOC;
		case 1:
			return LVol;
		case 2:
			return LVol >> 8 & 0xff;
		case 3:			return  HVol;
		case 4:
			return HVol >> 8 & 0xff;
		case 5:
			return LTem;
		case 6:
			return HTem;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 5; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual string typeName()const { return "BSD"; }
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_BSD() {}
};
class CAN_CSD :public Basic_CAN_frame {
	WORD chargetime, outenergy; 
	DWORD	CM_id;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "充电桩统计数据报文"; }
	virtual string typeName()const { return "CSD"; }
	CAN_CSD() :Basic_CAN_frame(CSD_CANID, STATE_CHARGE), chargetime(0xffff), outenergy(0xffff), CM_id(0xffffffff) {}
	virtual BOOL get_data(Table_field &);
	BOOL direct() override { return true; }
	virtual string get_detail()const;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return chargetime;
		case 1:
			return chargetime >> 8 & 0xff;
		case 2:
			return  outenergy;
		case 3:
			return outenergy >> 8 & 0xff;
		case 4:
			return CM_id;
		case 5:
			return CM_id >> 8 & 0xff;
		case 6:
			return CM_id >> 16 & 0xff;
		case 7:
			return CM_id >> 24 & 0xff;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 3; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_CSD() {}
};
//------------- 错误报文 ------------//
class CAN_BEM :public Basic_CAN_frame {
	union {
		struct {
			DWORD CRM00_OV : 2;
			DWORD CRMAA_OV : 2;
			DWORD : 4;
			DWORD CML_OV : 2;
			DWORD CRO_OV : 2;
			DWORD : 4;
			DWORD CCS_OV : 2;
			DWORD CST_OV : 2;
			DWORD : 4;
			DWORD CSD_OV : 2;
			DWORD : 6;
		} compts;
		DWORD data;
	}errinfo;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "BMS错误报文"; }
	virtual string typeName()const { return "BEM"; }
	BOOL direct() override { return false; }
	CAN_BEM() :Basic_CAN_frame(BEM_CANID, STATE_FAULT) { errinfo.data = 0xffffffff; }
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return errinfo.data * 0xff;
		case 1:
			return errinfo.data >> 8 & 0xff;
		case 2:
			return  errinfo.data >> 16 & 0xff;
		case 3:
			return errinfo.data >> 24 & 0xff;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 8; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_BEM() {}
};
class CAN_CEM :public Basic_CAN_frame {
	union {
		struct {
			DWORD BRM_OV : 2;
			DWORD : 6;
			DWORD BCP_OV : 2;
			DWORD BRO_OV : 2;
			DWORD : 4;
			DWORD BCS_OV : 2;
			DWORD BCL_OV : 2;
			DWORD BST_OV : 2;
			DWORD : 2;
			DWORD BSD_OV : 2;
			DWORD : 6;
		} compts;
		DWORD data;
	}errinfo;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "充电桩错误报文"; }
	virtual string typeName()const { return "CEM"; }
	BOOL direct() override { return true; }
	CAN_CEM() :Basic_CAN_frame(CEM_CANID, STATE_FAULT) { errinfo.data = 0xffffffff; }
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return errinfo.data * 0xff;
		case 1:
			return errinfo.data >> 8 & 0xff;
		case 2:
			return  errinfo.data >> 16 & 0xff;
		case 3:
			return errinfo.data >> 24 & 0xff;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 8; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_CEM() {}
};
//--------------- 多帧传输 ---------------//
#define TP_START_CONSOLE 	0x10
#define TP_REPLY_CONSOLE 	0x11
#define TP_END_CONSOLE		0x13
class CAN_TP_START :public Basic_CAN_frame {
	friend class Multi_frame;
	friend class CAN_BRM;
	friend class CAN_BCS;
	friend class CAN_BCP;
	BYTE console;
	WORD length;
	BYTE packets;
	BYTE reserve;
	BYTE PGN;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "多帧传输请求帧"; }
	virtual string typeName()const { return "TP_START"; }
	BOOL direct() override { return (canid & 0x000000ff) == 0x56; }
	CAN_TP_START() :Basic_CAN_frame(TP_START_CANID, STATE_TP), console(0xff), length(0xffff), packets(0xff), reserve(0xff), PGN(0xff) {}
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	BYTE getdata(int i) override {
		switch (i)
		{
		case 0:
			return console;
		case 1:
			return length;
		case 2:
			return  length >> 8 & 0xff;
		case 3:
			return packets;
		case 4:
			return reserve;
		case 5:
			return 0xff;
		case 6:
			return PGN;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 3; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	virtual BYTE getPGN() { return PGN; }
	virtual BYTE getConsole() { return console; }
	~CAN_TP_START() {}
};
class CAN_TP_REPLY :public Basic_CAN_frame {
	friend class Multi_frame;
	friend class CAN_BRM;
	friend class CAN_BCS;
	friend class CAN_BCP;
	union {
		struct {
			BYTE console;
			BYTE packets;
			BYTE nextpacket;
			WORD reserve;
			BYTE PGN;
		}reply;
		struct {
			BYTE console;
			WORD length;
			BYTE packets;
			BYTE reserve;
			BYTE PGN;
		}end;
		BYTE data[6];
	}compts;
public:
	virtual CONST_CHARPTR get_frame_name()const { return "多帧传输回复帧"; }
	virtual string typeName()const { return "TP_REPLY(TP_END)"; }
	CAN_TP_REPLY() :Basic_CAN_frame(TP_REPLY_CANID, STATE_TP) { for (UINT i = 0; i < 6; i++)compts.data[i] = 0xff; }
	virtual BOOL get_data(Table_field &);
	BOOL direct() override { return  (canid & 0x000000ff) == 0x56; }
	virtual BYTE getPGN() { return compts.data[5]; }
	BYTE getdata(int i) override {
		if (i < 5) {
			return compts.data[i];
		}
		switch (i) {
		case 5:
			return 0xff;
		case 6:
			return compts.data[5];
		default:
			return 0;
		}
	}
	virtual BYTE getConsole() { return compts.data[0]; }
	virtual string get_detail()const;
	int detailCount() const override {
		if (compts.reply.console == TP_REPLY_CONSOLE || compts.end.console == TP_END_CONSOLE)
			return 3;
		else return 0;
	}
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_TP_REPLY() {}};
class CAN_TP_DATA :public Basic_CAN_frame {
	friend class Multi_frame;
	friend class CAN_BRM;
	friend class CAN_BCS;
	friend class CAN_BCP;
protected:
	BYTE packet;
	BYTE data[7];
public:
	virtual CONST_CHARPTR get_frame_name()const { return "多帧传输数据帧"; }
	virtual string typeName()const { return "TP_DATA"; }
	BOOL direct() override { return (canid & 0x000000ff) == 0x56; }
	CAN_TP_DATA() :Basic_CAN_frame(TP_DATA_CANID, STATE_TP), packet(0xff) {}
	virtual BOOL get_data(Table_field &);
	virtual string get_detail()const;
	BYTE getdata(int i) override {
		if (0<i && i < 7) {
			return data[i - 1];
		}
		switch (i) {
		case 0:
			return packet;
		default:
			return 0;
		}
	}
	int detailCount() const override { return 1; }
	string getDetail(int i) const override;
	const char* detailName(int i)const override;
	virtual BOOL operator==(Basic_CAN_frame& cf)const;
	virtual Basic_CAN_frame& operator=(Basic_CAN_frame&);
	~CAN_TP_DATA() {}
};
//===================== 多帧数据帧 ===================//
class Multi_frame {
protected:
	BYTE PGN;
	UINT state;
	vector<Basic_CAN_frame*> frames;
public:
#define BRM_TP_SIZE 10
#define BCP_TP_SIZE 5
#define BCS_TP_SIZE 5
	CONST_CHARPTR getState() { return Basic_CAN_frame::states[state]; }
	Multi_frame(CAN_TP_START* tp, UINT state) :PGN(tp->PGN), state(state) { frames.push_back(tp); }
	Multi_frame(BYTE pgn, UINT state) :PGN(pgn), state(state) {}
	BYTE getPGN()const { return PGN; }
	virtual BOOL direct() = 0;
	virtual string typeName()const = 0;
	BYTE getByte(UINT i)const;
	WORD getWord(UINT i)const;
	DWORD getDWord(UINT i)const;
	vector<UINT> getIndexs() { vector<UINT> indexs; for (UINT i = 0; i < frames.size(); i++) indexs.push_back(frames[i]->getIndex()); return indexs; }
	VOID addFrame(Basic_CAN_frame* f) { frames.push_back(f); }
	VOID clear() { PGN = 0; state = 0xFFFFFFFF; frames.clear(); }
	virtual CONST_CHARPTR get_frame_name()const = 0;
	virtual string get_detail()const = 0;
	virtual BOOL rec_finish()const = 0;
	virtual BOOL valid()const = 0;
	BOOL operator==(Multi_frame & mf)const;
	Multi_frame& operator=(const Multi_frame&);
	virtual ~Multi_frame() {}
};
//------------- 充电辨识阶段 --------------//
class CAN_BRM :public Multi_frame {
public:
	CAN_BRM(CAN_TP_START*tp) :Multi_frame(tp, STATE_REC) {}
	CAN_BRM() :Multi_frame(BRM_PGN, STATE_REC) {}
	BOOL direct() override { return false; }
	virtual CONST_CHARPTR get_frame_name()const { return "BMS和车辆辨识报文"; }
	Multi_frame& operator=(const Multi_frame&mf) {return Multi_frame::operator=(mf); }
	virtual string typeName()const { return "BRM"; }
	virtual string get_detail()const;
	virtual BOOL rec_finish()const;
	virtual BOOL valid()const;
	virtual ~CAN_BRM() {}
};
//------------- 充电配置阶段 -------------//
class CAN_BCP :public Multi_frame {
public:
	CAN_BCP(CAN_TP_START*tp) :Multi_frame(tp, STATE_CONFIGURE) {}
	CAN_BCP() :Multi_frame(BCP_PGN, STATE_CONFIGURE) {}
	BOOL direct() override { return false; }
	virtual CONST_CHARPTR get_frame_name()const { return "动力蓄电池充电参数报文"; }
	Multi_frame& operator=(const Multi_frame&mf) { return Multi_frame::operator=(mf); }
	virtual string typeName()const { return "BCP"; }
	virtual string get_detail()const;
	virtual BOOL rec_finish()const;
	virtual BOOL valid()const;
	virtual ~CAN_BCP() {}
};
class CAN_BCS :public Multi_frame {
public:
	CAN_BCS(CAN_TP_START*tp) :Multi_frame(tp, STATE_CHARGE) {}
	CAN_BCS() :Multi_frame(BCS_PGN, STATE_CHARGE) {}
	BOOL direct() override { return false; }
	virtual CONST_CHARPTR get_frame_name()const { return "电池充电总状态报文"; }
	Multi_frame& operator=(const Multi_frame&mf) { return Multi_frame::operator=(mf); }
	virtual string typeName()const { return "BCS"; }
	virtual string get_detail()const;
	virtual BOOL rec_finish()const;
	virtual BOOL valid()const;
	virtual ~CAN_BCS() {}
};

#endif//CANDATA_H