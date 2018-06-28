/*
 * CANdata.cpp
 *
 *  Created on: 2017��11��29��
 *      Author: Administrator
 */
#include"CANdata.h"
#include<sstream>
#include<iomanip>
#include<stdlib.h>
#pragma execution_character_set("utf-8")
using namespace std;
UINT DATA_BASE=7, CANID_BASE=3,LENGTH_BASE=6;
CONST_CHARPTR Basic_CAN_frame::states[8]={"������ֽ׶�","����ʶ�׶�","������ý׶�","���׶�","�������׶�","������","��֡����","δ����"};
int Basic_CAN_frame::getState(const char* str) {
	for (int i = 0; i < 7; i++) {
		if (strcmp(states[i], str) == 0) {
			return i;
		}
	}
	return -1;
}
Basic_CAN_frame *Basic_CAN_frame::create_frame(DWORD can_id){
	switch(can_id){
	case CHM_CANID:
		return new CAN_CHM();
	case BHM_CANID:
		return new CAN_BHM();
	case CRM_CANID:
		return new CAN_CRM();
	case CTS_CANID:
		return new CAN_CTS();
	case CML_CANID:
		return new CAN_CML();
	case BRO_CANID:
		return new CAN_BRO();
	case CRO_CANID:
		return new CAN_CRO();
	case BCL_CANID:
		return new CAN_BCL();
	case CCS_CANID:
		return new CAN_CCS();
	case BSM_CANID:
		return new CAN_BSM();
	case BST_CANID:
		return new CAN_BST();
	case CST_CANID:
		return new CAN_CST();
	case BSD_CANID:
		return new CAN_BSD();
	case CSD_CANID:
		return new CAN_CSD();
	case BEM_CANID:
		return new CAN_BEM();
	case CEM_CANID:
		return new CAN_CEM();
	case TP_START_CANID:
		return new CAN_TP_START();
	case TP_REPLY_CANID:
		return new CAN_TP_REPLY();
	case TP_DATA_CANID:
		return new CAN_TP_DATA();
	default:
		return new CAN_UNDEF(can_id);
	}
}
//================================= ���ֽ׶� ===========================//
//-------------- CHM ----------------//
BOOL CAN_CHM::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==3&& tf.getNumField()>= 10){
		vmajor = tf.getByteHex( DATA_BASE + 0 );
		vminor = tf.getByteHex(DATA_BASE + 1 ) + (tf.getByteHex(DATA_BASE + 2)<<8);
		return true;
	}
	return false;
}
string CAN_CHM::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		if (vmajor != 0xff || vminor != 0xffff) {
			oss << (UINT)vmajor << '.' << vminor;
		}
		else {
			oss << "�޷�ʶ��";
		}
		return oss.str().c_str();
	default:
		return "";
	}
}
const char* CAN_CHM::detailName(int i) const{
	switch (i) {
	case 0:
		return "���׮Э��汾��";
	default:
		return "";
	}
}
string CAN_CHM::get_detail()const{
		ostringstream oss;
		oss<<"���׮Э��汾�ţ�";
		if(vmajor != 0xff || vminor != 0xffff)
			oss<<'v'<<(UINT)vmajor<<'.'<<vminor<<"...";
		else
			oss<<"�޷�ʶ��";
		return oss.str();
}
BOOL CAN_CHM::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_CHM& tmp=dynamic_cast<CAN_CHM&>(cf);
	return tmp.state==state&&tmp.vmajor==vmajor && tmp.vminor==vminor;
}
Basic_CAN_frame& CAN_CHM::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_CHM & chm = dynamic_cast<CAN_CHM&> (can);
	index = chm.index;
	vmajor = chm.vmajor;
	vminor = chm.vminor;
	return *this;
}
//-------------- BHM ---------------//
BOOL CAN_BHM::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==2&& tf.getNumField()>= 9){
		hvol = tf.getByteHex( DATA_BASE + 0 )+(tf.getByteHex(DATA_BASE + 1 )<<8);
		return true;
	}
	return false;
}
string CAN_BHM::get_detail()const{
		ostringstream oss;
		oss<<"BMS��������ѹ��";
		if(hvol==0xffff)
			oss<<"δ��ʼ��;";
		else
			oss<<double(hvol)*0.1<<'V';
		return oss.str();
}
string CAN_BHM::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		if (hvol != 0xffff) {
			oss << double(hvol)*0.1 << 'V';
		}
		else {
			oss << "δ��ʼ��;";
		}
		return oss.str().c_str();
	default:
		return "";
	}
}
const char* CAN_BHM::detailName(int i) const {
	switch (i) {
	case 0:
		return "BMS��������ѹ";
	default:
		return "";
	}
}
BOOL CAN_BHM::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_BHM& tmp=dynamic_cast<CAN_BHM&>(cf);
	return tmp.hvol == hvol;
}
Basic_CAN_frame& CAN_BHM::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_BHM & bhm = dynamic_cast<CAN_BHM&> (can);
	index = bhm.index ;
	hvol = bhm.hvol;
	return *this;
}
//========================== ��ʶ�׶� =============================//
//-------------- BRM ��֡���� ------------------//
 string CAN_BRM::get_detail()const{
		 ostringstream oss;
		 oss<<"ͨ��Э��汾�ţ�v"<<(UINT)getByte(0)<<'.'<<getWord(1)<<".";
		 oss<<"�������:";
		 switch(getByte(3)){
		 case 0x01:
			oss<<"Ǧ����.";
			break;
		 case 0x02:
			oss<<"������.";
			break;
		 case 0x03:
		 	oss<<"������.";
		 	break;
		 case 0x04:
		 	oss<<"������";
		 	break;
		 case 0x05:
		 	oss<<"������.";
		 	break;
		 case 0x06:
			oss<<"��Ԫ���ϵ��.";
		 	break;
		 case 0x07:
		 	oss<<"�ۺ���﮵��.";
		 	break;
		 case 0x08:
		 	oss<<"������.";
		 	break;
		 case 0xff:
			oss<<"δ֪���.";
			break;
		 default:
			oss<<"����!!!";
		 }
		 oss<<"�����������ض����:"<< 0.1 * getWord(4) << "A��h."<<"���ѹΪ:"<< 0.1 * getWord(6)<<"V.";
		BYTE name[5]; name[4] = '\0';for(UINT i = 0 ; i < 4 ; i++)name[i]=getByte( 8 + i);
		if(name[0]!=0xff)
			oss<<"����������̡���"<<name<<";";
		UINT num = getDWord(12);
		if(num != 0xffffffff)
			oss<<"�������ţ�0x"<<hex<<setw(8)<<num<<';'<<dec;
		for(UINT i = 0 ; i < 3 ; i++) name[ i] = getByte(16+i);
		if(name[0]!=0xff || name[1]!=0xff||name[2]!=0xff)
			oss<<"��������:"<<1985 + name[0]<<"��"<<(UINT)name[1]<<"��"<<(UINT)name[2]<<"��;";
		num = getByte(19)+(getWord(20)<<8);
		if(num != 0xffffff){
			oss<<"������:"<<num<<"��;";
		}
		switch(getByte(22)){
		case 0:
			oss<<"��ز�Ȩ-����.";
			break;
		case 1:
			oss<<"��ز�Ȩ-��������.";
			break;
		}
		oss<<"����ʶ���:";
		for(UINT i = 0; i < 17; i++)
			oss<<getByte(24 + i);
		oss<<".\t";
		oss<<"BMS����汾�ţ�"<<oct<<(getByte(44)<<8)+getByte(45)<<"��"<<(UINT)getByte(43)<<"��"<<(UINT)getByte(42)<<"�գ���"<<(UINT)getByte(41)<<"�α���.";
	 return oss.str();
 }
