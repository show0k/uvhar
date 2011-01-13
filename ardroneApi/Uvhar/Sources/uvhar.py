import Image, ImageDraw, time
from opencv import cv
from opencv.highgui import *


class Uvhar:

     counter = -1 
     image = None
 
     def __init__(self):
        print "Uvhar class contructor called.\n"
        pass 


     def update(self, newCounter):
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
         #if (self.image != None):
         #self.image.close()
         filename = "images/frame%05d.jpg" % (self.counter)
         print filename
         #self.image = Image.open(filename)
         #ImageDraw.Draw(self.image).show() 
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

         
