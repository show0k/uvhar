import Image, ImageDraw
import atexit, time
from opencv import cv
from opencv.highgui import *

atexit.register(exit)

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
          


#image = cv.LoadImageM("images/frame00001.jpg")

if __name__ == "__main__":
     uvhar = Uvhar()
     i = 1;
     while (i < 251):
         uvhar.update(i)
         time.sleep(0.10)
         i = i + 1
     uvhar.exit()

         