BOOL CAN_BRM::rec_finish()const{
	if(frames.size() < BRM_TP_SIZE)
		return false;
	return true;
}
BOOL CAN_BRM::valid()const{
	UINT size=frames.size();
	if( size >= 3 && size <= BRM_TP_SIZE - 1  ){
		if(frames[ size - 1 ]->canid != TP_DATA_CANID)
			return false;
		CAN_TP_DATA* tmp_d = dynamic_cast<CAN_TP_DATA*>(frames[size -1 ]);
		return tmp_d->packet == size - 2  ;
	}else if(size == 1){
		if((frames[0])->canid != TP_START_CANID )
			return false;
		CAN_TP_START* tmp_s = dynamic_cast<CAN_TP_START*>(frames[0]);
		return tmp_s->console == TP_START_CONSOLE && tmp_s->PGN == BRM_PGN && tmp_s->length == 49 && tmp_s->packets == BRM_TP_SIZE - 3;
	}else if(size == 2){
		if((frames[1])->canid != TP_REPLY_CANID )
			return false;
		CAN_TP_REPLY* tmp_r = dynamic_cast<CAN_TP_REPLY*>(frames[1]);
		return tmp_r->compts.reply.console == TP_REPLY_CONSOLE && tmp_r->compts.reply.PGN == BRM_PGN && tmp_r->compts.reply.packets== BRM_TP_SIZE - 3 && tmp_r->compts.reply.nextpacket == 1;
	}else if(size == BRM_TP_SIZE){
		if((frames[ BRM_TP_SIZE - 1])->canid != TP_REPLY_CANID )
			return false;
		CAN_TP_REPLY* tmp_r = dynamic_cast<CAN_TP_REPLY*>(frames[ BRM_TP_SIZE - 1]);
		return tmp_r->compts.end.console == TP_END_CONSOLE && tmp_r->compts.end.PGN == BRM_PGN && tmp_r->compts.end.packets== BRM_TP_SIZE - 3 && tmp_r->compts.end.length == 49 ;
	}else  return false;
}
//--------------- CRM ---------------//
BOOL CAN_CRM::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==8 && tf.getNumField()>= 15){
		rec = tf.getByteHex( DATA_BASE + 0 );
		mnum=tf.getByteHex(DATA_BASE + 1 )+(tf.getByteHex(DATA_BASE + 2 )<<8)+(tf.getByteHex(DATA_BASE + 3 )<<16) +(tf.getByteHex(DATA_BASE + 4 )<<24);
		mloc=tf.getByteHex(DATA_BASE + 5 )+(tf.getByteHex(DATA_BASE + 6 )<<8)+(tf.getByteHex(DATA_BASE + 7 )<<16) ;
		return true;
	}
	return false;
}
string CAN_CRM::get_detail()const{
		ostringstream oss;
		oss<<"BMS��ʶ���:";
		switch(rec){
		case 0x00:
			oss<<"δ��ʶ!";
			break;
		case 0xAA:
			oss<<"�ɹ���ʶ...";
			break;
		default:
			oss<<"δ֪!";
		}
		if(mnum!=0xffffffff)
			oss<<"�������:0x"<<hex<<setw(8)<<mnum<<".\t";
		if(mloc !=0xffffff)
			oss<<"�����������:0x"<<setw(6)<<hex<<mloc<<'.';
		return oss.str();
}
string CAN_CRM::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		switch (rec) {
		case 0x00:
			oss << "δ��ʶ!";
			break;
		case 0xAA:
			oss << "�ɹ���ʶ...";
			break;
		default:
			oss << "δ֪!";
		}
		break;
	case 1:
			oss << hex <<"0x"<< setw(8) << mnum;
		break;
	case 2:
			oss << "0x" << setw(6) << hex << mloc;
		break;
	default:
		oss << "";
		break;
	}
	return oss.str().c_str();
}
const char* CAN_CRM::detailName(int i) const {
	switch (i) {
	case 0:
		return "BMS��ʶ���";
	case 1:
		return "�������";
	case 2:
		return "�����������";
	default:
		return "";
	}
}
BOOL CAN_CRM::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_CRM& tmp=dynamic_cast<CAN_CRM&>(cf);
	return tmp.rec == rec && tmp.mnum == mnum && tmp.mloc == mloc;
}
Basic_CAN_frame& CAN_CRM::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_CRM & crm = dynamic_cast<CAN_CRM&> (can);
	index = crm.index ;
	rec = crm.rec ;
	mnum = crm.mnum;
	mloc = crm.mloc;
	return *this;
}
//========================== ���ý׶� =======================//
//-------------- BCP ��֡���� ------------------//
 string CAN_BCP::get_detail()const{
	ostringstream oss;
	oss<<"���嶯��������������ѹ:"<<0.01*getWord(0)<<"V.";
	oss<<"������������Ϊ:"<<0.1*getWord(2) - 400<<"A,��������ܵ�ѹΪ"<<0.1 *getWord(6)<<"V.�������ر��������Ϊ"<<0.1*getWord(4)<<"Kw��h.";
	oss<<"��������¶�:"<<(UINT)getByte(8)-50<<"���϶�.��ǰ��ص���(SOC):"<<0.1*(UINT)getWord(9)<<"%,�ܵ�ѹ:"<<0.1*getWord(11)<<"V.";
	return oss.str();
 }
