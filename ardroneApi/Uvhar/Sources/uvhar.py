import Image, ImageDraw, time
import sys, math
from opencv.cv import *
from opencv.highgui import *



# BUG
# - When the object is lost the drone should go to the height on which it was last seen

# TODO
# - keep a log with the commands from c, so the replay shows the data more
# correctly
# - remove self.imageHeight and self.imageWidth


class Uvhar:


    exitOnNextUpdate = False

    #loggin' output
    logFile = None

    # keeping track of the image we use
    counter = -1 
    # Counter to hover before we fly towards target
    hoverCounter = 0
    hoverCounterMax = 10
    # time granted to seek the object with the bottom camera
    bottomCameraCounter = 0
    bottomCameraMax = 100
    # time after take off before we are start sending commands
    takeOffTime = 100

    # indicator of what camera we are using [..bottom camera, 0, front camera.. ]
    videoMode = 1
    # so we can detect changes to send them to c
    videoSwitch = 0
    # we first want to make sure the object is in the centre, and only then
    # then fly towards it
    # it is also a fly made out of a can
    # currently not used, deprecated
    canFly = False
    
    # the information that c supplies us with
    cTuple = None
    # the point of the object in the last image
    point = None
    # the amount of pixels in the last frames that represented the object
    objectPixels = -1

    # steering values
    roll = 0
    pitch = 0
    gaz = 0
    yaw = 0
    turnValue = 1
    bottomTurnValueX = 0
    bottomTurnValueY = 0
    # a window that we want the target in our image to be in. These 
    # are defined in initImages
    lowerX = 0
    upperX = 0
    lowerY = 0 
    upperY = 0 # Higher in the coordinate system, but lower in the image!

    """"
    # Defining a square where we can lose the object before we start 
    # hoovering above it
    lastKnownLowerX = 100
    lastKnownUpperX = 210
    lastKnownLowerY = 210
    lastKnownUpperY = 240
    """
   
    logLines = None 
    
    # Images 
    image = None
    imageH = None
    imageS = None
    imageV = None
    tempResultImage = None
    resultImage = None
    targetImage = None
    matchImage = None
    onesImage = None

    # width and height of the image
    imageWidth = 320
    imageHeight = 240

    # bounding hsv values to spot something pink
    hMin = 313/2
    hMax = 347/2
    sMin = 40/0.393
    sMax = 101/0.393
    vMin = 40/0.393
    vMax = 101/0.393 

    # the height we want to fly at (when looking for the object etc)
    preferredHeight = 400
    foundHeight = -1
    distanceToObject = -1
    # when we are flying towards the target we want to increase altitude
    # to have a bigger chance of finding the object
    deltaHeightWhenFound = 160
    # is the target in the square we want it to be in?
    inSquare = False
    
    # window names
    mainWindowName = "Image"
    processedWindowName = "Processed Image"
    processedWindowName2 = "Processed Image 2"


    #files
    logFile = None
    dataFile = None
    # file names
    logFileName = "log.txt"
    dataFileName = "data.txt"
    # for reading back
    dataCounter = 0
    dataLines = None


    def __init__(self):
        print "Uvhar class contructor called."

        if (__name__ != "__main__"):
           self.logFile = open(self.logFileName, "w")
           self.dataFile = open(self.dataFileName, "w")
        else:
            self.dataFile = open(self.dataFileName, "r")
            self.dataLines = self.dataFile.readlines()

    

        self.initImages()
       
        #cvShowImage("targetImage", self.targetImage)
        #cvWaitKey(0)

        # already open the windows :O
        cvNamedWindow(self.mainWindowName) # if window does not resize automagically add CV_WINDOW_AUTOSIZE
        cvNamedWindow(self.processedWindowName)
        cvNamedWindow(self.processedWindowName2)
        cvMoveWindow(self.mainWindowName, 0, 50)
        cvMoveWindow(self.processedWindowName, 360, 50)
        cvMoveWindow(self.processedWindowName2, 700, 50)
        cvSetMouseCallback(self.mainWindowName, self.setExitOnNextUpdate, None)
        cvSetMouseCallback(self.processedWindowName, self.setExitOnNextUpdate, None)
        cvSetMouseCallback(self.processedWindowName2, self.setExitOnNextUpdate, None)
        cvWaitKey(1)

    def initImages(self):
        self.log("\tCalling initImages")
        # setting to the right size 
        if (self.image == None):
            # creatin' an empty image because self.image cant be None
            self.image = cvCreateImage(cvSize(320, 240), 8, 3)
            self.videoMode = 1

        if (cvGetSize(self.image).height == 144):
            #self.videoMode = -1
            self.lowerX = 58
            self.upperX = 108
            self.lowerY = 42
            self.upperY = 92
        else: 
            #self.videoMode = 1
            self.lowerX = 135
            self.upperX = 170
            self.lowerY = 130  
            self.upperY = 160 
        # creating the images
        self.imageH = cvCreateImage(cvGetSize(self.image), 8, 1)
        self.imageS = cvCreateImage(cvGetSize(self.image), 8, 1)
        self.imageV = cvCreateImage(cvGetSize(self.image), 8, 1)
        self.resultImage= cvCreateImage(cvGetSize(self.image), 8, 1)
        self.tempResultImage = cvCreateImage(cvGetSize(self.image), 8, 3)
        self.finalMatchImage = cvCreateImage(cvGetSize(self.image), 32, 1)
        #self.finalMatchImage = cvCreateImage(cvGetSize(self.image), 8, 1)

    def createMatchImage(self):
        # size of the target image used to template match
        cmpw = int(100 - abs(90 * self.distanceToObject))
        if (cmpw < 10):
            cmpw = 10
        if (cmpw > 100):
            cmpw = 100
        cmph = int(100 - abs(90 * self.distanceToObject))
        if (cmph < 10):
            cmph = 10
        if (cmph > 100):
            cmph = 100
            
        imageWidth = cvGetSize(self.image).width
        imageHeight = cvGetSize(self.image).height

        self.matchImage = cvCreateImage(cvSize(imageWidth - cmpw + 1, imageHeight - cmph+ 1), 32, 1)
        
        self.onesImage = cvCreateImage(cvGetSize(self.image), 32, 1)

        self.targetImage = cvCreateImage(cvSize(cmpw, cmph), 8, 1)
        cvSet(self.targetImage, CV_RGB(255, 255, 255))


    def logData(self, cTuple):
       for i in range(0, 9):
           self.dataFile.write("%f\n" % cTuple[i])

    def getLoggedData(self):
        cTuple = [0, ]*9 # tuple isn't actually tuple, but rather a list
        for i in range(0, 9):
            cTuple[i] = eval(self.dataLines[self.dataCounter])
            self.dataCounter += 1
        return cTuple   
                

    def update(self, cTuple):
        if (__name__ != "__main__"):
            self.logData(cTuple)
        else:
            cTuple = self.getLoggedData()

        #self.log("altitude: %4.2f, battery level: %4.2f" % (cTuple[5], cTuple[1]))
        #self.log("vx, %4.2f, vy, %4.2f" % (cTuple[6], cTuple[7]))

        self.cTuple = cTuple
        # take some time to fully take off before we start doin' things
        if (cTuple[0]>self.takeOffTime):
            #self.log("new counter received %d" % newCounter
            self.counter = cTuple[0] - 1
            self.loadNewImage()
            if (self.findPicture()):
                #check if the info in self.point is useful and acts on it
                if (self.videoMode > 0):
                    self.thinkAboutFrontCamera()
                else:
                    self.thinkAboutBottomCamera()

        elif(self.counter != cTuple[0] - 1):
            self.counter = cTuple[0] - 1
            self.loadNewImage()
            self.findPicture()
            
        #self.log("navdata: battery level: %4.2f, theta: %4.2f, phi: %4.2f, psi %4.2f, altitude %4.2f, vx %4.2f, vy %4.2f, vz %4.2f" % (cTuple[1], cTuple[2], cTuple[3], cTuple[4], cTuple[5], cTuple[6], cTuple[7], cTuple[8])  )

        cvShowImage(self.mainWindowName, self.image)
        cvShowImage(self.processedWindowName, self.resultImage)
        cvShowImage(self.processedWindowName2, self.finalMatchImage)
        if(self.finalMatchImage != None):
            self.log("finalMatchImage width: %d, finalMatchImage height: %d" % (cvGetSize(self.finalMatchImage).width, cvGetSize(self.finalMatchImage).height))

        

        cvWaitKey(4)
        returnArray = [self.exitOnNextUpdate, self.roll, self.pitch, self.gaz, self.yaw, self.videoSwitch == 1]
        if (self.videoSwitch == 1):
            self.videoMode *= -1
        self.videoSwitch = 0
        return returnArray

    def resetSteeringValues(self):
        self.roll = 0
        self.pitch = 0
        self.gaz = 0
        self.yaw = 0

    def setPreferredHeight(self, newHeight):
        self.preferredHeight = newHeight
        self.log("\tNew preferred height: %d" % self.preferredHeight)

    def thinkAboutFrontCamera(self):
        self.resetSteeringValues()

        # if we have hovered we know we can switch to the bottom camera
        if (self.hoverCounter > self.hoverCounterMax):
            self.hoverCounter = 0
            self.bottomCameraCounter = 0
            self.log("\tSwitching cameras")
            self.videoSwitch = 1
            self.bottomTurnValueX = 0
            self.bottomTurnValueY = 0
            self.foundHeight = self.cTuple[5]
            self.setPreferredHeight(self.foundHeight + self.deltaHeightWhenFound)
            return 
           
        self.changeBoundries() 

        # keep turnin' untill we have more interesting information
        if (self.point == None):
            return 
        if(self.point.x == 0 or self.point.y == 0):
            self.log("\tFront Camera: no point found, keep on turnin'!")
            self.yaw = 0.3*self.turnValue
            self.hoverCounter = 0
            self.goToPreferredHeight()
            return 



        # assume we are focused in the square (centre of the screen)
        #and change it if we later find out we aren't
        self.inSquare = True

        # bring the object to the centre of the screen if it isn't already
        # for x with rolling
        self.yaw = -0.0015 * (155 - self.point.x)

        if (self.point.x < self.lowerX):
            self.log("\tFront Camera: point found: x is too much to the left!")
            if (self.yaw > -0.05):
                self.yaw = -0.05
            self.turnValue = -1
            self.inSquare = False
        elif (self.point.x > self.upperX):
            self.log("\tFront Camera: point found: x is too much to the right!")
            if (self.yaw < 0.05):
                self.yaw = 0.05
            self.turnValue = 1
            self.inSquare = False

        # for y with gaz
        if (self.point.y < self.lowerY):
            self.log("\tFront Camera: point found: y is too low!")
            self.gaz = 0.15 * self.distanceToObject
            if(self.gaz > 0.4):
                self.gaz = 0.4
        elif (self.point.y > self.upperY):
            self.log("\tFront Camera: point found: y is too high")
            self.gaz = -0.075 * self.distanceToObject
            if(self.gaz < 0.3):
                self.gaz = -0.3
        else:
            self.foundHeight = self.cTuple[5]
            self.setPreferredHeight(self.foundHeight)
            self.log("\tFound height set in prefferedHeight with value: %d" % self.foundHeight)

        # checkin' if we have been hoovering with the target in the square
        # if this happened we can continue with flying forward and ignore
        # the y axes of the square (canFly is actually, ignore y axes)
        if (self.inSquare):
            if (self.distanceToObject > 0 and self.distanceToObject < 0.3 and self.inSquare):
                self.hoverCounter += 1
            else:
                self.log("\tFront Camera: flyin' towards the target!")
                self.pitch = -0.08 * self.distanceToObject 
                if(self.pitch < -0.08):
                    self.pitch = -0.08
        else:
            self.hoverCounter = 0

    def changeBoundries(self):
        difference = 40 - 40 * self.distanceToObject
        self.lowerX = 135 - difference
        if (self.lowerX > 135):
            self.lowerX = 135
        if (self.lowerX < 100):
            self.lowerX = 100
        self.upperX = 170 + difference
        if (self.upperX < 170):
            self.upperX = 170
        if (self.upperX > 205):
            self.upperX = 205
        self.lowerY = 130 - difference
        if (self.lowerY > 130):
            self.lowerY = 130
        if (self.lowerY < 95):
            self.lowerY = 95
        self.upperY = 160 + difference
        if (self.upperY < 160):
            self.upperY = 160
        if (self.upperY > 195):
            self.upperY = 195
    
    def thinkAboutBottomCamera(self):
        self.resetSteeringValues()
        self.setPreferredHeight(self.foundHeight + self.deltaHeightWhenFound)
        self.goToPreferredHeight()

        if (self.point != None and self.point.x != 0):
            # Not needed, i think
            # self.bottomCameraCounter = 0
            # adjusting for image if we found the object
            if (self.point.x < self.lowerX):
                self.log("\tBottom Camera: x is too much to the left!")
                self.bottomTurnValueX = -1
                self.roll = -0.013
            elif (self.point.x > self.upperX):
                self.log("\tBottom Camera: x is too much to the right!")
                self.bottomTurnValueX = 1
                self.roll = 0.013
            """
            if (self.point.y < self.lowerY):
                self.log("\tBottom Camera: y is too low!")
                self.bottomTurnValueY = -1
                self.pitch = -0.013
            elif (self.point.y > self.upperY):
                self.log("\tBottom Camera: y is too high!")
                self.bottomTurnValueY = 1
                self.pitch = 0.013
            elif (self.point.x < self.lowerX and self.point.x > self.upperX):
                self.resetSteeringValues()
                self.gaz = 0
            """
        if (self.bottomCameraCounter < self.bottomCameraMax):
            self.bottomCameraCounter += 1 
            self.roll = self.cTuple[7]/-7000.
            if(self.bottomTurnValueX * self.cTuple[7] < 0):
                if (self.roll > 0.05):
                    self.roll = 0.05
                elif (self.roll < -0.05):
                    self.roll = -0.05
            elif(self.roll > 0):
                self.roll = 0.05 - self.roll
            elif(self.roll < 0):
                self.roll = -0.05 + self.roll
            """
            self.pitch = self.cTuple[6]/7000.          
            if(self.bottomTurnValueY * self.cTuple[6] < 0):
                if (self.pitch > 0.05):
                    self.pitch = 0.05
                elif (self.pitch < -0.05):
                    self.pitch = -0.05
            if (self.pitch > 0):
                self.pitch = 0.05 - self.pitch
            elif (self.pitch < -0):
                self.pitch = -0.05 + self.pitch
            """
            #if (self.bottomTurnValueX == 0 and self.bottomTurnValueY == 0):
            self.log("\tBottom Camera: keep on flyin'!")
            self.pitch = -0.08
        else:
            # ahhh man, start over
            self.log("\tBottom Camera: Ahhh man!! We did not find the object :( :( ")
            self.videoSwitch = 1
            self.setPreferredHeight(self.foundHeight)


  
    # loads image with an image from file using the counter, processes it and 
    # shows the result 
    def loadNewImage(self):
        filename = "images/frame%05d.jpg" % (self.counter)
        #self.log(filename
        self.image = cvLoadImage(filename, CV_LOAD_IMAGE_COLOR)

    def goToPreferredHeight(self):
        if (self.cTuple[5] == 0):
            return
        dif = (1/600.) * (self.preferredHeight-self.cTuple[5])
        if (dif > 0.5):
            dif = 0.5
        elif (dif < -0.5):
            dif = -0.5
        self.log("\tGoing from %4.2f to %d with %4.2f" % (self.cTuple[5], self.preferredHeight, dif))
        self.gaz = dif
            

    # processes image and sets the result in resultImage
    # also returns a point that contains the found  
    def findPicture(self):
        if (self.image == None):
            self.log("\timage is null, skipping frame")
            return False 
        if (self.resultImage == None):
            self.log("\tresult image is null, skipping frame")
            return False 

        if (cvGetSize(self.image).height == 144):
            self.image = cvGetSubRect(self.image, cvRect(0, 0, 176, 144))

        if (cvGetSize(self.image).height != cvGetSize(self.tempResultImage).height):
            self.initImages()

   
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
        self.createMatchImage()
        cvMatchTemplate(self.resultImage, self.targetImage, self.matchImage, CV_TM_SQDIFF_NORMED) 
        if(self.matchImage != None):
            offset = cvPoint((cvGetSize(self.finalMatchImage).width - cvGetSize(self.matchImage).width)/2, (cvGetSize(self.finalMatchImage).height - cvGetSize(self.matchImage).height)/2)
            cvCopyMakeBorder(self.matchImage, self.finalMatchImage, offset, IPL_BORDER_CONSTANT, 0)
            _, _, self.point, _ = cvMinMaxLoc(self.finalMatchImage)

        cvSub(self.onesImage, self.finalMatchImage, self.finalMatchImage)

        # counting the pixels we have discovered to calculate the distance
        self.objectPixels = cvCountNonZero(self.finalMatchImage)
        if (self.objectPixels > 0):
            self.distanceToObject = self.distanceFromPixels(self.objectPixels) 
        else:
            print "\tToo few pixels to calculate a distance"
            self.distanceToObject = -1
       
        print "\tobject pixels: %d, guessed distance: %f" % (self.objectPixels, self.distanceToObject)

        #cvRectangle(self.matchImage, (self.lowerX, self.lowerY), (self.upperX, self.upperY), CV_RGB(1,1,1))
        return True

    def setExitOnNextUpdate(self, event, x, y, flags, param):
        if (event == CV_EVENT_RBUTTONDOWN):
            self.log("\tPython recognizes exit command")
            self.exitOnNextUpdate = True
            if __name__ == "__main__":
                self.exit()

    def log(self, string):
        if (__name__ != "__main__"):
            self.logFile.write("%s\n" % string)
        print string

    

    # the function from zunzun.com to calculate the
    # distance given a number of pixels
    def distanceFromPixels(self, x_in):
       temp = 0.0

       # coefficients
       a = 7.6315999175901217E-01
       b = -6.4523069253280248E-04
       c = 8.3189620080930808E+01
       d = -1.5517143254934185E-01
       f = 1.4487053946697799E+00
       g = -4.6478934298555246E-03
       Offset = 2.1482272389707244E-01

       temp = a * math.exp(b*x_in) + c * math.exp(d*x_in) + f * math.exp(g*x_in)
       temp = temp + Offset
       return temp

    
    # we need to find some way to call this baby when we stop
    def exit(self):
        self.log("\tpython exit called")
        self.log(self.counter)
        cvDestroyAllWindows()
        if __name__ == "__main__":
            exit()
        else:
            self.logFile.close()


#image = cv.LoadImageM("images/frame00001.jpg")

if __name__ == "__main__":

     logFile = file("log.txt", "r")
     """
     try:
        maxCounter = eval(logFile.readlines()[- 1])
     except IndexError:
        print "no counter read from file\n"
        maxCounter = 800 
     logFile.close()
     print "\tMax counter of last run: %s " % maxCounter
     """
     maxCounter = 800
     uvhar = Uvhar()
     i = 100;
     while (i < maxCounter):
         uvhar.update([i, 0, 0, 0, 0, 0, 0, 0, 0, 0])
         #time.sleep(0.0467)
         time.sleep(0.04)
         i = i + 1
     uvhar.exit()

