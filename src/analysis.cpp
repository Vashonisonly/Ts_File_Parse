#include "../include/analysis.h"

map<int, string> stream_type_map = {{0x1B, "MPEG-4 H264-video"},{0x0F,"MPEG-2 AAC-audio"},
                                    {0x01, "MPEG-1 video"},{0x03, "MPEG-1 audio"},
                                    {0x02, "MPEG-2 higher video"},{0x04, "MPEG-2 haved audio"},
                                    {0x07, "ISO-IEC MGEG"},{0x08, "DSM CC"},
                                    {0x09, "AUXIARILY"},{0x0E, "AUXIARILY"},
                                    {0x10, "MPEG-4 H263-video"},{0x11, "MPEG-4 LAOS-audio"},
                                    {0x12, "MPEG-4 FlexMux"},{0x24, "H265-video"},
                                    {0x42, "AVS-video"},{0x80, "BLUE-RAY-audio"},
                                    {0x81, "BLUE-RAY-audio"},{0x82, "BLUE-RAY-audio"},
                                    {0x83, "BLUE-RAY-audio"},{0x84, "BLUE-RAY-audio"},
                                    {0x85, "BLUE-RAY-audio"},{0x86, "BLUE-RAY-audio"},
                                    {0x87, "ATSC-audio"},{0x90, "subtiting"},
                                    {0xc0, "Digiciper"},{0xc1, "Dolby Digital"},
                                    {0xc2, "ATSC DSM CC"},{0xCF, "ISO-IEC ACC"},
                                    {0xD1, "BBC Dirc"},{0xDB, "ITU-T H264"},
                                    {0xEA, "WIN video-9"}
                     };

ANALYSIS::ANALYSIS(char *file){
    ts_path = file;
}

ANALYSIS::~ANALYSIS(){

}

bool ANALYSIS::execute_parse(){
    FILE* fp = fopen(ts_path, "rb");
    if(fp == nullptr){
        perror("open file lose:");
    }
    //存储文件包的长度以及第一个包的位置
    packetLength = new PacketLength(fp);
    if((mpacket_length = packetLength->get_packet_length()) == -1){
        return false;
    }
    mfile_start_position = packetLength->get_start_position();
    fseek(fp, mfile_start_position, SEEK_SET);

    //获取节目流信息存入map中，map的key值代表每个PMT表的pid值，map的value值代表对应PMT表的流类型信息
    packetInfo = new PacketInfo(fp,mpacket_length);
    if(!packetInfo->get_packet_info(infos)){
        printf("get the pid infos lose!\n");
        return -1;   
    }

    //与用户交互，读入用户想要保存的pid对应的流
    printf("the pid's read had finished---\n");
    for(auto it = infos.begin(); it != infos.end(); ++it){
        printf("the PMT PID is : %d\n",(*it).first);
        for(auto stream_it = (*it).second.begin(); stream_it != (*it).second.end(); ++stream_it){
            printf("the STREAM_TYPE is: %s, it's PID is: %d\n",stream_type_map[(*stream_it)->stream_type].c_str(),(*stream_it)->elementry_PID);
        }
    }
    printf("plese enter the PID which you want to save:\n");
    scanf("%d", &PID);

    //将相对应的pid值的流保存
    fileSava = new FileSava(fp,mfile_start_position);
    fileSava->save(PID,mpacket_length);
    return 1;
}