BOOL CAN_BCP::rec_finish()const{
	if(frames.size() < BCP_TP_SIZE)
		return false;
	return true;
}
BOOL CAN_BCP::valid()const{
	UINT size=frames.size();
	if( size >= 3 && size <= BCP_TP_SIZE - 1  ){
		if(frames[ size - 1 ]->canid != TP_DATA_CANID)
			return false;
		CAN_TP_DATA* tmp_d = dynamic_cast<CAN_TP_DATA*>(frames[size -1 ]);
		return tmp_d->packet == size - 2  ;
	}else if(size == 1){
		if((frames[0])->canid != TP_START_CANID )
			return false;
		CAN_TP_START* tmp_s = dynamic_cast<CAN_TP_START*>(frames[0]);
		return tmp_s->console == TP_START_CONSOLE && tmp_s->PGN == BCP_PGN && tmp_s->length == 13 && tmp_s->packets == BCP_TP_SIZE - 3;
	}else if(size == 2){
		if((frames[1])->canid != TP_REPLY_CANID )
			return false;
		CAN_TP_REPLY* tmp_r = dynamic_cast<CAN_TP_REPLY*>(frames[1]);
		return tmp_r->compts.reply.console == TP_REPLY_CONSOLE && tmp_r->compts.reply.PGN == BCP_PGN && tmp_r->compts.reply.packets== BCP_TP_SIZE - 3 && tmp_r->compts.reply.nextpacket == 1;
	}else if(size == BCP_TP_SIZE){
		if((frames[ BCP_TP_SIZE - 1])->canid != TP_REPLY_CANID )
			return false;
		CAN_TP_REPLY* tmp_r = dynamic_cast<CAN_TP_REPLY*>(frames[ BCP_TP_SIZE - 1]);
		return tmp_r->compts.end.console == TP_END_CONSOLE && tmp_r->compts.end.PGN == BCP_PGN && tmp_r->compts.end.packets== BCP_TP_SIZE - 3 && tmp_r->compts.end.length == 49 ;
	}else return false;
}
//---------------- CTS -------------------//
static inline UINT GET_BCD_BYTE(BYTE b){
	return ((b>>4)&0x0f) * 10 + (b&0x0f);
}
static UINT GET_BCD_WORD(WORD b){
	return GET_BCD_BYTE((b>>8)&0xff)*100+GET_BCD_BYTE((b)&0xff);
}
BOOL CAN_CTS::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==7 && tf.getNumField()>= 14){
		second = tf.getByteHex( DATA_BASE + 0 );
		minute=tf.getByteHex(DATA_BASE + 1 );
		hour=tf.getByteHex(DATA_BASE + 2 );
		day=tf.getByteHex(DATA_BASE + 3 );
		month=tf.getByteHex(DATA_BASE + 4 );
		year=tf.getByteHex(DATA_BASE + 5 )+(tf.getByteHex(DATA_BASE + 6 )<<8);
		return true;
	}
	return false;
}
string CAN_CTS::get_detail()const{
		ostringstream oss;
		if(second != 0xff || minute != 0xff || hour != 0xff || day != 0xff || month != 0xff || year != 0xffff)
			oss<<"ͬ��ʱ��:\t"<<setw(4)<<GET_BCD_WORD(year)<<"��"<<setw(2)<<(UINT)GET_BCD_BYTE(month)<<"��"<<setw(2)<<(UINT)GET_BCD_BYTE(day)<<"��"<<setw(2)<<(UINT)GET_BCD_BYTE(hour)<<"ʱ"<<setw(2)<<(UINT)GET_BCD_BYTE(minute)<<"��"<<setw(2)<<(UINT)GET_BCD_BYTE(second)<<"��";
		return oss.str();
}
string CAN_CTS::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		oss << setw(4) << GET_BCD_WORD(year) << "��" << setw(2) << (UINT)GET_BCD_BYTE(month) << "��" << setw(2) << (UINT)GET_BCD_BYTE(day) << "��" << setw(2) << (UINT)GET_BCD_BYTE(hour) << "ʱ" << setw(2) << (UINT)GET_BCD_BYTE(minute) << "��" << setw(2) << (UINT)GET_BCD_BYTE(second) << "��";
		break;
	}
	return oss.str().c_str();
}
const char* CAN_CTS::detailName(int i) const {
	switch (i) {
	case 0:
		return "ͬ��ʱ�� : ";
	default:
		return "";
	}
}
BOOL CAN_CTS::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_CTS& tmp=dynamic_cast<CAN_CTS&>(cf);
	return tmp.second== second && tmp.minute == minute && tmp.hour == hour && tmp.day == day && tmp.month == month && tmp.year == year;
}
Basic_CAN_frame& CAN_CTS::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_CTS & cts = dynamic_cast<CAN_CTS&> (can);
	index =cts.index;
	second = cts.second  ;
	minute = cts.minute;
	hour = cts.hour;
	day = cts.day ;
	month = cts.month ;
	year = cts.year ;
	return *this;
}
//------------------ CML --------------------//
BOOL CAN_CML::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==8 && tf.getNumField()>= 15){
		houtvol = tf.getByteHex( DATA_BASE + 0 )+(tf.getByteHex(DATA_BASE + 1 )<<8);
		loutvol=tf.getByteHex(DATA_BASE + 2 ) + (tf.getByteHex(DATA_BASE + 3 )<<8);
		houtcur=tf.getByteHex(DATA_BASE + 4 ) + (tf.getByteHex(DATA_BASE + 5 )<<8);
		loutcur=tf.getByteHex(DATA_BASE + 6 )+(tf.getByteHex(DATA_BASE + 7 )<<8);
		return true;
	}
	return false;
}
string CAN_CML::get_detail()const{
		ostringstream oss;
		if(houtvol != 0xffff)
			oss<<"���׮��������ѹ�� "<<0.1 *houtvol<<"V.";
		if(loutvol != 0xffff)
			oss<<"���׮��������ѹ:"<<0.1 * loutvol <<"V.";
		if(houtcur != 0xffff)
			oss<<"���׮����������"<< 0.1 * houtcur - 400 << "A.";
		if(loutcur != 0xffff)
			oss<<"���׮��С�������"<< 0.1 * loutcur - 400 << "A.";
		return oss.str();
}
string CAN_CML::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		if (houtvol != 0xffff)
			oss << 0.1 *houtvol << "V.";
		break;
	case 1:
		if (loutvol != 0xffff)
			oss << 0.1 * loutvol << "V.";
		break;
	case 2:
		if (houtcur != 0xffff)
			oss << 0.1 * houtcur - 400 << "A.";
		break;
	case 3:
		if (loutcur != 0xffff)
			oss << 0.1 * loutcur - 400 << "A.";
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_CML::detailName(int i) const {
	switch (i) {
	case 0:
		return "���׮��������ѹ";
	case 1:
		return "���׮��������ѹ";
	case 2:
		return "���׮����������";
	case 3:
		return "���׮��С�������";
	default:
		return "";
	}
}
BOOL CAN_CML::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_CML& tmp=dynamic_cast<CAN_CML&>(cf);
	return tmp.houtvol== houtvol && tmp.loutvol == loutvol && tmp.houtcur == houtcur && tmp.loutcur == loutcur;
}
Basic_CAN_frame& CAN_CML::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_CML & cml = dynamic_cast<CAN_CML&> (can);
	index =cml.index;
	houtvol = cml.houtvol;
	loutvol = cml.loutvol;
	houtcur = cml.houtcur;
	loutcur = cml.loutcur;
	return *this;
}
//---------------- BRO -------------------//
BOOL CAN_BRO::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==1 && tf.getNumField()>= 8){
		rec = tf.getByteHex( DATA_BASE + 0 );
		return true;
	}
	return false;
}
string CAN_BRO::get_detail()const{
	ostringstream oss;
	oss<<"BMS������״̬--";
	switch(rec){
	case 0x00:
		oss<<"δ����!";
		break;
	case 0xAA:
		oss<<"׼������...";
		break;
	default:
		oss<<"δ֪!";
			}
	return oss.str();
}
string CAN_BRO::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		switch (rec) {
		case 0x00:
			oss << "δ����!";
			break;
		case 0xAA:
			oss << "׼������...";
			break;
		default:
			oss << "δ֪״̬!";
		}
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_BRO::detailName(int i) const {
	switch (i) {
	case 0:
		return "BMS������״̬";
	default:
		return "";
	}
}
BOOL CAN_BRO::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_BRO& tmp=dynamic_cast<CAN_BRO&>(cf);
	return tmp.rec== rec ;
}
Basic_CAN_frame& CAN_BRO::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_BRO & bro = dynamic_cast<CAN_BRO&> (can);
	index =bro.index;
	rec= bro.rec ;
	return *this;
}
//------------------ CRO ------------------//
BOOL CAN_CRO::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==1 && tf.getNumField()>= 8){
		rec = tf.getByteHex( DATA_BASE + 0 );
		return true;
	}
	return false;
}
string CAN_CRO::get_detail()const{
	ostringstream oss;
	oss<<"���׮������״̬--";
	switch(rec){
	case 0x00:
		oss<<"δ����!";
		break;
	case 0xAA:
		oss<<"׼������...";
		break;
	default:
		oss<<"δ֪!";
	}
	return oss.str();
}
string CAN_CRO::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		switch (rec) {
		case 0x00:
			oss << "δ����!";
			break;
		case 0xAA:
			oss << "׼������...";
			break;
		default:
			oss << "δ֪״̬!";
		}
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_CRO::detailName(int i) const {
	switch (i) {
	case 0:
		return "���׮������״̬";
	default:
		return "";
	}
}
BOOL CAN_CRO::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_CRO& tmp=dynamic_cast<CAN_CRO&>(cf);
	return tmp.rec== rec ;
}
Basic_CAN_frame& CAN_CRO::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_CRO & cro = dynamic_cast<CAN_CRO&> (can);
	index =cro.index;
	rec =cro.rec ;
	return *this;
}
//============================== ���׶� ============================//
//------------------- BCL -------------------//
BOOL CAN_BCL::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==5 && tf.getNumField()>= 12){
		nvol = tf.getByteHex( DATA_BASE + 0 )+(tf.getByteHex(DATA_BASE + 1 )<<8);
		ncur = tf.getByteHex(DATA_BASE + 2 ) + (tf.getByteHex(DATA_BASE + 3 )<<8);
		cmod = tf.getByteHex(DATA_BASE + 4 );
		return true;
	}
	return false;
}
string CAN_BCL::get_detail()const{
	ostringstream oss;
	if(nvol != 0xffff)
		oss<<"BMS�����ѹ:"<<0.1 *nvol<<"V.";
	if(ncur != 0xffff)
		oss<<"BMS�������:"<< 0.1 * ncur - 400 << "A.";
	oss<<"���ģʽ--";
	switch(cmod){
		case 0x01:
			oss<<"��ѹ���!";
			break;
		case 0x02:
			oss<<"�������...";
			break;
		default:
			oss<<"δ֪!";
	}
	return oss.str();
}
string CAN_BCL::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		oss <<  0.1 *nvol << "V.";
		break;
	case 1:
		oss << 0.1 * ncur - 400 << "A.";
		break;
	case 2:
		switch (cmod) {
		case 0x01:
			oss << "��ѹ���!";
			break;
		case 0x02:
			oss << "�������...";
			break;
		default:
			oss << "δ֪���ģʽ!";
		}
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_BCL::detailName(int i) const {
	switch (i) {
	case 0:
		return "BMS�����ѹ";
	case 1:
		return "BMS�������";
	case 2:
		return "���ģʽ";
	default:
		return "";
	}
}
BOOL CAN_BCL::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_BCL& tmp=dynamic_cast<CAN_BCL&>(cf);
	return tmp.nvol== nvol && tmp.ncur == ncur && tmp.cmod == cmod;
}
Basic_CAN_frame& CAN_BCL::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_BCL & bcl = dynamic_cast<CAN_BCL&> (can);
	index =bcl.index;
	nvol =bcl.nvol ;
	ncur = bcl.ncur;
	cmod =bcl.cmod ;
	return *this;
}
//-------------- BCS ��֡���� ------------------//
 string CAN_BCS::get_detail()const{
	ostringstream oss;
	oss<<"����ѹ����ֵ:"<<0.1*getWord(0)<<"V.";
	oss<<"����������ֵ:"<<0.1 * getWord(2) - 400<<"A.��ߵ������ص�ѹ�������:("<<0.1 * (0xfff & getWord(4))<<"V,0x"<<(0xf&(getWord(4)>>12))<<").��ǰ�ɵ�״̬(SOC):"<<(UINT)getByte(6)<<"%.Ŀǰʣ����ʱ��Ϊ"<<getWord(7)<<"����.";
	return oss.str();
 }
