//
// Created by letv on 19-6-15.
//

#ifndef TS_PARSE_MASTER_PACKETINFO_H
#define TS_PARSE_MASTER_PACKETINFO_H

#include "section.h"
#include "tsheader.h"
#include "pat.h"
#include "pmt.h"

#include <stdio.h>
#include <vector>
#include <map>

using namespace std;

class PacketInfo{
public:
    PacketInfo(FILE* fp, unsigned int packet_length);
    ~PacketInfo();

    bool get_packet_info(map<unsigned int ,vector<STREAM_TYPE*>>& infos);


private:
    int get_payload_start_positon();
    bool info_adapter(map<unsigned int, vector<STREAM_TYPE*>> &infos);

private:
    FILE* mfp = nullptr;
    unsigned int m_packet_length;
    unsigned int payload_position = 0;
    char* buffer_data;
    bool pat_exit_flag = 0;
    vector<PROGRAM_INFO*> program_numbers; //PMT表数据，可能包含一个NIT
    int program_numbers_size;

    TSHEADER* ts_header;
    PAT* pat;
    PMT* pmt;
};
#endif //TS_PARSE_MASTER_PACKETINFO_H
