

#include "Process.h"

#include <utility>


unsigned long Process::networkSize = -1;

void Process::setNetworkSize(int networkSize){
    Process::networkSize = networkSize;
}