BOOL CAN_BCS::rec_finish()const{
	if(frames.size() < BCS_TP_SIZE)
		return false;
	return true;
}
BOOL CAN_BCS::valid()const{
	UINT size=frames.size();
	if( size >= 3 && size <= BCS_TP_SIZE - 1  ){
		if(frames[ size - 1 ]->canid != TP_DATA_CANID)
			return false;
		CAN_TP_DATA* tmp_d = dynamic_cast<CAN_TP_DATA*>(frames[size -1 ]);
		return tmp_d->packet == size - 2  ;
	}else if(size == 1){
		if((frames[0])->canid != TP_START_CANID )
			return false;
		CAN_TP_START* tmp_s = dynamic_cast<CAN_TP_START*>(frames[0]);
		return tmp_s->console == TP_START_CONSOLE && tmp_s->PGN == BCS_PGN && tmp_s->length == 13 && tmp_s->packets == BCS_TP_SIZE - 3;
	}else if(size == 2){
		if((frames[1])->canid != TP_REPLY_CANID )
			return false;
		CAN_TP_REPLY* tmp_r = dynamic_cast<CAN_TP_REPLY*>(frames[1]);
		return tmp_r->compts.reply.console == TP_REPLY_CONSOLE && tmp_r->compts.reply.PGN == BCS_PGN && tmp_r->compts.reply.packets== BCS_TP_SIZE - 3 && tmp_r->compts.reply.nextpacket == 1;
	}else if(size == BCS_TP_SIZE){
		if((frames[ BCS_TP_SIZE - 1])->canid != TP_REPLY_CANID )
			return false;
		CAN_TP_REPLY* tmp_r = dynamic_cast<CAN_TP_REPLY*>(frames[ BCS_TP_SIZE - 1]);
		return tmp_r->compts.end.console == TP_END_CONSOLE && tmp_r->compts.end.PGN == BCS_PGN && tmp_r->compts.end.packets== BCS_TP_SIZE - 3 && tmp_r->compts.end.length == 49 ;
	}else return false;
}
//---------------- CCS ------------------//
BOOL CAN_CCS::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==7 && tf.getNumField()>= 14){
		ovol = tf.getByteHex( DATA_BASE + 0 )+(tf.getByteHex(DATA_BASE + 1 )<<8);
		ocur = tf.getByteHex(DATA_BASE + 2 ) + (tf.getByteHex(DATA_BASE + 3 )<<8);
		chargetime = tf.getByteHex(DATA_BASE + 4 ) + (tf.getByteHex(DATA_BASE + 5 )<<8);
		chargeallow = tf.getByteHex(DATA_BASE + 6);
		return true;
	}
	return false;
}
string CAN_CCS::get_detail()const{
	ostringstream oss;
	if(ovol != 0xffff)
		oss<<"��ѹ���Ϊ"<<0.1 * ovol<<"V.";
	if(ocur != 0xffff)
		oss<<"�������Ϊ"<< 0.1 * ocur - 400 << "A.";
	if(chargetime != 0xffff)
			oss<<"Ŀǰ�Ѿ����"<< chargetime << "����.";
	oss<<"��ǰ���״̬--";
	switch(chargeallow&0x03){
		case 0x00:
			oss<<"��ֹ���!";
			break;
		case 0x01:
			oss<<"���ڳ��...";
			break;
		default:
			oss<<"δ֪!";
	}
	return oss.str();
}
string CAN_CCS::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		if (ovol != 0xffff)
			oss <<  0.1 * ovol << "V.";
		break;
	case 1:
		if (ocur != 0xffff)
			oss  << 0.1 * ocur - 400 << "A.";
		break;
	case 2:
		oss << chargetime << "����.";
		break;
	case 3:
		switch (chargeallow & 0x03) {
		case 0x00:
			oss << "��ֹ���!";
			break;
		case 0x01:
			oss << "���ڳ��...";
			break;
		default:
			oss << "δ֪���״̬!";
		}
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_CCS::detailName(int i) const {
	switch (i) {
	case 0:
		return "��ѹ���";
	case 1:
		return "�������";
	case 2:
		return "�ѳ��ʱ��";
	case 3:
		return "���״̬";
	default:
		return "";
	}
}
BOOL CAN_CCS::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_CCS& tmp=dynamic_cast<CAN_CCS&>(cf);
	return tmp.ovol== ovol && tmp.ocur == ocur && tmp.chargetime == chargetime && tmp.chargeallow == chargeallow;
}
Basic_CAN_frame& CAN_CCS::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_CCS & ccs = dynamic_cast<CAN_CCS&> (can);
	index =ccs.index;
	ovol = ccs.ovol;
	ocur = ccs.ocur;
	chargetime =ccs.chargetime;
	chargeallow = ccs.chargeallow;
	return *this;
}
//------------------ BSM ----------------//
BOOL CAN_BSM::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==7 && tf.getNumField()>= 14){
		maxvolid = tf.getByteHex( DATA_BASE + 0 ) ;
		maxtem = tf.getByteHex(DATA_BASE + 1 );
		maxtemid = tf.getByteHex(DATA_BASE + 2 ) ;
		mintem = tf.getByteHex(DATA_BASE + 3 );
		mintemid = tf.getByteHex(DATA_BASE + 4 ) ;
		chargestate.data =tf.getByteHex(DATA_BASE + 5 ) + (tf.getByteHex(DATA_BASE + 6 ) << 8 );
		return true;
	}
	return false;
}
string CAN_BSM::get_detail()const{
	ostringstream oss;
	if(maxvolid != 0xff)
		oss<<"��ǰ������ص�ѹ��ţ�0x"<<hex<<setw(2)<<(UINT)maxvolid<<".";
	if(maxtem != 0xff)
		oss<<"��ߵ���¶ȱ��:0x"<< hex<<setw(2)<< (UINT)maxtemid << ",���¶�Ϊ:"<<(UINT)maxtem - 50<<"���϶�.";
	if(mintem != 0xff)
			oss<<"��͵���¶ȱ��:0x"<< hex<<setw(2)<< (UINT)mintemid << ",���¶�Ϊ:"<<(UINT)mintem - 50<<"���϶�.";
	oss<<"��ǰ��������״̬--";
	if(chargestate.data == 0xD000)
		oss<<"������";
	else{
		switch(chargestate.compts.volstate){
		case 0:
		oss<<"��ѹ����,";
		break;
		case 1:
		oss<<"��ѹ����,";
		break;
		}
		switch(chargestate.compts.chargestate){
		case 1:
			oss<<"�ɵ�״̬����,";
			break;
		case 2:
			oss<<"�ɵ�״̬����,";
			break;
		}
		switch(chargestate.compts.curstate){
		case 1:
			oss<<"������,";
			break;
		case 2:
			oss<<"��������־λ������,";
			break;
		}
		switch(chargestate.compts.temstate){
		case 1:
			oss<<"�¶ȹ���,";
			break;
		case 2:
			oss<<"�¶ȱ�־λ������,";
			break;
		}
		switch(chargestate.compts.insultstate){
			case 1:
				oss<<"��Ե״̬����,";
				break;
			case 2:
				oss<<"��Ե״̬��־λ������,";
				break;
		}
		switch(chargestate.compts.OCS){
			case 1:
				oss<<"���������������,";
				break;
			case 2:
				oss<<"���������״̬��־λ������,";
				break;
		}
		switch(chargestate.compts.CA){
			case 1:
				oss<<"�����硣";
				break;
			case 0:
				oss<<"��ֹ��硣";
				break;
			case 2:
				oss<<"�������״̬δ֪.";
		}
	}
	return oss.str();
}
string CAN_BSM::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		oss << hex<<setw(2)<<"0x"<<setfill('0')<<(UINT)maxtemid ;
		break;
	case 1:
			oss<< (UINT)maxtem - 50 << "��";
		break;
	case 2:
		oss << hex << setw(2) << "0x" << setfill('0') << (UINT)mintemid;
		break;
	case 3:
		oss << (UINT)mintem - 50 << "��";
		break;
	case 4:
		oss << hex << setw(2)<<"0x"<<setfill('0') << (UINT)maxvolid << ".";
		break;
	case 5:
		oss << DETAIL_BOOLS_NAME;
		break;
	case 6:
		switch (chargestate.compts.volstate) {
		case 0:
			oss << DETAIL_FALSE ;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 7:
		switch (chargestate.compts.volstate) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 8:
		switch (chargestate.compts.chargestate) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 9:
		switch (chargestate.compts.chargestate) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 10:
		switch (chargestate.compts.curstate) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 11:
		switch (chargestate.compts.temstate) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 12:
		switch (chargestate.compts.insultstate) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 13:
		switch (chargestate.compts.OCS) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 14:
			switch (chargestate.compts.CA) {
			case 0:
				oss << DETAIL_FALSE;
				break;
			case 1:
				oss << DETAIL_TRUE;
				break;
			default:
				oss << DETAIL_UNKNOW;
			}
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_BSM::detailName(int i) const {
	switch (i) {
	case 0:
		return "��ߵ���¶ȱ��";
	case 1:
		return "��ߵ���¶�";
	case 2:
		return "��͵���¶ȱ��";
	case 3:
		return "��͵���¶�";
	case 4:
		return "��ǰ������ص�ѹ���";
	case 5:
		return "��ǰ��������״̬";
	case 6:
		return "��ѹ����";
	case 7:
		return "��ѹ����";
	case 8:
		return "�ɵ�״̬����";
	case 9:
		return "�ɵ�״̬����";
	case 10:
		return "������";
	case 11:
		return "�¶ȹ���";
	case 12:
		return "��Ե״̬������";
	case 13:
		return "��������������������";
	case 14:
		return "������";
	default:
		return "";
	}
}
BOOL CAN_BSM::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_BSM& tmp=dynamic_cast<CAN_BSM&>(cf);
	return  tmp.maxtem == maxtem  && tmp.mintem == mintem && tmp.chargestate.data == chargestate.data && tmp.maxvolid== maxvolid && tmp.maxtemid == maxtemid && tmp.mintemid == mintemid;
}
Basic_CAN_frame& CAN_BSM::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_BSM & bsm = dynamic_cast<CAN_BSM&> (can);
	index =bsm.index;
	maxvolid =  bsm.maxvolid ;
	maxtem = bsm.maxtem ;
	maxtemid = bsm.maxtemid;
	mintem = bsm.mintem ;
	mintemid = bsm.mintemid;
	chargestate.data = bsm.chargestate.data ;
	return *this;
}
//--------------- BST -------------------//
BOOL CAN_BST::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==4 && tf.getNumField()>= 11){
		stopinfo.data = tf.getByteHex( DATA_BASE + 0 ) ;
		faultinfo.data[ 0 ] = tf.getByteHex(DATA_BASE + 1 ) ;
		faultinfo.data[ 1 ] = tf.getByteHex(DATA_BASE + 2 ) ;
		stopfaultinfo.data = tf.getByteHex(DATA_BASE + 3 ) ;
		return true;
	}
	return false;
}
string CAN_BST::get_detail()const{
	ostringstream oss;
	oss<<"BMS��ֹ���ԭ��--";
	if(stopinfo.data==0)
		oss<<"��;";
	else{
		if(stopinfo.compts.stopbySOC == 1)
			oss<<"����SOCֵ�Ѵﵽ;";
		if(stopinfo.compts.stopbyVT == 1)
			oss<<"�ܵ�ѹ�趨ֵ�Ѵﵽ;";
		if(stopinfo.compts.stopbyVN == 1)
			oss<<"�����ص�ѹ�趨ֵ�Ѵﵽ;";
		if(stopinfo.compts.stopbyCM == 1)
			oss<<"���׮����Ҫ����ֹ���;";
	}
	oss<<"���׮��ֹ���Ĺ���ԭ��--";
	if(faultinfo.data[0] == 0 && faultinfo.data[1] == 0)
		oss<<"��.";
	else{
		switch(faultinfo.compt.insultFault){
		case 1:
			oss<<"��Ե����,";
			break;
		case 2:
			oss<<"��Ե���ϱ�־λ������,";
			break;
		}
		switch(faultinfo.compt.OCTFault){
		case 1:
			oss<<"�������������,";
			break;
		case 2:
			oss<<"�����������־λ������,";
			break;
		}
		switch(faultinfo.compt.BMStemFault){
		case 1:
			oss<<"BMS���������������,";
			break;
		case 2:
			oss<<"BMS�������������־λ������,";
			break;
		}
		switch(faultinfo.compt.CCFault){
			case 1:
				oss<<"�������������,";
				break;
			case 2:
				oss<<"�����������־λ������,";
				break;
		}
		switch(faultinfo.compt.BTFault){
			case 1:
				oss<<"������¶ȹ���,";
				break;
			case 2:
				oss<<"������¶�״̬��־λ������,";
				break;
		}
		switch(faultinfo.compt.HVolFault){
		case 1:
				oss<<"��ѹ�̵������ϡ�";
				break;
		case 2:
			oss<<"��ѹ�̵�����־λ�쳣��";
			break;
		}
		switch(faultinfo.compt.T2VolFault){
		case 1:
			oss<<"����2��ѹ�����ϡ�";
				break;
		case 2:
			oss<<"����2��ѹ״̬��־λ�쳣��";
			break;
		}
		switch(faultinfo.compt.OtherFault){
		case 1:
			oss<<"�������ϡ�";
				break;
		case 2:
			oss<<"�������ϱ�־λ�쳣��";
			break;
		}
	}
	oss<<"BMS��ֹ������ԭ��--";
	if((stopfaultinfo.data&0x0f) == 0)
		oss<<"��.";
	else{
		switch(stopfaultinfo.compt.curfault){
		case 1:
			oss<<"��������,";
			break;
		case 2:
			oss<<"���������־λ�쳣,";
			break;
		}
		switch(stopfaultinfo.compt.volfault){
		case 1:
			oss<<"��ѹ�쳣.";
			break;
		case 2:
			oss<<"��ѹ�쳣��־λ�쳣.";
			break;
		}
	}
	return oss.str();
}
string CAN_BST::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		if (stopinfo.data == 0)
			oss << "��;";
		else {
			if (stopinfo.compts.stopbySOC == 1)
				oss << "����SOCֵ�Ѵﵽ;";
			if (stopinfo.compts.stopbyVT == 1)
				oss << "�ܵ�ѹ�趨ֵ�Ѵﵽ;";
			if (stopinfo.compts.stopbyVN == 1)
				oss << "�����ص�ѹ�趨ֵ�Ѵﵽ;";
			if (stopinfo.compts.stopbyCM == 1)
				oss << "���׮����Ҫ����ֹ���;";
		}
		break;
	case 1:
		oss << DETAIL_BOOLS_NAME;
		break;
	case 2:
		switch (faultinfo.compt.insultFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 3:
		switch (faultinfo.compt.OCTFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 4:
		switch (faultinfo.compt.BMStemFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 5:
		switch (faultinfo.compt.CCFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 6:
		switch (faultinfo.compt.BTFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 7:
		switch (faultinfo.compt.HVolFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 8:
		switch (faultinfo.compt.T2VolFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 9:
		switch (faultinfo.compt.OtherFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 10:
		if ((stopfaultinfo.data & 0x0f) == 0)
			oss << "��.";
		else {
			switch (stopfaultinfo.compt.curfault) {
			case 1:
				oss << "��������,";
				break;
			case 2:
				oss << "���������־λ�쳣,";
				break;
			}
			switch (stopfaultinfo.compt.volfault) {
			case 1:
				oss << "��ѹ�쳣.";
				break;
			case 2:
				oss << "��ѹ�쳣��־λ�쳣.";
				break;
			}
		}
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_BST::detailName(int i) const {
	switch (i) {
	case 0:
		return "BMS��ֹ���ԭ��";
	case 1:
		return "���׮��ֹ���Ĺ���ԭ��";
	case 2:
		return "��Ե����";
	case 3:
		return "�������������";
	case 4:
		return "BMS���������������";
	case 5:
		return "�������������";
	case 6:
		return "������¶ȹ���";
	case 7:
		return "��ѹ�̵�������";
	case 8:
		return "����2��ѹ������";
	case 9:
		return "��������";
	case 10:
		return "BMS��ֹ������ԭ��";
	default:
		return "";
	}
}
BOOL CAN_BST::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_BST& tmp=dynamic_cast<CAN_BST&>(cf);
	return  tmp.stopinfo.data == stopinfo.data  && tmp.faultinfo.data[ 0 ] == faultinfo.data[ 0 ] && tmp.faultinfo.data[ 1 ] == faultinfo.data[ 1 ] && tmp.stopfaultinfo.data == stopfaultinfo.data;
}
Basic_CAN_frame& CAN_BST::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_BST & bst = dynamic_cast<CAN_BST&> (can);
	index =bst.index;
	stopinfo.data = bst.stopinfo.data;
	faultinfo.data[0] = bst.faultinfo.data[0];
	faultinfo.data[1] = bst.faultinfo.data[1];
	stopfaultinfo.data = bst.stopfaultinfo.data;
	return *this;
}
//------------------ CST ------------------//
BOOL CAN_CST::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==4 && tf.getNumField()>= 11){
		stopinfo.data = tf.getByteHex( DATA_BASE + 0 ) ;
		faultinfo.data[ 0 ] = tf.getByteHex(DATA_BASE + 1 ) ;
		faultinfo.data[ 1 ] = tf.getByteHex(DATA_BASE + 2 ) ;
		stopfaultinfo.data = tf.getByteHex(DATA_BASE + 3 ) ;
		return true;
	}
	return false;
}
string CAN_CST::get_detail()const{
	ostringstream oss;
	oss<<"���׮��ֹ���ԭ��--";
	if(stopinfo.data == 0){
		oss<<"��";
	}else{
		if(stopinfo.compts.stopbyCM == 1)
			oss<<"�����趨�����Ѵﵽ;";
		if(stopinfo.compts.stopbyMan == 1)
			oss<<"�˹���ֹ;";
		if(stopinfo.compts.stopbyFault == 1)
			oss<<"������ֹ;";
		if(stopinfo.compts.stopbyBMS == 1)
			oss<<"BMS����Ҫ����ֹ���;";
	}
	oss<<"BMS�����ֹ�Ĺ���ԭ��--";
	if(faultinfo.data[0] == 0 && (faultinfo.data[1]&0x0f) == 0)
		oss<<"��.";
	else{
		switch(faultinfo.compt.HTempFault){
		case 1:
			oss<<"��������,";
			break;
		case 2:
			oss<<"�������±�־λ������,";
			break;
		}
		switch(faultinfo.compt.CCFault){
		case 1:
			oss<<"�������������,";
			break;
		case 2:
			oss<<"�����������־λ������,";
			break;
		}
		switch(faultinfo.compt.CMtemFault){
		case 1:
			oss<<"�����ڲ�����,";
			break;
		case 2:
			oss<<"�����ڲ����±�־λ������,";
			break;
		}
		switch(faultinfo.compt.CurFault){
			case 1:
				oss<<"��������޷�����,";
				break;
			case 2:
				oss<<"�������ͱ�־λ������,";
				break;
		}
		switch(faultinfo.compt.CSFault){
			case 1:
				oss<<"������ͣ����,";
				break;
			case 2:
				oss<<"������ͣ��־λ������,";
				break;
		}
		switch(faultinfo.compt.OtherFault){
		case 1:
			oss<<"�������ϡ�";
				break;
		case 2:
			oss<<"�������ϱ�־λ�쳣��";
			break;
		}
	}
	oss<<"���׮��ֹ������ԭ��--";
	if((stopfaultinfo.data&0x0f) == 0)
		oss<<"��.";
	else{
		switch(stopfaultinfo.compt.curfault){
		case 1:
			oss<<"������ƥ��,";
			break;
		case 2:
			oss<<"����ƥ���־λ�쳣,";
			break;
		}
		switch(stopfaultinfo.compt.volfault){
		case 1:
			oss<<"��ѹ�쳣.";
			break;
		case 2:
			oss<<"��ѹ�쳣��־λ�쳣.";
			break;
		}
	}
	return oss.str();
}
string CAN_CST::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		if (stopinfo.data == 0) {
			oss << "��";
		}
		else {
			if (stopinfo.compts.stopbyCM == 1)
				oss << "�����趨�����Ѵﵽ;";
			if (stopinfo.compts.stopbyMan == 1)
				oss << "�˹���ֹ;";
			if (stopinfo.compts.stopbyFault == 1)
				oss << "������ֹ;";
			if (stopinfo.compts.stopbyBMS == 1)
				oss << "BMS����Ҫ����ֹ���;";
		}
		break;
	case 1:
		oss << DETAIL_BOOLS_NAME;
		break;
	case 2:
		switch (faultinfo.compt.HTempFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 3:
		switch (faultinfo.compt.CCFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 4:
		switch (faultinfo.compt.CMtemFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 5:
		switch (faultinfo.compt.CurFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 6:
		switch (faultinfo.compt.CSFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 7:
		switch (faultinfo.compt.OtherFault) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		case 1:
			oss << DETAIL_TRUE;
			break;
		default:
			oss << DETAIL_UNKNOW;
		}
		break;
	case 8:
		if ((stopfaultinfo.data & 0x0f) == 0)
			oss << "��.";
		else {
			switch (stopfaultinfo.compt.curfault) {
			case 1:
				oss << "������ƥ��,";
				break;
			default:
				break;
			}
			switch (stopfaultinfo.compt.volfault) {
			case 1:
				oss << "��ѹ�쳣.";
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_CST::detailName(int i) const {
	switch (i) {
	case 0:
		return "���׮��ֹ���ԭ��";
	case 1:
		return "BMS�����ֹ�Ĺ���ԭ��";
	case 2:
		return "��������";
	case 3:
		return "�������������";
	case 4:
		return "�����ڲ�����";
	case 5:
		return "��������޷�����";
	case 6:
		return "������ͣ����";
	case 7:
		return "��������";
	case 8:
		return "���׮��ֹ������ԭ��";
	default:
		return "";
	}
}
BOOL CAN_CST::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_CST& tmp=dynamic_cast<CAN_CST&>(cf);
	return  tmp.stopinfo.data == stopinfo.data  && tmp.faultinfo.data[0] == faultinfo.data[0] &&  tmp.faultinfo.data[1] == faultinfo.data[1] &&tmp.stopfaultinfo.data == stopfaultinfo.data;
}
Basic_CAN_frame& CAN_CST::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_CST & cst = dynamic_cast<CAN_CST&> (can);
	index =cst.index;
	stopinfo.data = cst.stopinfo.data;
	faultinfo.data[0] = cst.faultinfo.data[0];
	faultinfo.data[1] = cst.faultinfo.data[1];
	stopfaultinfo.data = cst.stopfaultinfo.data;
	return *this;
}
//================================ �������׶� ==========================//
//----------------- BSD ------------------//
BOOL CAN_BSD::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==7 && tf.getNumField()>= 14){
		SOC = tf.getByteHex( DATA_BASE + 0 ) ;
		LVol = tf.getByteHex(DATA_BASE + 1 ) + (tf.getByteHex(DATA_BASE + 2 )<<8);
		HVol =  tf.getByteHex(DATA_BASE + 3 ) + (tf.getByteHex(DATA_BASE + 4 )<<8);
		LTem =  tf.getByteHex(DATA_BASE + 5 );
		HTem = tf.getByteHex(DATA_BASE + 6);
		return true;
	}
	return false;
}
string CAN_BSD::get_detail()const{
	ostringstream oss;
	if(SOC != 0xff)
		oss<<"��ǰ�ɵ�״̬(SOC)"<< (UINT)SOC<<"%.";
	if(LVol != 0xffff)
		oss<<"��������͵�ѹ"<< 0.01 * LVol << "V.";
	if(HVol != 0xffff)
		oss<<"��������ߵ�ѹ"<< 0.01 * HVol << "V.";
	if(LTem != 0xff)
		oss<<"������������¶�"<<(UINT) LTem - 50 << "���϶�.";
	if(HTem != 0xff)
		oss<<"������������¶�"<< (UINT)HTem - 50 << "���϶�.";
	return oss.str();
}
string CAN_BSD::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		oss << (UINT)SOC << "%.";
		break;
	case 1:
		oss  << 0.01 * LVol << "V.";
		break;
	case 2:
		oss << 0.01 * HVol << "V.";
		break;
	case 3:
		oss << (UINT)LTem - 50 << "��.";
		break;
	case 4:
		oss << (UINT)HTem - 50 << "��.";
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_BSD::detailName(int i) const {
	switch (i) {
	case 0:
		return "��ǰ�ɵ�״̬(SOC)";
	case 1:
		return "��������͵�ѹ";
	case 2:
		return "��������ߵ�ѹ";
	case 3:
		return "������������¶�";
	case 4:
		return "������������¶�";
	default:
		return "";
	}
}
BOOL CAN_BSD::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_BSD& tmp=dynamic_cast<CAN_BSD&>(cf);
	return tmp.SOC== SOC && tmp.LVol == LVol && tmp.HVol == HVol && tmp.LTem == LTem && tmp.HTem == HTem;
}
Basic_CAN_frame& CAN_BSD::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_BSD & bsd = dynamic_cast<CAN_BSD&> (can);
	index =bsd.index;
	SOC = bsd.SOC ;
	LVol = bsd.LVol;
	HVol =bsd.HVol;
	LTem = bsd.LTem;
	HTem = bsd.HTem;
	return *this;
}
//---------------- CSD ------------------//
BOOL CAN_CSD::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)==8 && tf.getNumField()>= 15){
		chargetime = tf.getByteHex( DATA_BASE + 0 ) + (tf.getByteHex(DATA_BASE + 1 )<<8);
		outenergy = tf.getByteHex(DATA_BASE + 2 ) + (tf.getByteHex(DATA_BASE + 3 ) << 8 ) ;
		CM_id =  tf.getByteHex(DATA_BASE + 4 )+ (tf.getByteHex(DATA_BASE + 5 )<<8) + (tf.getByteHex(DATA_BASE + 6 )<<16)+ (tf.getByteHex(DATA_BASE + 7 )<<24);
		return true;
	}
	return false;
}
string CAN_CSD::get_detail()const{
	ostringstream oss;
	if(chargetime != 0xffff)
		oss<<"�������ʱ��:"<< chargetime <<"����.";
	if(outenergy != 0xffff)
		oss<<"�������"<< 0.1 * outenergy << "Kw��h.";
	if(CM_id != 0xffffffff)
		oss<<"�������:0x"<<hex<<setw(8)<<setfill('0')<<CM_id<<'.';
	return oss.str();
}
string CAN_CSD::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		oss <<  chargetime << "����.";
		break;
	case 1:
		oss <<  0.1 * outenergy << "Kw��h.";
		break;
	case 2:
		oss << "0x" << hex << setw(8)<<setfill('0') << CM_id << '.';
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_CSD::detailName(int i) const {
	switch (i) {
	case 0:
		return "�������ʱ��";
	case 1:
		return "�������";
	case 2:
		return "�������";
	default:
		return "";
	}
}
BOOL CAN_CSD::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_CSD& tmp=dynamic_cast<CAN_CSD&>(cf);
	return tmp.chargetime== chargetime && tmp.outenergy == outenergy && tmp.CM_id == CM_id;
}
Basic_CAN_frame& CAN_CSD::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_CSD & csd = dynamic_cast<CAN_CSD&> (can);
	index =csd.index;
	chargetime = csd.chargetime ;
	outenergy = csd.outenergy;
	CM_id = csd.CM_id ;
	return *this;
}
//========================== ������ =========================//
//------------ BEM -----------------//
BOOL CAN_BEM::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)== 4 && tf.getNumField()>= 11){
		errinfo.data = tf.getByteHex( DATA_BASE + 0 ) + (tf.getByteHex(DATA_BASE + 1 )<<8 ) + (tf.getByteHex(DATA_BASE + 2 )<< 16 ) + (tf.getByteHex(DATA_BASE + 3 )<<24 ) ;
		return true;
	}
	return false;
}
string CAN_BEM::get_detail()const{
	ostringstream oss;
	oss<<"��ǰ���׮ͨ��״̬--";
	if(errinfo.data == 0)
		oss<<"��.";
	else{
		switch(errinfo.compts.CRM00_OV){
		case 1:
			oss<<"CRM=0x00���ĳ�ʱ";
			break;
		case 2:
			oss<<"CRM00��ʱ��־λ������,";
			break;
		}
		switch(errinfo.compts.CRMAA_OV){
		case 1:
			oss<<"CRM=0xAA���ĳ�ʱ,";
			break;
		case 2:
			oss<<"CRMAA��ʱ��־λ������,";
			break;
		}
		switch(errinfo.compts.CML_OV){
		case 1:
			oss<<"CML���ĳ�ʱ,";
			break;
		case 2:
			oss<<"CML��ʱ��־λ������,";
			break;
		}
		switch(errinfo.compts.CRO_OV){
		case 1:
			oss<<"CRO���ĳ�ʱ,";
			break;
		case 2:
			oss<<"CRO��ʱ��־λ������,";
			break;
		}
		switch(errinfo.compts.CCS_OV){
		case 1:
			oss<<"CCS���ĳ�ʱ,";
			break;
		case 2:
			oss<<"CCS��ʱ��־λ������,";
			break;
		}
		switch(errinfo.compts.CST_OV){
		case 1:
			oss<<"CST���ĳ�ʱ,";
			break;
		case 2:
			oss<<"CST��ʱ��־λ������,";
			break;
		}
		switch(errinfo.compts.CSD_OV){
		case 1:
			oss<<"CSD���ĳ�ʱ,";
			break;
		case 2:
			oss<<"CSD��ʱ��־λ������,";
			break;
		}
	}
	return oss.str();
}
string CAN_BEM::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		oss << DETAIL_BOOLS_NAME;
		break;
	case 1:
		switch (errinfo.compts.CRM00_OV) {
		case 0:
			oss << DETAIL_FALSE ;
			break;
		default:
			oss << DETAIL_TRUE;
			break;
		}
		break;
	case 2:
		switch (errinfo.compts.CRMAA_OV) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		default:
			oss << DETAIL_TRUE ;
			break;
		}
		break;
	case 3:
		switch (errinfo.compts.CML_OV) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		default:
			oss << DETAIL_TRUE ;
			break;
		}
		break;
	case 4:
		switch (errinfo.compts.CRO_OV) {
		case 0:
			oss << DETAIL_FALSE ;
			break;
		default:
			oss << DETAIL_TRUE;
			break;
		}
		break;
	case 5:
		switch (errinfo.compts.CCS_OV) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		default:
			oss << DETAIL_TRUE ;
			break;
		}
		break;
	case 6:
		switch (errinfo.compts.CST_OV) {
		case 0:
			oss << DETAIL_FALSE ;
			break;
		default:
			oss << DETAIL_TRUE ;
			break;
		}
		break;
	case 7:
		switch (errinfo.compts.CSD_OV) {
		case 0:
			oss << DETAIL_FALSE ;
			break;
		default:
			oss << DETAIL_TRUE;
			break;
		}
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_BEM::detailName(int i) const {
	switch (i) {
	case 0:
		return "��ǰ���׮ͨ��״̬";
	case 1:
		return "CRM=0x00���ĳ�ʱ";
	case 2:
		return "CRM=0xAA���ĳ�ʱ";
	case 3:
		return "CML���ĳ�ʱ";
	case 4:
		return "CRO���ĳ�ʱ";
	case 5:
		return "CCS���ĳ�ʱ";
	case 6:
		return "CST���ĳ�ʱ";
	case 7:
		return "CSD���ĳ�ʱ";
	default:
		return "";
	}
}
BOOL CAN_BEM::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_BEM& tmp=dynamic_cast<CAN_BEM&>(cf);
	return  tmp.errinfo.data == errinfo.data ;
}
Basic_CAN_frame& CAN_BEM::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_BEM & bem = dynamic_cast<CAN_BEM&> (can);
	index =bem.index;
	errinfo.data = bem.errinfo.data;
	return *this;
}
//------------------ CEM ---------------------//
BOOL CAN_CEM::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)== 4 && tf.getNumField()>= 11){
		errinfo.data = tf.getByteHex( DATA_BASE + 0 ) + (tf.getByteHex(DATA_BASE + 1 )<<8 ) + (tf.getByteHex(DATA_BASE + 2 )<< 16 ) + (tf.getByteHex(DATA_BASE + 3 )<<24 ) ;
		return true;
	}
	return false;
}
string CAN_CEM::get_detail()const{
	ostringstream oss;
	oss<<"��ǰBMSͨ��״̬--";
	if(errinfo.data == 0)
		oss<<"��.";
	else{
		switch(errinfo.compts.BRM_OV){
		case 1:
			oss<<"BRM���ĳ�ʱ";
			break;
		case 2:
			oss<<"BRM��ʱ��־λ������,";
			break;
		}
		switch(errinfo.compts.BCP_OV){
		case 1:
			oss<<"BCP���ĳ�ʱ,";
			break;
		case 2:
			oss<<"BCP��ʱ��־λ������,";
			break;
		}
		switch(errinfo.compts.BRO_OV){
		case 1:
			oss<<"BRO���ĳ�ʱ,";
			break;
		case 2:
			oss<<"BRO��ʱ��־λ������,";
			break;
		}
		switch(errinfo.compts.BCS_OV){
		case 1:
			oss<<"BCS���ĳ�ʱ,";
			break;
		case 2:
			oss<<"BCS��ʱ��־λ������,";
			break;
		}
		switch(errinfo.compts.BCL_OV){
		case 1:
			oss<<"BCL���ĳ�ʱ,";
			break;
		case 2:
			oss<<"BCL��ʱ��־λ������,";
			break;
		}
		switch(errinfo.compts.BST_OV){
		case 1:
			oss<<"BST���ĳ�ʱ,";
			break;
		case 2:
			oss<<"BST��ʱ��־λ������,";
			break;
		}
		switch(errinfo.compts.BSD_OV){
		case 1:
			oss<<"BSD���ĳ�ʱ,";
			break;
		case 2:
			oss<<"BSD��ʱ��־λ������,";
			break;
		}
	}
	return oss.str();
}
string CAN_CEM::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		oss << DETAIL_BOOLS_NAME;
		break;
	case 1:
		switch (errinfo.compts.BRM_OV) {
		case 0:
			oss << DETAIL_FALSE ;
			break;
		default:
			oss << DETAIL_TRUE ;
			break;
		}
		break;
	case 2:
		switch (errinfo.compts.BCP_OV) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		default:
			oss << DETAIL_TRUE;
			break;
		}
		break;
	case 3:
		switch (errinfo.compts.BRO_OV) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		default:
			oss << DETAIL_TRUE;
			break;
		}
		break;
	case 4:
		switch (errinfo.compts.BCS_OV) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		default:
			oss << DETAIL_TRUE;
			break;
		}
		break;
	case 5:
		switch (errinfo.compts.BCL_OV) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		default:
			oss << DETAIL_TRUE;
			break;
		}
		break;
	case 6:
		switch (errinfo.compts.BST_OV) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		default:
			oss << DETAIL_TRUE;
			break;
		}
		break;
	case 7:
		switch (errinfo.compts.BSD_OV) {
		case 0:
			oss << DETAIL_FALSE;
			break;
		default:
			oss << DETAIL_TRUE;
			break;
		}
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_CEM::detailName(int i) const {
	switch (i) {
	case 0:
		return "��ǰBMSͨ��״̬";
	case 1:
		return "BRM���ĳ�ʱ";
	case 2:
		return "BCP���ĳ�ʱ";
	case 3:
		return "BRO���ĳ�ʱ";
	case 4:
		return "BCS���ĳ�ʱ";
	case 5:
		return "BCL���ĳ�ʱ";
	case 6:
		return "BST���ĳ�ʱ";
	case 7:
		return "BSD���ĳ�ʱ";
	default:
		return "";
	}
}
BOOL CAN_CEM::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_CEM& tmp=dynamic_cast<CAN_CEM&>(cf);
	return  tmp.errinfo.data == errinfo.data ;
}
Basic_CAN_frame& CAN_CEM::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_CEM & cem = dynamic_cast<CAN_CEM&> (can);
	index =cem.index;
	errinfo.data = cem.errinfo.data;
	return *this;
}
//======================== ��֡���� =========================//
//--------------- TP_START --------------------//
BOOL CAN_TP_START::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)== 8 && tf.getNumField()>= 15){
		console = tf.getByteHex( DATA_BASE + 0 ) ;
		length = tf.getByteHex(DATA_BASE + 1 ) + (tf.getByteHex(DATA_BASE + 2 )<< 8 );
		packets = tf.getByteHex(DATA_BASE + 3 ) ;
		reserve = tf.getByteHex(DATA_BASE + 4);
		PGN = tf.getByteHex(DATA_BASE + 6) ;
		return true;
	}
	return false;
}
string CAN_TP_START::get_detail()const{
	ostringstream oss;
	if(console == TP_START_CONSOLE)
		oss<<"��֡�������ֽ�����"<<length<<",���ݰ���:"<<(UINT)packets<<".�������PGNΪ0x00"<<hex<<setw(2)<<setfill('0')<<int(PGN)<<"00.";
	else
		oss<<"�������󣬲��Ƕ����������֡������";
	return oss.str();
}
string CAN_TP_START::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		oss << length ;
		break;
	case 1:
		oss << (UINT)packets ;
		break;
	case 2:
		oss << "0x00" << hex << setw(2) << setfill('0') << (UINT)(PGN) << "00.";
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_TP_START::detailName(int i) const {
	switch (i) {
	case 0:
		return "��֡�������ֽ���";
	case 1:
		return "���ݰ���";
	case 2:
		return "�������PGN";
	default:
		return "";
	}
}
BOOL CAN_TP_START::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_TP_START& tmp=dynamic_cast<CAN_TP_START&>(cf);
	return  tmp.console == console && tmp.length == length && tmp.packets == packets && tmp.PGN == PGN ;
}
Basic_CAN_frame& CAN_TP_START::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_TP_START & tp = dynamic_cast<CAN_TP_START&> (can);
	index =tp.index;
	console = tp.console;
	length = tp.length;
	packets = tp.packets;
	reserve= tp.reserve;
	PGN = tp.PGN ;
	return *this;
}
//--------------- TP_REPLY --------------------//
BOOL CAN_TP_REPLY::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)== 8 && tf.getNumField()>= 15){
		for( UINT i = 0 ; i < 5 ; i++ ){
			compts.data[ i ] = tf.getByteHex( DATA_BASE + i ) ;
		}
		compts.data[ 5 ] = tf.getByteHex(DATA_BASE + 6) ;
		return true;
	}
	return false;
}
string CAN_TP_REPLY::get_detail()const{
	ostringstream oss;
	if(compts.reply.console == TP_REPLY_CONSOLE)
		oss<<"��֡����ʣ�����ݰ�����"<<(UINT)compts.reply.packets<<".��֡������һ���ݰ����Ϊ:"<<(UINT)compts.reply.nextpacket<<".�������PGNΪ0x00"<<hex<<setw(2)<< setfill('0') <<int(compts.reply.PGN)<<"00.";
	else if(compts.end.console == TP_END_CONSOLE)
		oss<<"�Ѵ������ֽ�����"<<(UINT)compts.end.length<<",���ݰ���:"<<(UINT)compts.end.packets<<".�������PGNΪ0x00"<<hex<<setw(2)<<setfill('0')<<int(compts.end.PGN)<<"00.";
	else
		oss<<"�������󣬲��Ƕ�����ݻظ�֡������";
	return oss.str();
}
string CAN_TP_REPLY::getDetail(int i) const {
	ostringstream oss;
	if (compts.reply.console == TP_REPLY_CONSOLE) {
		switch (i) {
		case 0:
			oss << (UINT)compts.reply.packets ;
			break;
		case 1:
			oss  << (UINT)compts.reply.nextpacket;
			break;
		case 2:
			oss  << hex << setw(6) << setfill('0') << int(compts.reply.PGN);
			break;
		default:
			break;
		}
	}
	else if (compts.end.console == TP_END_CONSOLE) {
		switch (i) {
		case 0:
			oss << (UINT)compts.end.length;
			break;
		case 1:
			oss <<(UINT)compts.end.packets;
			break;
		case 2:
			oss << hex << setw(6) << setfill('0') << int(compts.end.PGN);
			break;
		default:
			break;
		}
	}
	return oss.str().c_str();
}
const char* CAN_TP_REPLY::detailName(int i) const {
	if (compts.reply.console == TP_REPLY_CONSOLE)
		switch (i) {
		case 0:
			return "ʣ�����ݰ���";
		case 1:
			return "��һ���ݰ����";
		case 2:
			return "�������PGN";
		default:
			return "";
		}
	else if (compts.reply.console == TP_END_CONSOLE) {
		switch (i) {
		case 0:
			return "�Ѵ������ֽ���";
		case 1:
			return "���ݰ���";
		case 2:
			return "�������PGN";
		default:
			return "";
		}
	}
	return "";
}
BOOL CAN_TP_REPLY::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_TP_REPLY& tmp=dynamic_cast<CAN_TP_REPLY&>(cf);
	for(UINT i = 0 ; i < 6 ; i++)
		if(tmp.compts.data[ i ] != compts.data[ i ])
			return false ;
	return true;
}
Basic_CAN_frame& CAN_TP_REPLY::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_TP_REPLY & tp = dynamic_cast<CAN_TP_REPLY&> (can);
	index =tp.index;
	for(UINT i = 0 ; i < 6 ; i++){
		compts.data[i] = tp.compts.data[i];
	}
	return *this;
}
//--------------- TP_DATA --------------------//
BOOL CAN_TP_DATA::get_data(Table_field &tf){
	if(tf.getByteHex(LENGTH_BASE)== 8 && tf.getNumField()>= 15){
		packet = tf.getByteHex(DATA_BASE);
		for( UINT i = 0 ; i < 7; i++ ){
			data[ i ] = tf.getByteHex( DATA_BASE + i + 1 ) ;
		}
		return true;
	}
	return false;
}
string CAN_TP_DATA::get_detail()const{
	ostringstream oss;
	oss<< "��ǰ���ݰ�Ϊ��֡�����"<<(UINT)packet<<"��.";
	return oss.str();
}
string CAN_TP_DATA::getDetail(int i) const {
	ostringstream oss;
	switch (i) {
	case 0:
		oss << (UINT)packet;
		break;
	default:
		break;
	}
	return oss.str().c_str();
}
const char* CAN_TP_DATA::detailName(int i) const {
	switch (i) {
	case 0:
		return "���ݰ����";
	default:
		return "";
	}
}
BOOL CAN_TP_DATA::operator==(Basic_CAN_frame& cf)const{
	if(cf.getCANID() != canid)return false;
	CAN_TP_DATA& tmp=dynamic_cast<CAN_TP_DATA&>(cf);
	for(UINT i = 0 ; i < 7 ; i++)
		if(tmp.data[ i ] != data[ i ])
			return false ;
	return tmp.packet==packet;
}
Basic_CAN_frame& CAN_TP_DATA::operator=(Basic_CAN_frame& can){
	if(getCANID() != can.getCANID()){
		return *this;
	}
	CAN_TP_DATA & tp = dynamic_cast<CAN_TP_DATA&> (can);
	index =tp.index;
	packet = tp.packet;
	for(UINT i = 0 ; i < 7 ; i++){
		data[i] = tp.data[i];
	}
	return *this;
}
//=============================== Mulit_frame ================//
BOOL Multi_frame::operator==(Multi_frame & mf)const{
	if(mf.PGN != PGN || frames.size()!= mf.frames.size() ){
		return false;
	}
	for(UINT i = 0 ; i < frames.size(); i++){
		if(!((*frames[ i ]) == (*mf.frames[ i ])))
			return false;
	}
	return true;
}
Multi_frame& Multi_frame::operator=(const Multi_frame&mf){
	PGN = mf.PGN;
	state = mf.state;
	frames.resize(mf.frames.size());
	for(UINT i = 0 ; i < mf.frames.size() ; i++){
		frames[i] = mf.frames[i];
	}
	return *this;
}
BYTE Multi_frame::getByte(UINT i)const{
	UINT r = i / 7 + 2, c = i % 7 ;
	if( frames.size() <= r )
		return 0xff;
	return dynamic_cast<CAN_TP_DATA*>(frames[ r ])->data[c];
}
WORD Multi_frame::getWord(UINT i)const{
	UINT r = (i + 1) / 7 + 2;
	if(frames.size() <= r){
		return 0xffff;
	}
	return getByte( i ) + (getByte( i + 1 )<<8);
}
DWORD Multi_frame::getDWord(UINT i)const{
	UINT r = (i + 3) / 7 + 2;
	if(frames.size() <= r){
			return 0xffffffff;
	}
	return getWord(i) + (getWord( i + 2) << 16 );
}
