//
// Created by letv on 19-6-15.
//

#ifndef TS_PARSE_MASTER_FILESAVA_H
#define TS_PARSE_MASTER_FILESAVA_H

#include "tsheader.h"
#include "pes.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

class FileSava{
public:
    FileSava(FILE* fp, unsigned int file_start_position);
    ~FileSava();

    void save(unsigned int pid, unsigned int packet_length);

private:
    FILE* mfp = nullptr;
    FILE* pes_fp = fopen("demo.pes", "wb");
    FILE* es_fp = fopen("demo.es", "wb");
    TSHEADER* ts_header;
    PES* pes;

    unsigned int per_read_size = 0;
    unsigned int received_length = 0;
    unsigned int available_length = 0;
    unsigned int max_ts_length = 65536;
    unsigned int es_packet_count = 0;
    bool start_flag = 0;
    bool received_flag = 0;
    char* payload_position = NULL;
    char buffer_data[188*204*5] = {};
    char* es_buffer = (char*)malloc(max_ts_length);
    char* current_p = NULL;
};
#endif //TS_PARSE_MASTER_FILESAVA_H
