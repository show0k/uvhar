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
         imageH = cvCreateImage(cvSize(320, 240), 8, 1)
         imageS = cvCreateImage(cvSize(320, 240), 8, 1)
         imageV = cvCreateImage(cvSize(320, 240), 8, 1)

         # make sure we don't lose the orignal image
         hsvImage = cvCloneImage(image) 

         # convert to hsv
         cvCvtColor(image, hsvImage, CV_BGR2HSV) 

         # split the image in h, s and v values
         cvSplit(hsvImage, imageH, imageS, imageV, None)  

         # only keep those values in range
         cv.cvInRangeS(imageH, hMin, hMax, imageH)
         cv.cvInRangeS(imageS, sMin, sMax, imageS)
         cv.cvInRangeS(imageV, vMin, vMax, imageV)


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

         
