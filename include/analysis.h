#ifndef _ANALYSIS_H
#define _ANALYSIS_H
#include "PacketLength.h"
#include "PacketInfo.h"
#include "FileSava.h"

#include <string.h>

using namespace std;

#define PACKET_LENGTH_188 188
#define PACKET_LENGTH_204 204

class ANALYSIS{
public:
    ANALYSIS(char* file);
    ~ANALYSIS();
    //判断TS包长是否是188
    //获取节目信息到map里面
    //获取pes文件，es文件
    bool execute_parse();

private:
    PacketLength* packetLength;
    PacketInfo* packetInfo;
    FileSava* fileSava;

    char* ts_path;
    unsigned int PID = 0;
    unsigned int mpacket_length = 0;
    unsigned int mfile_start_position = 0;
    //存储PAT表中的节目信息。
    vector<PROGRAM_INFO*> program_infos;
    //将节目信息中的PID值与PMT表分别匹配
    map<unsigned int, vector<STREAM_TYPE*>> infos;

};
#endif


