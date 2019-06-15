//
// Created by letv on 19-6-14.
//

#ifndef TS_PARSE_MASTER_PACKETLENGTH_H
#define TS_PARSE_MASTER_PACKETLENGTH_H

#include <stdio.h>

class PacketLength{
public:
    PacketLength(FILE* fp);
    ~PacketLength();
    unsigned int get_packet_length();
    unsigned int get_start_position();

private:
    bool judge_packet_length(unsigned int packet_length);
private:
    FILE* mfp = nullptr;
    unsigned int packet_length_188 = 188;
    unsigned int packet_length_204 = 204;
    unsigned int file_start_position = 0;
};
#endif //TS_PARSE_MASTER_PACKETLENGTH_H
