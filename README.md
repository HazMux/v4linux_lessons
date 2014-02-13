<h1>Video for linux lessons</h1>

<b><h2>Lesson1</h2>

<h4>compile:</h4></b>

<code>$ gcc catvd.c -o catvd</code>

<b><h4>execute:<h4/><b/>

<code>$./catvd dev_name </code>   

where <i>dev_name</i> - name of video device  (like  "/dev/video0")

<hr>
<b><h2>Lesson2</h2>

<h4>compile:</h4></b>

<code>$ cmake .</code>

<code>$ make </code>

<b><h4>execute:<h4/><b/>

<code>$./getimage dev_name out_image </code>   

where <i>dev_name</i> - name of video device  (like  "/dev/video0")

where <i>out_image</i> - name of captured image  (like  "image.raw")
