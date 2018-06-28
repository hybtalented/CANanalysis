/*
 * canread.h
 *
 *  Created on: 2017��12��5��
 *      Author: Administrator
 */

#ifndef CANREAD_H_
#define CANREAD_H_
#include<iostream>
#include<vector>
#include<string>
#include<CANdata.h>
#include<assert.h>
using namespace std;
class CANElement;
typedef vector<string> stdStringList;
class CANREAD{
	vector<Basic_CAN_frame*> canframes;
	vector<UINT32> timestampes;
	CAN_CHM oldchm;
	CAN_BHM oldbhm;
	CAN_CRM oldcrm;
	CAN_CTS oldcts;
	CAN_CML oldcml;
	CAN_BRO oldbro;
	CAN_CRO oldcro;
	CAN_BCL oldbcl;
	CAN_CCS oldccs;
	CAN_BSM oldbsm;
	CAN_BST oldbst;
	CAN_CST oldcst;
	CAN_BSD oldbsd;
	CAN_CSD oldcsd;
	CAN_BEM oldbem;
	CAN_CEM oldcem;
	CAN_BRM oldbrm;
	CAN_BCP oldbcp;
	CAN_BCS oldbcs;
	Multi_frame *multiframe;
	BYTE oldPGN ;
	bool autoindex; //�Ƿ��Զ���ţ�Ϊ�����ȡ�ĵ��б��
	UINT indexcol ; //������Զ���ţ���ȡ�ĵ��еĵڼ�����Ϊ��ţ�Ĭ��Ϊ0
	BYTE ifs,ofs;
public:
	CANREAD():canframes(),timestampes(),oldchm(),oldbhm(),oldcrm(),oldcts(),oldcml(),oldbro(),oldbcl(),oldccs(),oldbsm(),oldbst(),oldcst(),oldbsd(),oldcsd(),oldbem(),oldcem(),oldbrm(),oldbcp(),oldbcs(),multiframe(NULL),oldPGN(0xff),autoindex(false),indexcol(0),ifs('\t'),ofs('\t'){}
	VOID read_data( istream& in, bool ignorefirstline=true);	//ignorefirstline��ʾ�Ƿ���Ե�һ��
	bool atEnd() { return canframes.size()==0; }
	CANREAD& operator>>(stdStringList& s);
	CANREAD& operator>>(CANElement& s);
	VOID set_index(vector<UINT>& where,bool auto_index = false, UINT indexc = 0){CANID_BASE=where[0 ];LENGTH_BASE=where[1]; DATA_BASE = where[2];autoindex= auto_index ; indexcol =indexc;} //��ȡ�ĵ��У�canid�����ݳ��Ⱥ�����֡���ڵ��У���0��ʼ����Ĭ��Ϊ��3����6�͵�7��
	VOID set_iofs(BYTE is='\t',BYTE os='\t'){ifs=is; ofs =os;}
	VOID print(ostream&);
	~CANREAD() { if (multiframe) { delete multiframe; multiframe = NULL; } for (vector<Basic_CAN_frame*>::iterator it = canframes.begin(); it != canframes.end(); it++)delete *it; canframes.clear(); timestampes.clear(); }
};
stdStringList& operator<<(stdStringList&, const string);


#endif /* CANREAD_H_ */
