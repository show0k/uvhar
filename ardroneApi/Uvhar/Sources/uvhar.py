import Image, ImageDraw, time
from opencv.cv import *
from opencv.highgui import *



class Uvhar:


     exitOnNextUpdate = False

     # keeping track of the image we use
     counter = -1 

     # Images 
     image = None
     imageH = None
     imageS = None
     imageV = None
     tempResultImage = None
     resultImage = None

     # width and height of the image, we might need to change this
     # when we switch to the bottom camera (or we can detect it, somehow)
     imageWidth = 320
     imageHeight = 240

     # bounding hsv values to spot something pink
     hMin = 327/2
     hMax = 347/2
     sMin = 66/0.393
     sMax = 86/0.393
     vMin = 88/0.393
     vMax = 100/0.393 

     # window names
     mainWindowName = "Image"
     processedWindowName = "Processed Image"


     def __init__(self):
         print "Uvhar class contructor called.\n"
         self.imageH = cvCreateImage(cvSize(self.imageWidth, self.imageHeight), 8, 1)
         self.imageS = cvCreateImage(cvSize(self.imageWidth, self.imageHeight), 8, 1)
         self.imageV = cvCreateImage(cvSize(self.imageWidth, self.imageHeight), 8, 1)
         self.tempResultImage = cvCreateImage(cvSize(self.imageWidth, self.imageHeight), 8, 3)
         self.resultImage = cvCreateImage(cvSize(self.imageWidth, self.imageHeight), 8, 1)

         # already open the windows :O
         cvNamedWindow(self.mainWindowName) # if window does not resize automagically add CV_WINDOW_AUTOSIZE
         cvNamedWindow(self.processedWindowName)
         cvSetMouseCallback(self.mainWindowName, self.setExitOnNextUpdate, None)
         cvSetMouseCallback(self.processedWindowName, self.setExitOnNextUpdate, None)

     def update(self, newCounter):
         # sometimes c isn't ready with writing the file
         # so we always take the previous image
         if (newCounter > 0):
             newCounter -= 1
         if (self.counter != newCounter ):
             #print "new counter received %d" % newCounter
             self.counter = newCounter
             self.loadNewImage();
         # return 33 if we want to quit this thing!
         if (self.exitOnNextUpdate):
             return 33 

     # we need to find some way to call this baby when we stop
     def exit(self):
         print "\tpython exit called\n"
         cvDestroyAllWindows()
         #destroyWindow(self.mainWindowName)
         #destroyWindow(self.processedWindowName)
         return 42
   
     # loads image with an image from file using the counter, processes it and 
     # shows the result 
     def loadNewImage(self):
         filename = "images/frame%05d.jpg" % (self.counter)
         #print filename
         self.image = cvLoadImage(filename, CV_LOAD_IMAGE_COLOR)
         self.findPicture()
         cvShowImage(self.mainWindowName, self.image)
         cvShowImage(self.processedWindowName, self.resultImage)
         cvWaitKey(4)

     # processes image and sets the result in resultImage
     def findPicture(self):
         if (self.image == None):
             print "image is null\n"
             return None
         if (self.resultImage == None):
             print "result image is null\n"
             return None

         # convert to hsv
         cvCvtColor(self.image, self.tempResultImage, CV_BGR2HSV) 

         # split the image in h, s and v values
         cvSplit(self.tempResultImage, self.imageH, self.imageS, self.imageV, None)  

         # only keep those values in range
         cvInRangeS(self.imageH, self.hMin, self.hMax, self.imageH)
         cvInRangeS(self.imageS, self.sMin, self.sMax, self.imageS)
         cvInRangeS(self.imageV, self.vMin, self.vMax, self.imageV)

         # "and" to make sure we only end up with those values that 
         # are range on every channel 
         cvAnd(self.imageH, self.imageS, self.resultImage)
         cvAnd(self.resultImage, self.imageV, self.resultImage)

     def setExitOnNextUpdate(self, event, x, y, flags, param):
         if (event == CV_EVENT_RBUTTONDOWN):
             print "\tPython recognizes exit command\n"
             exitOnNextUpdate = True
             if __name__ == "__main__":
                 exit()

     def test(self):
         self.image = cvLoadImage("example.jpg", CV_LOAD_IMAGE_COLOR)  
         cvShowImage(self.mainWindowName, self.image)
         cvSetMouseCallback(self.mainWindowName, self.setExitOnNextUpdate, None)
         cvWaitKey(0)

#image = cv.LoadImageM("images/frame00001.jpg")

if __name__ == "__main__":
     uvhar = Uvhar()
     uvhar.test()
     # from the range of gimp hsv to the range of opencv hsv      
     #uvhar.testPicture(230, 260, 60, 80, 128, 150)
     """
     i = 1;
     while (i < 251):
         uvhar.update(i)
         time.sleep(0.10)
         i = i + 1
     uvhar.exit()
     """

         
