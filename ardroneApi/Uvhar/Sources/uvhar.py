import Image, ImageDraw, time
from opencv.cv import *
from opencv.highgui import *


class Uvhar:

     counter = -1 
     image = None
 
     def __init__(self):
        print "Uvhar class contructor called.\n"
        pass 

     def update(self, newCounter):
         # sometimes c isn't ready with writing the file
         # so we always take the previous image
         if (newCounter > 0):
             newCounter -= 1
         if (self.counter != newCounter ):
             #print "new counter received %d" % newCounter
             self.counter = newCounter
             self.loadNewImage();
         return 1337

     # we need to find some way to call this baby when we stop
     def exit(self):
         print "exit called\n"
         destroyWindow("Image");
         return 0
    
     def loadNewImage(self):
         filename = "images/frame%05d.jpg" % (self.counter)
         #print filename
         image = cvLoadImage(filename, CV_LOAD_IMAGE_COLOR)
         cvShowImage("image", image)
         cvWaitKey(2)

     def testPicture(self):
         image = cvLoadImage("example.jpg", CV_LOAD_IMAGE_COLOR)
         #cvShowImage("image", image)
         #cvWaitKey(0)
         imageh = cvCreateImage(cvSize(320, 240), 8, 1)
         images = cvCreateImage(cvSize(320, 240), 8, 1)
         imagev = cvCreateImage(cvSize(320, 240), 8, 1)

         hsv_image = cvCloneImage(image) # temporary copy of the frame
         cvCvtColor(image, hsv_image, CV_BGR2HSV) # convert to HSV

         cvSplit(image, imageh, images, imagev, None)  


#image = cv.LoadImageM("images/frame00001.jpg")

if __name__ == "__main__":
     uvhar = Uvhar()
     uvhar.testPicture()
     """
     i = 1;
     while (i < 251):
         uvhar.update(i)
         time.sleep(0.10)
         i = i + 1
     uvhar.exit()
     """

         
