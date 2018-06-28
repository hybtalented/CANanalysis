/*
 * canread.cpp
 *
 *  Created on: 2017年12月5日
 *      Author: Administrator
 */
#include"canread.h"
#include"table field.h"
#include<sstream>
#include<canelement.h>
inline string int2str(int a) { 
	stringstream s;
	string str;
	s << a;
	s >> str;
	return str;
}
inline string uchar2str(BYTE c) {
	stringstream s;
	string str;
	s << c;
	s >> str;
	return str;
}
VOID CANREAD::read_data(istream& in,bool igfl){
	if (timestampes.size()) {
		this->~CANREAD();
	}
	string line;
	if(igfl){getline(in,line) ;}
	while(getline(in,line)){
		Table_field tf(line,ifs,false);
		tf.setODivide(ofs);
		DWORD canid = tf.getDWordHex(CANID_BASE);
		Basic_CAN_frame * newcan = Basic_CAN_frame::create_frame(canid);
		if(!newcan){
			cerr<<"创建数据帧错误:"<<endl;
			for(UINT i = 0 ; i< tf.getNumField(); i++){
				cerr<<tf.getString(i)<<'\t';
			}
			cout<<endl;
			exit(0);
		}
		newcan->get_data(tf);
		newcan->setIndex(tf.getDWord(indexcol));
		timestampes.push_back(tf.getDWordHex(2));
		canframes.push_back(newcan);
	}
}
static void direct(bool b,string&&tpname){
	const char  * charge ="充电桩",*bms="BMS";
	cout<<charge;
	if(b){
		for(UINT i = 0 ; i< 20 ; i++){
			cout<<">>";
		}
		cout<<"\t"<<tpname<<"\t";
		for(UINT i = 0 ; i< 20 ; i++){
			cout<<">>";
		}
	}else{
		for(UINT i = 0 ; i< 20 ; i++){
			cout<<"<<";
		}
		cout<<"\t"<<tpname<<"\t";
		for(UINT i = 0 ; i< 20 ; i++){
			cout<<"<<";
		}
	}
	cout<<bms;
}
static inline void theindex(UINT index){
	cout<<"第"<<index<<"帧:";
}
VOID CANREAD::print(ostream& out){
	out<<"帧编号"<<"阶段"<<ofs<<"帧名"<<ofs<<"内容"<<endl;
	for( UINT i = 0 ; i < canframes.size(); i++){
		DWORD canid = canframes[i]->getCANID();
		UINT index ;
		if(autoindex)
			index = i;
		else
			index=canframes[i]->getIndex();
		string detail = canframes[i]->get_detail() ;
		out<<index<<ofs<<canframes[i]->getState()<<ofs<<canframes[i]->get_frame_name()<<ofs<<detail<<endl;
		switch(canid){
		case BHM_CANID:
			if(!(*canframes[i]==oldbhm)){
				oldbhm = *canframes[i];
				direct(0,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case CHM_CANID:
			if(!(*canframes[i]==oldchm)){
				oldchm = *canframes[i];
				direct(1,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case CRM_CANID:
			if(!(*canframes[i]==oldcrm)){
				oldcrm = *canframes[i];
				direct(1,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case CTS_CANID:
			if(!(*canframes[i]==oldcts)){
				oldcts = *canframes[i];
				direct(1,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case CML_CANID:
			if(!(*canframes[i]==oldcml)){
				oldcml = *canframes[i];
				direct(1,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case BRO_CANID:
			if(!(*canframes[i]==oldbro)){
				oldbro = *canframes[i];
				direct(0,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case CRO_CANID:
			if(!(*canframes[i]==oldcro)){
				oldcro = *canframes[i];
				direct(1,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case BCL_CANID:
			if(!(*canframes[i]==oldbcl)){
				oldbcl = *canframes[i];
				direct(0,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case CCS_CANID:
			if(!(*canframes[i]==oldccs)){
				oldccs = *canframes[i];
				direct(1,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case BSM_CANID:
			if(!(*canframes[i]==oldbsm)){
				oldbsm = *canframes[i];
				direct(0,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case BST_CANID:
			if(!(*canframes[i]==oldbst)){
				oldbst = *canframes[i];
				direct(0,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case CST_CANID:
			if(!(*canframes[i]==oldcst)){
				oldcst = *canframes[i];
				direct(1,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case BSD_CANID:
			if(!(*canframes[i]==oldbsd)){
				oldbsd = *canframes[i];
				direct(0,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case CSD_CANID:
			if(!(*canframes[i]==oldcsd)){
				oldcsd = *canframes[i];
				direct(1,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case BEM_CANID:
			if(!(*canframes[i]==oldbem)){
				oldbem = *canframes[i];
				direct(0,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case CEM_CANID:
			if(!(*canframes[i]==oldcem)){
				oldcem = *canframes[i];
				direct(1,canframes[i]->typeName());cout<<endl;
				theindex(index);cout<<endl;
				cout<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<"\n内容:"<<endl<<detail<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		case TP_START_CANID:
			if(multiframe){
				delete multiframe;
			}
			if(canframes[i]->getConsole()!=TP_START_CONSOLE){
				multiframe =NULL;
			}else switch(canframes[i]->getPGN()){
			case BRM_PGN:
				multiframe = new CAN_BRM(dynamic_cast<CAN_TP_START*>(canframes[i]));
				break;
			case BCP_PGN:
				multiframe =new CAN_BCP(dynamic_cast<CAN_TP_START*>(canframes[i]));
				break;
			case BCS_PGN:
				multiframe = new CAN_BCS(dynamic_cast<CAN_TP_START*>(canframes[i]));
				break;
			default:
				multiframe = NULL;
				break;
			}
			break;
		case TP_REPLY_CANID:
			if(!multiframe){
				break;
			}
			if( canframes[i]->getPGN()!=multiframe->getPGN()){
				delete multiframe;
				multiframe = NULL;
			}else if(canframes[i]->getConsole() == TP_REPLY_CONSOLE ){
				multiframe->addFrame(canframes[i]);
				if(!multiframe->valid()){
					cout<<"不合理请求帧:";
					theindex(index);
					cout<<endl;
					vector<UINT> indexs=multiframe->getIndexs();
					for(UINT j = 0 ; j < indexs.size();j++){
							theindex(indexs[j]);
					}
					cout<<endl<<"阶段:"<<multiframe->getState()<<"\t帧名:"<<multiframe->get_frame_name()<<endl;
					cout<<"*****************************************************************************\n";
				}
			}else if(canframes[i]->getConsole() == TP_END_CONSOLE){
				multiframe->addFrame(canframes[i]);
				if(multiframe->rec_finish()){
					switch(multiframe->getPGN()){
					case BRM_PGN:
						if(!(*multiframe ==oldbrm) ){
							oldbrm.operator=( *multiframe );
							vector<UINT> indexs=multiframe->getIndexs();
							direct(1,multiframe->typeName());cout<<endl;
							for(UINT j = 0 ; j < indexs.size();j++){
								theindex(indexs[j]);
							}
							cout<<endl<<multiframe->getState()<<"\t帧名:"<<multiframe->get_frame_name()<<"\n内容:\n"<<multiframe->get_detail()<<endl;
							cout<<"*****************************************************************************\n";
						}
						delete multiframe;
						multiframe = NULL;
						break;
					case BCP_PGN:
						if(!((*multiframe) ==oldbcp) ){
							oldbcp = *multiframe ;
							vector<UINT> indexs=multiframe->getIndexs();
							direct(1,multiframe->typeName());cout<<endl;
							for(UINT j = 0 ; j < indexs.size();j++){
								theindex(indexs[j]);
							}
							cout<<endl<<multiframe->getState()<<"\t帧名:"<<multiframe->get_frame_name()<<"\n内容:\n"<<multiframe->get_detail()<<endl;
							cout<<"*****************************************************************************\n";
						}
						delete multiframe;
						multiframe = NULL;
						break;
					case BCS_PGN:
						if(!(*multiframe==oldbcs) ){
							oldbcs= *multiframe ;
							vector<UINT> indexs=multiframe->getIndexs();
							direct(1,multiframe->typeName());cout<<endl;
							for(UINT j = 0 ; j < indexs.size();j++){
								theindex(indexs[j]);
							}
							cout<<endl<<multiframe->getState()<<"\t帧名:"<<multiframe->get_frame_name()<<"\n内容:\n"<<multiframe->get_detail()<<endl;
							cout<<"*****************************************************************************\n";
						}
						delete multiframe;
						multiframe = NULL;
						break;
					default:
						delete multiframe;
						multiframe = NULL;
						break;
				}
			}
		}
			break;
		case TP_DATA_CANID:
			if(!multiframe) break;
			multiframe->addFrame(canframes[i]);
			if(!multiframe->valid()){
				cout<<"不合理数据帧:";
				theindex(index);
				cout<<endl;
				vector<UINT> indexs=multiframe->getIndexs();
				for(UINT j = 0 ; j < indexs.size();j++){
					theindex(indexs[j]);
				}
				cout<<endl<<"阶段:"<<multiframe->getState()<<"\t帧名:"<<multiframe->get_frame_name()<<endl;
				cout<<"*****************************************************************************\n";
			}
			break;
		default:
			cout<<"无法识别帧:";
			theindex(index);
			cout<<endl;
			cout<<endl<<"阶段:"<<canframes[i]->getState()<<"\t帧名:"<<canframes[i]->get_frame_name()<<endl;
			cout<<"*****************************************************************************\n";
		}
	}
}


CANREAD& CANREAD::operator>>(stdStringList& out) {
	out.clear();
	assert(canframes.size() > 0 && timestampes.size() > 0);
	DWORD canid = (*canframes.begin())->getCANID();
	UINT index;
	index = (*canframes.begin())->getIndex();
	out << int2str(index)<<int2str(timestampes[0]) << (*canframes.begin())->getState()  << (*canframes.begin())->get_detail();
	canframes.erase(canframes.begin());
	timestampes.erase(timestampes.begin());
	return *this;
}
stdStringList& operator<<(stdStringList& strs, const string s) {
	strs.push_back(s);
	return strs;
}
CANREAD&  CANREAD::operator>>(CANElement& s) {
	s.setFrame(*canframes.begin());
	this->operator>>(dynamic_cast<stdStringList&>(s) );
	return *this;
}
