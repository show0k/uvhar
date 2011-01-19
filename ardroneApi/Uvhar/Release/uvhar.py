import Image, ImageDraw, time
import sys
from opencv.cv import *
from opencv.highgui import *



class Uvhar:


     exitOnNextUpdate = False

     # keeping track of the image we use
     counter = -1 
     # last known coordinates of a find object in the picture
     point = None
     # steering values
     roll = 0
     pitch = 0
     gaz = 0
     yaw = 0
     # defining a window before we fly towards the object 
     lowerX = 135
     upperX = 170
     lowerY = 150  
     upperY = 200  # Higher in the coordinate system, but lower in the image!


     # Images 
     image = None
     imageH = None
     imageS = None
     imageV = None
     tempResultImage = None
     resultImage = None
     targetImage = None
     matchImage = None

     """ :(
     # histogram of what we are looking for
     sourceHistogram = None
     hBins = 30 # python api example
     sBins = 32 # python api example
     vBins = 32 # 32 because.. why not?
     # hue varies from 0 (~0 deg red) to 180 (~360 deg red again), python api example 
     hRange = [0, 180]
     # saturation varies from 0 (black-gray-white) to 255 (pure spectrum color), python api example
     sRange = [0, 255]
     vRange = [0, 255] # I think this applies for value as well
     """

     # width and height of the image, we might need to change this
     # when we switch to the bottom camera (or we can detect it, somehow)
     imageWidth = 320
     imageHeight = 240

     # bounding hsv values to spot something pink
     hMin = 327/2
     hMax = 347/2
     sMin = 68/0.393
     sMax = 86/0.393
     vMin = 70/0.393
     vMax = 100/0.393 

     # window names
     mainWindowName = "Image"
     processedWindowName = "Processed Image"
     processedWindowName2 = "Processed Image 2"


     def __init__(self):
         print "Uvhar class contructor called.\n"
         self.imageH = cvCreateImage(cvSize(self.imageWidth, self.imageHeight), 8, 1)
         self.imageS = cvCreateImage(cvSize(self.imageWidth, self.imageHeight), 8, 1)
         self.imageV = cvCreateImage(cvSize(self.imageWidth, self.imageHeight), 8, 1)
         self.resultImage= cvCreateImage(cvSize(self.imageWidth, self.imageHeight), 8, 1)
         self.tempResultImage = cvCreateImage(cvSize(self.imageWidth, self.imageHeight), 8, 3)

         cmpw = 10
         cmph = 10

         self.matchImage = cvCreateImage(cvSize(self.imageWidth - cmpw + 1, self.imageHeight - cmph+ 1), 32, 1)

         self.targetImage = cvCreateImage(cvSize(cmpw, cmph), 8, 1)
         cvSet(self.targetImage, CV_RGB(255, 255, 255))

         #cvShowImage("targetImage", self.targetImage)
         #cvWaitKey(0)
    
         """ 
         # create histogram
         self.sourceHistogram = cvCreateHist([self.hBins, self.sBins, self.vBins], CV_HIST_ARRAY, [self.hRange, self.sRange, self.vRange])
         # load image and convert to hsv
         sourceImage = cvLoadImage("pinkObject.jpg", CV_LOAD_IMAGE_COLOR)
         cvCvtColor(sourceImage, sourceImage, CV_BGR2HSV)
         # extract h, s and v planes
         hPlane = cvCreateImage(cvSize(30, 18), 8, 1)
         sPlane = cvCreateImage(cvSize(30, 18), 8, 1)
         vPlane = cvCreateImage(cvSize(30, 18), 8, 1)
         cvSplit(sourceImage, hPlane, sPlane, vPlane, None)
         planes = [hPlane, sPlane, vPlane]

         # calculate histogram
         cvCalcHist(planes, self.sourceHistogram, 0, None)
         """

         # already open the windows :O
         cvNamedWindow(self.mainWindowName) # if window does not resize automagically add CV_WINDOW_AUTOSIZE
         cvNamedWindow(self.processedWindowName)
         cvNamedWindow(self.processedWindowName2)
         cvMoveWindow(self.mainWindowName, 0, 50)
         cvMoveWindow(self.processedWindowName, 340, 50)
         cvMoveWindow(self.processedWindowName2, 680, 50)
         cvSetMouseCallback(self.mainWindowName, self.setExitOnNextUpdate, None)
         cvSetMouseCallback(self.processedWindowName, self.setExitOnNextUpdate, None)
         cvSetMouseCallback(self.processedWindowName2, self.setExitOnNextUpdate, None)
         cvWaitKey(1)

     def update(self, cTuple):
         # sometimes c isn't ready with writing the file
         # so we always take the previous image
         if (self.counter != cTuple[0] - 1):
             #print "new counter received %d" % newCounter
             self.counter = cTuple[0] - 1
             self.loadNewImage()
             self.findPicture()
             self.thinkAboutPoint() # check if the info in self.point is useful and acts on it
	 print "battery level: %4.2f" % cTuple[1]
         #print "navdata: battery level: %4.2f, theta: %4.2f, phi: %4.2f, psi %4.2f, altitude %4.2f, vx %4.2f, vy %4.2f, vz %4.2f" % (cTuple[1], cTuple[2], cTuple[3], cTuple[4], cTuple[5], cTuple[6], cTuple[7], cTuple[8])  
        
         cvWaitKey(4)
         return [self.exitOnNextUpdate, self.roll, self.pitch, self.gaz, self.yaw]

     def resetSteeringValues(self):
         self.roll = 0
         self.pitch = 0
         self.gaz = 0
         self.yaw = 0



     def thinkAboutPoint(self):
         self.resetSteeringValues()

         # keep turnin' untill we have more interesting information
         if (self.point == None):
             return
	 if(self.point.x == 0 or self.point.y == 0):
             print "\tNo point found, keep on turnin'!\n"
             self.yaw = 0.3
             return
         print "\tpoint found: "
         # bring the object to the centre of the screen
         # for x with rolling
         if (self.point.x < self.lowerX):
             print "x is too much to the left!"
             self.yaw = -0.1
         elif (self.point.x > self.upperX):
             print "x is too much to the right!"
             self.yaw = 0.1

         # for y with gaz
         elif (self.point.y < self.lowerY):
             print "y is too low!"
             self.gaz = 0.5
         elif (self.point.y > self.upperY):
             print "y is too high"
             self.gaz = -0.5

         # otherwise, fly towards the target!
         else: 
             print "flyin' towards the target!"
             self.pitch = -0.1
         print "\n"

     # we need to find some way to call this baby when we stop
     def exit(self):
         print "\tpython exit called\n"
         cvDestroyAllWindows()
   
     # loads image with an image from file using the counter, processes it and 
     # shows the result 
     def loadNewImage(self):
         filename = "images/frame%05d.jpg" % (self.counter)
         #print filename
         self.image = cvLoadImage(filename, CV_LOAD_IMAGE_COLOR)
         cvShowImage(self.mainWindowName, self.image)
         cvShowImage(self.processedWindowName, self.resultImage)
         cvShowImage(self.processedWindowName2, self.matchImage)

     # processes image and sets the result in resultImage
     # also returns a point that contains the found  
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
       
         cvMatchTemplate(self.resultImage, self.targetImage, self.matchImage, CV_TM_SQDIFF_NORMED) 
         _, _, self.point, _ = cvMinMaxLoc(self.matchImage)
         #print "%d, %d" % (self.point.x, self.point.y)
         cvRectangle(self.matchImage, (self.lowerX, self.lowerY), (self.upperX, self.upperY), CV_RGB(255,0,0))

     def setExitOnNextUpdate(self, event, x, y, flags, param):
         if (event == CV_EVENT_RBUTTONDOWN):
             print "\tPython recognizes exit command\n"
             self.exitOnNextUpdate = True
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
     i = 400;
     while (i < 500):
         uvhar.update([i, 0, 0, 0, 0])
         time.sleep(0.0667)
         i = i + 1
     uvhar.exit()

         
