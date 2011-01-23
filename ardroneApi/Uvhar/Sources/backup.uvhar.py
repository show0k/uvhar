import Image, ImageDraw, time
import sys
from opencv.cv import *
from opencv.highgui import *



class Uvhar:


    exitOnNextUpdate = False

    # keeping track of the image we use
    counter = -1 
    # Counter to hover before we fly towards target
    hoverCounter = 0
    hoverCounterMax = 15
    # time granted to seek the object with the bottom camera
    bottomCameraCounter = 0
    bottomCameraMax = 90
    # time after take off before we are start sending commands
    takeOffTime = 120

    # indicator of what camera we are using [..bottom camera, 0, front camera.. ]
    videoSwitch = 1
    # so we can detect changes to send them to c
    oldVideoSwitch = 1
    # we only want to switch when we know we are in the right position
    # it is also a switch made out of a can
    canSwitch = False
    # we first want to make sure the object is in the centre, and only then
    # then fly towards it
    canFly = False
    
    # the information that c supplies us with
    cTuple = None
    # last known coordinates of a find object in the picture
    point = None
    lastKnown = None
    # steering values
    roll = 0
    pitch = 0
    gaz = 0
    yaw = 0
    turnValue = 1
    # a window that we want the target in our image to be in. These 
    # are define in initImages
    lowerX = 0
    upperX = 0
    lowerY = 0 
    upperY = 0 # Higher in the coordinate system, but lower in the image!

    # Defining a square where we can lose the object before we start 
    # hoovering above it
    lastKnownLowerX = 120
    lastKnownUpperX = 185
    lastKnownLowerY = 210
    lastKnownUpperY = 240


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

    # width and height of the image
    imageWidth = 320
    imageHeight = 240

    # bounding hsv values to spot something pink
    hMin = 327/2
    hMax = 347/2
    sMin = 58/0.393
    sMax = 95/0.393
    vMin = 50/0.393
    vMax = 100/0.393 

    # window names
    mainWindowName = "Image"
    processedWindowName = "Processed Image"
    processedWindowName2 = "Processed Image 2"


    def __init__(self):
        print "Uvhar class contructor called."
        self.initImages()
       
        #cvShowImage("targetImage", self.targetImage)
        #cvWaitKey(0)

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

    def initImages(self):
        print "\tCalling initImages"
        # setting to the right size 
        if (self.videoSwitch < 0):
            self.imageWidth = 320
            self.imageHeight = 144
            self.lowerX = 58
            self.upperX = 108
            self.lowerY = 42
            self.upperY = 92
        else: 
            self.imageWidth = 320
            self.imageHeight = 240
            self.lowerX = 135
            self.upperX = 170
            self.lowerY = 150  
            self.upperY = 180 #210 
