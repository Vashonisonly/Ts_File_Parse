#include <FileSava.h>


//
// Created by letv on 19-6-15.
//
FileSava::FileSava(FILE* fp, unsigned int file_start_position) {
    mfp = fp;
    if(mfp == NULL || pes_fp == NULL || es_fp == NULL){
        perror("open file lose:");
    }
    //使这个文件指针回到第一个包开始的位置
    if(fseek(mfp, file_start_position, SEEK_SET) == -1){
        perror("seek the file lose:");
    }
}
FileSava::~FileSava() {
    delete ts_header;
    delete pes;
}

void FileSava::save(unsigned int pid, unsigned int packet_length) {
    while(!feof(mfp)){
        per_read_size = fread(buffer_data, 1, sizeof(buffer_data), mfp);
        if(per_read_size == 0){
            perror("fread lose:");
        }
        current_p = buffer_data;
        //处理读到的数据
        while(current_p < buffer_data + per_read_size){
            ts_header = new TSHEADER(current_p);
            if(ts_header->PID == pid){
                es_packet_count++;
                if(ts_header->adapation_field_control == 1){
                    payload_position = current_p + 4;
                }else if(ts_header->adapation_field_control == 3){
                    payload_position = current_p + 4 + current_p[4] + 1;
                }
                //不是完整帧的直接丢弃
                if(ts_header->payload_uint_start_indicator != 0){
                    start_flag =1;
                }
                if(start_flag && payload_position && pes_fp){
                    available_length = packet_length + current_p -payload_position;
                    //写入pes文件
                    fwrite(current_p, available_length, 1, pes_fp);
                    //写入es文件
                    if(ts_header->payload_uint_start_indicator != 0){
                        if(received_length > 0){
                            pes = new PES(es_buffer);
                            if(pes->packet_start_code_prefix != 0x000001){
                                printf("pes is not correct.received %d es packet\n",es_packet_count);
                                return;
                            }
                            if(pes->PES_packet_data_length != 0){
                                fwrite(pes->elementy_stream_position, received_length, 1, es_fp);
                            }
                            memset(es_buffer, 0, received_length);
                            received_length = 0;
                        }
                        received_flag = 1;
                    }
                    if(received_flag){
                        if(received_length + available_length > max_ts_length){
                            max_ts_length *= 2;
                            es_buffer = (char*)realloc(es_buffer,max_ts_length);
                        }
                        memcpy(es_buffer + received_length, payload_position, available_length);
                        received_length += available_length;
                    }

                }
            }
            current_p += packet_length;
        }
    }
    printf("the packet number is: %d\n"
           "and the demo.pes, demo.es has been saved at current directory.\n",es_packet_count);
    if(es_buffer){
        free(es_buffer);
    }
    if(es_fp){
        fclose(es_fp);
    }
    if(pes_fp){
        fclose(pes_fp);
    }
    if(mfp){
        fclose(mfp);
    }
}
