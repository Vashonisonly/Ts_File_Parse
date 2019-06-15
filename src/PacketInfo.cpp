#include <PacketInfo.h>

//
// Created by letv on 19-6-15.
//
PacketInfo::PacketInfo(FILE *fp, unsigned int packet_length) {
    mfp =fp;
    m_packet_length = packet_length;
    buffer_data = new char[packet_length];
}

PacketInfo::~PacketInfo() {
    delete buffer_data;
    delete pat;
    delete pmt;
    delete ts_header;
}

bool PacketInfo::get_packet_info(map<unsigned int, vector<STREAM_TYPE *>> &infos) {
    while(!feof(mfp)) {
        //将当前包存入buffer_data
        if (fread(buffer_data, m_packet_length, 1, mfp) == 0) {
            perror("fread error:");
            return false;
        }
        payload_position = get_payload_start_positon();
        if(payload_position == -1){
            goto ret;
        }
        if(info_adapter(infos)){
            return true;
        }
        ret:
        if(fseek(mfp, m_packet_length, SEEK_CUR) == -1){
            return false;
        }
    }
    return false;
}

int PacketInfo::get_payload_start_positon() {
    //通过包头tsheader判断payload位置
    ts_header = new TSHEADER(buffer_data);
    /*switch(tsheader->adapation_field_control){
    case 0:
        break;
    case 1:
    }*/
    if(ts_header->adapation_field_control == 0 || ts_header->adapation_field_control == 2){
        printf("no payload, continue the next packet");
        return -1;
    }else if(ts_header->adapation_field_control == 1){
        payload_position = 4;
    }else{
        //pes header头部第一个字节代表剩下长度，再加上它本身
        payload_position = 4 + buffer_data[4] +1;
    }
    //对含有有效负载偏移的情况。当这个值为1，说明含有1个字节的FieldPointer，这个FieldPointer的值又代表了PSI，SI表在payload开头占据的字节数。
    if(ts_header->payload_uint_start_indicator/* == 1*/){
        payload_position += buffer_data[payload_position] + 1;
    }
    return payload_position;
}

bool PacketInfo::info_adapter(map<unsigned int, vector<STREAM_TYPE*>> &infos) {
    //分析PID值，PAT表只需要分析一次即可，以后不再分析。得到一个map，first代表pid值，second是pmt表中的stream_types信息，比如音频，食品。
    if(ts_header->PID == 0x00 && !pat_exit_flag){
        pat = new PAT(buffer_data + payload_position);
        pat->get_program_info(program_numbers);
        pat_exit_flag = 1;
        program_numbers_size = program_numbers.size();
    }else if(pat_exit_flag){
        //也可以先根据PAT表算出PMT表的PID值，由ts header直接判断出是不是PMT，再进入处理
        pmt = new PMT(buffer_data + payload_position);
        if(program_numbers_size && (pmt->table_id == 0x02)){

            for(auto program_it = program_numbers.begin(); program_it != program_numbers.end(); ++program_it){

                if((*program_it)->program_number == pmt->program_number){

                    vector<STREAM_TYPE*> stream_types;
                    pmt->get_stream_types(stream_types);
                    infos.insert(make_pair((*program_it)->PMT_PID, stream_types));
                    if(infos.size() == program_numbers_size){
                        return true;
                    }
                    // for(auto stream_it = stream_types.begin(); stream_it != stream_types.end(); ++stream_it){
                    //     infos.insert(make_pair((*stream_it)->elementry_PID, )
                    // }
                }
            }
        }
    }else{
        return false;
    }
    return false;
}