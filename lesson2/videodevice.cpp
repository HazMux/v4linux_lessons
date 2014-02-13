#include "videodevice.h"

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sstream>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <linux/videodev2.h>

videodevice::videodevice()
{

}

void videodevice::errno_exit(string err_str)
{
    cout << "\"" << err_str << "\": " << errno << ", " << strerror(errno) << endl;
    exit(EXIT_FAILURE);
}

int videodevice::xioctl(int fd, int request, void *arg)
{
       int r;

       r = ioctl (fd, request, arg);

       if(r == -1)
       {
           if (errno == EAGAIN)
               return EAGAIN;

           stringstream ss;

           ss << "ioctl code " << request << " ";

           errno_exit(ss.str());
       }

       return r;
}

void videodevice::openDevice(string dev_name)
{
    fileDevicePath = dev_name;

    fd = open(fileDevicePath.c_str(), O_RDWR /* required */ | O_NONBLOCK, 0);

    if (fd == -1)
    {
        stringstream str;
        str << "Cannot open '" << fileDevicePath << "'";
        errno_exit(str.str());
    }

    cout << "Open device " <<  fileDevicePath << endl;
}

void videodevice::closeDevice()
{
    if (close(fd) == -1)
        errno_exit("close");

    fd = -1;

    cout << "Close device " <<  fileDevicePath << endl;
}


void videodevice::getFrame(string file_name)
{
    initMMAP();

    startCapturing();

    long int i = 0;

    for (;;)
    {
        if(readFrame(file_name))
           break;

        i++;
    }

    cout << "iter == " << i << endl;


    stopCapturing();

    freeMMAP();
}

void videodevice::initMMAP()
{
    struct v4l2_requestbuffers req;

    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    xioctl(fd, VIDIOC_REQBUFS, &req);

    devbuffer = (buffer*) calloc(req.count, sizeof(*devbuffer));

    struct v4l2_buffer buf;

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.index       = 0;

    xioctl(fd, VIDIOC_QUERYBUF, &buf);

    devbuffer->length = buf.length;
    devbuffer->start =
               mmap(NULL,
                    buf.length,
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED,
                    fd,
                    buf.m.offset);

    if (devbuffer->start == MAP_FAILED)
        errno_exit("mmap");


    cout <<  "Init mmap"<< endl;
}

void videodevice::startCapturing()
{
    struct v4l2_buffer buf;

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;

    xioctl(fd, VIDIOC_QBUF, &buf);


    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    xioctl(fd, VIDIOC_STREAMON, &type);


    cout << "Start capturing" << endl;
}

int videodevice::readFrame(string file_name)
{
    struct v4l2_buffer buf;

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    if (xioctl(fd, VIDIOC_DQBUF, &buf) == EAGAIN)
            return 0;

    buffer *temp = devbuffer;

    FILE *out_file = fopen(file_name.c_str(),"w");

    fwrite(temp->start,temp->length,1,out_file);

    fclose(out_file);

    cout << "read frame from buffer and write to file" << endl;

    return 1;
}

void videodevice::stopCapturing()
{
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    xioctl (fd, VIDIOC_STREAMOFF, &type);

    cout << "stop Capturing" << endl;
}


void videodevice::freeMMAP()
{
    if (munmap(devbuffer->start, devbuffer->length) == -1)
        errno_exit("munmap");

    free (devbuffer);

    cout << "free mmap" << endl;
}
