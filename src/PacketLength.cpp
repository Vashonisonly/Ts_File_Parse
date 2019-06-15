//
// Created by letv on 19-6-15.
//
#include "../include/PacketLength.h"

PacketLength::PacketLength(FILE *fp) {
    mfp = fp;
}
PacketLength::~PacketLength() {

}

unsigned int PacketLength::get_packet_length() {
    if(mfp == nullptr) {
        return -1;
    }
    //ts包前面可能有同步字节，我们循环跳过
    while(!feof(mfp)){
        if(getc(mfp) == 0x47){
            if(judge_packet_length(packet_length_188)){
                return packet_length_188;
            }
            //重新定位到0x47位置来判断是不是204的包
            fseek(mfp, file_start_position, SEEK_SET);
            if(judge_packet_length(packet_length_204)){
                return packet_length_204;
            }
            fseek(mfp, file_start_position, SEEK_SET);
        }
        file_start_position++;
    }
    return -1;
}

unsigned int PacketLength::get_start_position() {
    return file_start_position;
}

bool PacketLength::judge_packet_length(unsigned int packet_length) {
    if(fseek(mfp, file_start_position + 1, SEEK_SET)== -1){
        perror("seek lose:");
    }
    for(int i = 0; i != 10; ++i){
        if(fseek(mfp, packet_length-1, SEEK_CUR) == -1){
            perror("fssek error:");
            return false;
        }
        if(feof(mfp)){
            return false;
        }
        if(fgetc(mfp) != 0x47){
            printf("fgetc is not the 0x47\n");
            return false;
        }

    }
    printf("---------------------------------------\n"
           "the transport stream packet length is %d\n",packet_length);
    return true;
}
