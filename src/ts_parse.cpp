#include <iostream>
#include <vector>
#include <map>

#include "../include/analysis.h"

using namespace std;

int main(int argc,char* argv[])
{
//    if(argc <= 1){
//        printf("please input the ts file as argument\n");
//        return -1;
//    }
//
//    ANALYSIS analysis(argv[1]);
//    analysis.execute_parse();
    char ts_file[] = "test.ts";
    ANALYSIS* analysis = new ANALYSIS(ts_file);
    analysis->execute_parse();
}
