#ifndef POLYNOMIAL_ROOT_FINDING_PGA_PROCESS_H
#define POLYNOMIAL_ROOT_FINDING_PGA_PROCESS_H

#include <mpi.h>
#include "../Local.h"

#define MAX_SIZE 1024
using namespace std;
//Abstract Class
class Process {
protected:
    int pid{-1};
    std::string pname;

    int buffer[MAX_SIZE];

public:
    static unsigned long networkSize;

    //TODO: put status message tags
    enum status {
        NOTHING = 0,
        CONTINUE = 1,
        COMPLETE = 2,
        NOTHING_MORE = 3,
        CONTINUE_MORE = 4,
        COMPLETE_MORE = 5,
        SUMMARY = 6,

        CONVERGED = 7,
        STOP = 8,


    };


    const static int MASTER_PID = 0;


    const static int TAG_CONTINUE = 3;
    const static int TAG_QUEUED_UP = 5;
    const static int TAG_STATUS_UPDATE = 4;
    const static int TAG_SUMMARY = 1;
    const static int TAG_FINAL = 2;



    virtual ~Process() = default;

    inline const int getPId()           const { return pid; }
    inline const std::string getPName() const { return pname; }
    inline const unsigned long getNetworkSize() const { return Process::networkSize; }

    static void setNetworkSize(int networkSize);

    virtual int mainProcedure() = 0;


    class NetworkSizeException final : public std::runtime_error{
    public:
        explicit NetworkSizeException(const std::string &msg = "Network size is too small")
                : runtime_error(msg) {}
    };

};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_PROCESS_H
