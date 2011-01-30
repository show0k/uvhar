#! /usr/bin/env python
##
## This program is a quick attempt to track the dot from a red laser pointer.
## Right now, the code performs steps 1-5 below, and shows the results in several windows. 
##
## General Idea:
## 1. Grab the video frame.
## 2. Convert it to HSV
## 3. Split the frame into individual components (separate images for H, S, and V)
## 4. Apply a threshold to each compenent (hopefully keeping just the dot from the laser)
## 5. AND the 3 images (which "should" cut down on false positives)
## TODO: 
## 6. Identify the "best" location for the identified laser. ie: x,y coordinates in the image.
## 7. Do something interesting with it...
##    (draw a circle around it, control the mouse cursor, build a game, etc).
##
## Author: Brad Montgomery (www.bradmontgomery.net)
## Date: 2008-01-23
#############################################################################
import sys
from opencv import cv
from opencv import highgui

#############################################################################
# definition of some constants
cam_width = 640
cam_height = 480

# HSV color space Threshold values for a RED laser pointer 
# hue
hmin = 5 
hmax = 6 # hmax = 180
# saturation
smin = 50
smax = 100
# value
vmin = 250
vmax = 256

#############################################################################

def create_and_position_window(name, xpos, ypos):
    ''' a function to created a named widow (from name), 
        and place it on the screen at (xpos, ypos) '''
    highgui.cvNamedWindow(name, highgui.CV_WINDOW_AUTOSIZE) # create the window
    highgui.cvResizeWindow(name, cam_width, cam_height) # resize it
    highgui.cvMoveWindow(name, xpos, ypos) # move it to (xpos,ypos) on the screen

def setup_camera_capture(device_num=0):
    ''' perform camera setup for the device number (default device = 0) i
        return a reference to the camera Capture
    '''
    try:
        # try to get the device number from the command line
        device = int(device_num)
    except (IndexError, ValueError):
        # no device number on the command line, assume we want the 1st device
        device = 0
    print 'Using Camera device %d'%device

    # no argument on the command line, try to use the camera
    capture = highgui.cvCreateCameraCapture (device)

    # set the wanted image size from the camera
    highgui.cvSetCaptureProperty (capture,highgui.CV_CAP_PROP_FRAME_WIDTH, cam_width)
    highgui.cvSetCaptureProperty (capture,highgui.CV_CAP_PROP_FRAME_HEIGHT, cam_height)

    # check that capture device is OK
    if not capture:
        print "Error opening capture device"
        sys.exit (1)
    
    return capture    

# so, here is the main part of the program
def main():

    print "OpenCV version: %s (%d, %d, %d)" % (cv.CV_VERSION,
                                               cv.CV_MAJOR_VERSION,
                                               cv.CV_MINOR_VERSION,
                                               cv.CV_SUBMINOR_VERSION)

    # create windows 
    create_and_position_window('Thresholded_HSV_Image', 10, 10)
    create_and_position_window('RGB_VideoFrame', 10+cam_width, 10)

    create_and_position_window('Hue', 10, 10+cam_height)
    create_and_position_window('Saturation', 210, 10+cam_height)
    create_and_position_window('Value', 410, 10+cam_height)
    create_and_position_window('LaserPointer', 0,0)

    capture = setup_camera_capture()

    # create images for the different channels
    h_img = cv.cvCreateImage (cv.cvSize (cam_width,cam_height), 8, 1)
    s_img = cv.cvCreateImage (cv.cvSize (cam_width,cam_height), 8, 1)
    v_img = cv.cvCreateImage (cv.cvSize (cam_width,cam_height), 8, 1)
    laser_img = cv.cvCreateImage (cv.cvSize (cam_width,cam_height), 8, 1)
    cv.cvSetZero(h_img)
    cv.cvSetZero(s_img)
    cv.cvSetZero(v_img)
    cv.cvSetZero(laser_img)

    while True: 
        # 1. capture the current image
        frame = highgui.cvQueryFrame (capture)
        if frame is None:
            # no image captured... end the processing
            break

        hsv_image = cv.cvCloneImage(frame) # temporary copy of the frame
        cv.cvCvtColor(frame, hsv_image, cv.CV_BGR2HSV) # convert to HSV

        # split the video frame into color channels
        cv.cvSplit(hsv_image, h_img, s_img, v_img, None)

        # Threshold ranges of HSV components.
        cv.cvInRangeS(h_img, hmin, hmax, h_img)
        cv.cvInRangeS(s_img, smin, smax, s_img)
        cv.cvInRangeS(v_img, vmin, vmax, v_img)

        # Perform an AND on HSV components to identify the laser!
        cv.cvAnd(h_img, v_img, laser_img)
        # This actually Worked OK for me without using Saturation.
        #cv.cvAnd(laser_img, s_img,laser_img) 

        # Merge the HSV components back together.
        cv.cvMerge(h_img, s_img, v_img, None, hsv_image)

        #-----------------------------------------------------
        # NOTE: default color space in OpenCV is BGR!!
        # we can now display the images 
        highgui.cvShowImage ('Thresholded_HSV_Image', hsv_image)
        highgui.cvShowImage ('RGB_VideoFrame', frame)
        highgui.cvShowImage ('Hue', h_img)
        highgui.cvShowImage ('Saturation', s_img)
        highgui.cvShowImage ('Value', v_img)
        highgui.cvShowImage('LaserPointer', laser_img)

        # handle events
        k = highgui.cvWaitKey (10)

        if k == '\x1b' or k == 'q':
            # user has press the ESC key, so exit
            break


if __name__ == '__main__':
    main()
