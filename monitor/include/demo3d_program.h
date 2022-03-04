#ifndef DEMO3D_PROGRAM_H
#define DEMO3D_PROGRAM_H
#include "hostprogram.h"

class Demo3dProgram : public HostProgram{
public:
    Demo3dProgram(BoardMC12101 *board, QLabel* label);

    void host_main() override;
};
#endif // DEMO3D_PROGRAM_H
