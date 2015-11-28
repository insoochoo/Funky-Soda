#include <iostream>
#include <string>

#include "MPRNG.h"
#include "config.h"
//#include "student.h"

using namespace std;

MPRNG mprng;


void usage(char *argv) {
    cerr << "Usage: " << argv << "  err msg" << endl;
    exit(-1);
}

void uMain::main(){
    int seed = getpid();
    string fname = "config.txt";

    try{
        switch(argc) {
            case 3:
                seed = stoi(argv[2]);
                if(seed <= 0){
                    usage(argv[0]);
                }
            case 2:
                fname = argv[1];
                break;
            default:
                usage(argv[0]);
        }
    }
    catch(...){
        usage(argv[0]);
    }

    mprng.seed(seed);

    ConfigParms config;
    processConfigFile(fname.c_str(), config);


}
