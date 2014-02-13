#include <iostream>
#include "videodevice.h"

using namespace std;

int main(int argc,char* argv[])
{
    string dev_name = "/dev/video0";

    string file_name = "1.raw";

    if (argc>1)
    {
        dev_name = argv[1];

        if (argc > 2)
            file_name = argv[2];
    }

    videodevice *vd = new videodevice();

    vd->openDevice(dev_name);

    vd->getFrame(file_name);

    vd->closeDevice();

    return 0;
}

