
#include <fcntl.h> 
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <linux/videodev2.h>

int main (int argc,char* argv[])
{

/* Read Params */

  char *device_name;

  if(argc>1)
  {
     device_name = argv[1];
  } 
  else
  {
     device_name = "/dev/video0";
  }


/* Open Device  */

    int  file_device = open(device_name, O_RDWR, 0);

    if (file_device == -1)
    {
 	printf ("%s error %d, %s\n",device_name, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
	
   

/* Read Params From Device */
	
   struct v4l2_capability device_params;

   if (ioctl(file_device, VIDIOC_QUERYCAP, &device_params) == -1)
    {
 	printf ("\"VIDIOC_QUERYCAP\" error %d, %s\n", errno, strerror(errno));
	exit(EXIT_FAILURE);
    }

	printf("driver : %s\n",device_params.driver);
	printf("card : %s\n",device_params.card);
	printf("bus_info : %s\n",device_params.bus_info);
	printf("version : %d.%d.%d\n",
		((device_params.version >> 16) & 0xFF),
		((device_params.version >> 8) & 0xFF),
		(device_params.version & 0xFF));

	printf("capabilities: 0x%08x\n", device_params.capabilities);
	printf("device capabilities: 0x%08x\n", device_params.device_caps);


/* Close Device */ 

   if (-1 == close (file_device))
   {
	printf ("\"close\" error %d, %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
   }

   file_device = -1;

return 0;
}
