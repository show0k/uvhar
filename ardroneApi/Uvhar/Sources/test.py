import Image, ImageDraw, time
import sys
from opencv.cv import *
from opencv.highgui import *
from math import sqrt

class Test:

     oneWindowName = "one"
     twoWindowName = "two"
     threeWindowName = "three"
     
     mainImage1 = None
     mainImage2 = None
     gaussianImage = None
     resultImage = None

     gaussianWidth = 50
     gaussianHeight = 50

     point = None
     hsv = dict(h=337, hLow=10, hHigh=10, s=78, sLow=10, sHigh=10, v=85, vLow=70, vHigh=100)

     # bounding hsv values to spot something pink
     hMin = 327/2
     hMax = 347/2
     sMin = 68/0.393
     sMax = 86/0.393
     vMin = 70/0.393
     vMax = 100/0.393 

     windowWidth = 100
     windowHeight = 100

     def __init__(self):
         cvNamedWindow(self.oneWindowName)
         cvNamedWindow(self.twoWindowName)
         cvNamedWindow(self.threeWindowName)
         cvMoveWindow(self.oneWindowName, 20, 50)
         cvMoveWindow(self.twoWindowName, 300, 50)
         cvMoveWindow(self.threeWindowName, 680, 50)

         self.mainImage1 = cvLoadImage("images/frame00098.jpg", CV_LOAD_IMAGE_COLOR)
         self.doImage(self.mainImage1)
         self.mainImage2 = cvLoadImage("images/frame00099.jpg", CV_LOAD_IMAGE_COLOR)
         self.doImage(self.mainImage2)
                  
         
     def doImage(self, image):
         matchImage = self.find(image)

         widthOffset = self.windowWidth /2
         heightOffset = self.windowHeight / 2
         cvRectangle(matchImage, (self.point.x - widthOffset, self.point.y - heightOffset), (self.point.x + widthOffset, self.point.y + heightOffset), CV_RGB(255,0,0))

         # decrease thresholds

         self.gaussianImage = cvCreateImage(cvSize(self.windowWidth, self.windowHeight), 8, 1)
         
         for x in range(0, self.windowWidth):
             for y in range(0, self.windowHeight):
                 cvSet(self.gaussianImage, sqrt((x-25)*(x-25)+(y-25)*(y-25)))

         cvShowImage(self.oneWindowName, image)
         cvShowImage(self.twoWindowName, matchImage) 
         cvWaitKey(0) 



     def find(self, inImage):
         cmpw = 10
         cmph = 10

         targetImage = cvCreateImage(cvSize(cmpw, cmph), 8, 1)
         cvSet(targetImage, CV_RGB(255, 255, 255))

         resultImage= cvCreateImage(cvSize(320, 240), 8, 1)
         tempResultImage = cvCreateImage(cvSize(320, 240), 8, 3)
         imageH = cvCreateImage(cvSize(320, 240), 8, 1)
         imageS = cvCreateImage(cvSize(320, 240), 8, 1)
         imageV = cvCreateImage(cvSize(320, 240), 8, 1)
         matchImage = cvCreateImage(cvSize(320 - cmpw + 1, 240 - cmph+ 1), 32, 1)

         # convert to hsv
         cvCvtColor(inImage, tempResultImage, CV_BGR2HSV) 
     

         # split the image in h, s and v values
         cvSplit(tempResultImage, imageH, imageS, imageV, None)  

         cvInRangeS(imageH, self.hMin, self.hMax, imageH)
         cvInRangeS(imageS, self.sMin, self.sMax, imageS)
         cvInRangeS(imageV, self.vMin, self.vMax, imageV)

         """
         # only keep those values in range
         cvInRangeS(imageH, self.hsv['h'] - self.hsv['hLow'], self.hsv['h'] + self.hsv['hHigh'], imageH)
         cvInRangeS(imageH, self.hsv['s'] - self.hsv['sLow'], self.hsv['s'] + self.hsv['sHigh'], imageH)
         cvInRangeS(imageH, self.hsv['v'] - self.hsv['vLow'], self.hsv['v'] + self.hsv['vHigh'], imageH)
         """

         # "and" to make sure we only end up with those values that 
         # are range on every channel 
         cvAnd(imageH, imageS, resultImage)

         cvAnd(resultImage, imageV, resultImage)
 
         cvMatchTemplate(resultImage, targetImage, matchImage, CV_TM_SQDIFF_NORMED) 
         minValue, maxValue, self.point, _ = cvMinMaxLoc(matchImage)
         if (self.point.x != 0 and self.point.y != 0):
             print "min value: %f, max value: %f" % (minValue, maxValue)
         else:
             print "nothing found"
         return matchImage 


if __name__ == "__main__":
     Test()

