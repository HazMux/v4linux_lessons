#ifndef VIDEODEVICE_H
#define VIDEODEVICE_H

#include <iostream>

using namespace std;

struct buffer {
    void   *start;
    size_t  length;
};

class videodevice
{
public:
    videodevice();
    void openDevice(string dev_name);
    void getFrame(string file_name);
    void closeDevice();

private:
    void errno_exit(string err_str);
    int xioctl(int fd, int request, void *arg);
    void initMMAP();
    void startCapturing();
    int readFrame(string file_name);
    void stopCapturing();
    void freeMMAP();

    int  fd;
    string fileDevicePath;
    struct buffer *devbuffer;
};

#endif // VIDEODEVICE_H