# creating the images
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
 

    def update(self, cTuple):
        self.cTuple = cTuple
        # take some time to fully take off before we start doin' things
        if (cTuple[0]>self.takeOffTime):
            #print "new counter received %d" % newCounter
            self.counter = cTuple[0] - 1
            self.loadNewImage()
            if (self.findPicture()):
                # check if the info in self.point is useful and acts on it
                if (self.videoSwitch > 0):
                    self.thinkAboutPoint()
                else:
                    self.thinkAboutLastKnown()
    

        elif(self.counter != cTuple[0] - 1):
            self.counter = cTuple[0] - 1
            self.loadNewImage()
            self.findPicture()
            
        #print "navdata: battery level: %4.2f, theta: %4.2f, phi: %4.2f, psi %4.2f, altitude %4.2f, vx %4.2f, vy %4.2f, vz %4.2f" % (cTuple[1], cTuple[2], cTuple[3], cTuple[4], cTuple[5], cTuple[6], cTuple[7], cTuple[8])  
       
        cvWaitKey(4)
        returnArray = [self.exitOnNextUpdate, self.roll, self.pitch, self.gaz, self.yaw, self.oldVideoSwitch != self.videoSwitch]
        self.oldVideoSwitch = self.videoSwitch
        return returnArray

    def resetSteeringValues(self):
        self.roll = 0
        self.pitch = 0
        self.gaz = 0
        self.yaw = 0



    def thinkAboutPoint(self):
        self.resetSteeringValues()
        if(self.lastKnown != None and self.videoSwitch > 0 and self.canSwitch):
            if(self.lastKnown.x > self.lastKnownLowerX and 
                    self.lastKnown.x < self.lastKnownUpperX and
                    self.lastKnown.y > self.lastKnownLowerY and 
                    self.lastKnown.y < self.lastKnownUpperY):                 
                self.bottomCameraCounter = 0
                self.lastKnown = None
                print "\tSwitching cameras"
                self.videoSwitch *= -1
                self.initImages()
                return 

        # keep turnin' untill we have more interesting information
        if (self.point == None):
            return 
        if(self.point.x == 0 or self.point.y == 0):
            print "\tFront Camera: no point found, keep on turnin'!"
            self.canSwitch = False
            self.canFly = False
            self.yaw = 0.3*self.turnValue
            self.hoverCounter = 0
            return 

        inSquare = True
        # bring the object to the centre of the screen
        # for x with rolling
        if (self.point.x < self.lowerX):
            print "\tFront Camera: point found: x is too much to the left!"
            self.yaw = -0.1
            self.turnValue = -1
            inSquare = False
        elif (self.point.x > self.upperX):
            print "\tFront Camera: point found: x is too much to the right!"
            self.yaw = 0.1
            self.turnValue = 1
            inSquare = False

        # for y with gaz
        if (not self.canFly and self.point.y < self.lowerY):
            print "\tFront Camera: point found: y is too low!"
            self.gaz = 0.5
            #there is a error backwards, so we have to go forward 
            #self.pitch = -0.05
            inSquare = False
        elif (not self.canFly and self.point.y > self.upperY):
            print "\tFront Camera: point found: y is too high"
            self.gaz = -0.2
            inSquare = False

        # otherwise, fly towards the target!
        if (inSquare and self.hoverCounter < self.hoverCounterMax):
            self.canFly = True
            self.hoverCounter += 1
            self.canSwitch = True
        elif(inSquare):
            print "\tFront Camera: flyin' towards the target!"
            self.pitch = -0.08
            self.canSwitch = True
            self.turnValue = -1
        
    
    def thinkAboutLastKnown(self):
        self.resetSteeringValues()
        # what we are going to do here:
        #   fly forward 
        #   check for point, and hoover above it
        #   if, for some time, the point was not found switch back to the
        #   front camera and start over

        if (self.point != None and (self.point.x != 0 or self.point.y != 0)):
            self.bottomCameraCounter = 0
            # adjusting for image if we found the object
            if (self.point.x < self.lowerX):
                print "\tBottom Camera: x is too much to the left!"
                self.turnValue = 0
                self.roll = -0.02
            elif (self.point.x > self.upperX):
                print "\tBottom Camera: x is too much to the right!"
                self.turnValue = 1
                self.roll = 0.02
            if (self.point.y < self.lowerY):
                print "\tBottom Camera: y is too low!"
                self.turnValue = 2
                self.pitch = -0.02
            elif (self.point.y > self.upperY):
                print "\tBottom Camera: y is too high!"
                self.turnValue = 3
                self.pitch = 0.02
        elif (self.bottomCameraCounter < self.bottomCameraMax):
            self.bottomCameraCounter += 1 
            if (self.turnValue == 0):
                self.roll = -0.02
            elif (self.turnValue == 1):
                self.roll = 0.02
            elif (self.turnValue == 2): 
                self.pitch = -0.02 
            elif (self.turnValue == 3):
                self.pitch = 0.02
            else:
                print "\tBottom Camera: no point found, keep on flyin'!"
                self.pitch = -0.02 
        else:
            # ahhh man, start over
            print "\tBottom Camera: Ahhh man!! We did not find the object :( :( "
            self.videoSwitch *= -1 
            self.initImages()
     
    
    # we need to find some way to call this baby when we stop
    def exit(self):
        print "\tpython exit called"
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
            print "\timage is null, skipping frame"
            return False 
        if (self.resultImage == None):
            print "\tresult image is null, skipping frame"
            return False 

         

        if (cvGetSize(self.image).height != cvGetSize(self.tempResultImage).height):
            print "\tincompatible image sizes in findPicture, skipping frame"
            return False 
    
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
        if (self.point.x != 0 or self.point.y != 0):
            self.lastKnown = self.point
        cvRectangle(self.matchImage, (self.lowerX, self.lowerY), (self.upperX, self.upperY), CV_RGB(255,0,0))
        if (self.videoSwitch > 0):
            cvRectangle(self.matchImage, (self.lastKnownLowerX, self.lastKnownLowerY), (self.lastKnownUpperX, self.lastKnownUpperY), CV_RGB(255, 0, 0))
        return True

    def setExitOnNextUpdate(self, event, x, y, flags, param):
        if (event == CV_EVENT_RBUTTONDOWN):
            print "\tPython recognizes exit command"
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
     i = 100;
     while (i < 800):
         uvhar.update([i, 0, 0, 0, 0, 0, 0, 0, 0, 0])
         time.sleep(0.0467)
         i = i + 1
     uvhar.exit()

         
