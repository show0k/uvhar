# Log Page (logbook) #
Our experiences on a day to day basis.

## Day 16, 25-01-11 ##
OpenCV is really nasty. It misses a lot of functions that you would expect (and that we knew and loved in MatLab in our 2nd year's project). Now we need to copy an image in another image and the way to do this is to define a region of interest (ROI). But, we can not... Why?
cvSetImageROI(image, rectangle), is not defined and nobody knows why.

## Day 15, 24-01-11 ##
Today we hooked up the hook on the AR Drone. Unfortunately it made it stop taking off some times, but on other times it flew without problems.

Unfortunately the AR Drone was not able to fly over and pick up the object yet! Now we are trying to make the AR Drone recognize how far the object is. We will do this by counting the ammount of pixels our found object takes in and then, using a formula, calculate our distance.

We used zunzun.com to calculate the right formula from the following data points:

| **Distance (m)**   |  **# Pixels**|
|:-------------------|:-------------|
|0.25                |   4802       |
|0.5                 |   1495       |
|0.75                |   753        |
|1                   |   402        |
|1.25                |   273        |
|1.5                 |   196        |
|2                   |   45         |
|2.5                 |    36        |

Now let's hope this works! We will also try to hook up the hook in another way, so that the AR Drone will have no trouble taking off. At least not more than usual.

The first function we got was: 7.6315999175901217E-01 **exp(-6.4523069253280248E-04x) + 8.3189620080930808E+01** exp(-1.5517143254934185E-01x) + 1.4487053946697799E+00 **exp(-4.6478934298555246E-03x), {x, 0, 6000},{y,0,2}]**

We also have a picture:

![http://uvhar.googlecode.com/hg/distance.png](http://uvhar.googlecode.com/hg/distance.png)

It works extremely well.


## Day 14, 23-01-11 ##
We put all kinds of random things on the floor so the drone has a better chance of hovering steadily (and not flying to all kinds of random corners of the room). It seems to be working better now, which allows us to continue with working on the REAL thing!

We were able to make the drone find the object, fly to the object and hover above it. here is a nice screenshot of that.

![http://uvhar.googlecode.com/hg/topView.png](http://uvhar.googlecode.com/hg/topView.png)



## Day 13, 21-01-11 ##
We forgot to log yesterday, but we basically made our object and had the drone fly towards it. It worked pretty okay but mostly luck. Today we are fine tuning parameters and trying things to fix the ginormous flying errors in the drone. Also added pictures to this page of our graphical output.

Sometimes the drone does exactly as our program tells it to act. It's quite nice to see it fly directly towards the object. Other times however, the error makes it do crazy things! We still have to find a way to cope with this.

![http://uvhar.googlecode.com/hg/objectHerkenning.jpg](http://uvhar.googlecode.com/hg/objectHerkenning.jpg)
The drone's view | after filtering | after template matching

## Day 11, 19-01-11 ##
Today we continued what we started. We found out that it would be better to use histogram backprojection to locate our object, than just using color recognition. We are going to use the OpenCV modules for this. Then we can use cvMinMaxLoc to locate the object. The difference will also be that this is a more sophisticated method for locating the object. The downside is that we will be replacing the color recognition code we made a couple of days ago.

We implemented a template matching algotithm on our own color recognition. So now we get an image in which we can find the centre of our object (for now: a pink piece of paper). This appears to work pretty good, because it ignores the pink ball, which is also in the room.
Then we continued on letting the drone move to the object. It now moves itself to have the object in a user specified square on its captured image. Tomorrow we will make the actual object.

![http://uvhar.googlecode.com/hg/blaadjeHerkenning.png](http://uvhar.googlecode.com/hg/blaadjeHerkenning.png)
The drone's view | after filtering | after template matching

## Day 10, 18-01-11 ##
Today we started working on being able to fly to our subject. We also took an extra look at the segfault, but were unsuccesful in solving it.
We are now able to transfer navigation data to python and return flying instructions. We started investigating on how to find the pixels where we know there is something pink. This is because the only thing we have right now, is that we filter our object from the image. Unfortunately this does not mean that the computer instantly knows where these white pixels are.


## Day 9, 17-01-11 ##
This morning we were able to recognize a specified color by using its HSV values. The recognition, for now, returns a black image with a white area where it finds the color. This has almost no error with our pink ball.

We also were able to do this in real time.
Then we tried to fix our segmentation fault and we wanted to be able to exit when we click the window. Unfortunately we don't know where the segmentation fault is yet.

## Day 8, 13-01-11 ##
We are able to correctly send and receive all the information from c to python and back. We should be looking on how to call the signal\_exit, to close and shut down everything correctly when we are done flying (We will probably have to use the gamepad.c again). BUT we have actually started to recognize things with openCV! It is all very exciting.

## Day 7, 12-01-11 ##
We were able to save the images and initialize python and call pythons functions. We are now going to install opencv and start recognizing things.

After that we were able to open the image with python and show some sort of movie from several images. The problem now, is combining this with our C code, so that Python can recognize what we want to recognize.

Later on in the day we found out that we could not capture images from the camera anymore. Trying anything we spent a lot of time on something that turned out to be fixed by rebooting. Tomorrow we will work on combining C with our new Python code, which now involves loading a python class.

## Day 6, 11-01-11 ##
We continued to try and call a python function from C, which shouldn't be that hard but certainly is (for us).

We found out how we could compile c that uses python, no questions asked: http://bytes.com/topic/python/answers/45199-problem-embedding-python , undefined reference to `Py*something`. I'm logging this to help others find the same solution we did in less time.

## Day 5, 10-01-11 ##
We continued to look at the video processing and thought it would be a wise idea to just call a python function with what we thought was the received picture. We did not have a lot of time this day so we couldn't get a lot done but we will catch up later this week.

## Day 4, 07-01-11 ##
We started to understand the framework of the API and programmed a little script to have the robot to take of, rotate a circle, and land and repeat. AND IT WORKED!!!

We now have navdata printing to the console.

We started on getting the video image from the camera. The Navigation example has a video feed in it, so hopefully we can use that to our advantage.

We were able to successfully build and use this feature and fly around watching our environment using the joystick.

## Day 3, 06-01-11 ##
Today we found out why the Ar Drone was not hovering correctly. It uses the camera to stay on the right position. The completely white floor (without any texture) of the experience lab prevents this. When we start the ar drone on the box it hovers better.

We analyzed the structure of the AT commands and could not figure out why the example java scripted worked at all because the drone's documentation says the drone considers a connection lost when it does not receive a command every 2 seconds or so. The java script did not send commands that often and yet the drone did not consider the connection lost (it should land automagically by itself). The other odd thing was that the commands to yaw and gaz were executed but the exact same commands for pitch and roll were not. So we tried to get the gamepad example working.

The gamepad example was compiled, a joystick was connected to the computer, installed and controlled the ar drone. It flew around quite nicely. This gave us confidence to start working with the API but we do not know how to compile our own program yet. We will find this out tonight after guitar lessons, meetings and dinner.

We have made our own copy of the example code, to edit for our own program. This compiles now. Time for diner!

## Day 2, 05-01-11 ##
We went on trying to connect to the Ar Drone. To test our connection we downloaded a simple java app (https://projects.ardrone.org/attachments/197/ARDrone.java) .
We resetted the Ar Drone and connected to it. Now it was possible to use the app to rise to the sky! Unfortunately we were not able to move around using the app. This could be because of the app itself or because our understanding of the app. We went on to try to use the ROS driver to take off.

We fixed the ros driver to work on both computers. There was a problem with the CMakeLog.txt file which I had from the other laptop. Now everything will build and work on both computers.

After that we were able to connect to the Ar Drone and use ardrone/takeoff to make the ArDrone take off. There was one problem: The takeoff took very long to initiate. We don't know why.

Next thing to try was to publish our own commands to the cmd\_vel topic of the ar drone.

Unfortunately this did not seem to work as well. We were starting to wonder why we should use Ros at all if such a simple script worked better than the weird c++ code to control the drone using Ros.

Now we are going to use the API for the drone itself to go and leave ros for what is it for now. If it does not work we are in big trouble. If it works okay we can continue to use that until we run into something for which Ros can be extra ordinary handy or is already made by someone else.

There is a problem with the Example code in the api: It won't compile. Due to many errors we are spending some time on letting it compile. When finally were able to compile we needed a (game) controller to operate the drone. We did not have this. But luckily we did found out a little bit more about AT commands and how they work. We were also able to change the height of the drone and rotate it left and right and in another program to blink with the leds in specific colors and frequencies.

Perhaps the most important thing; we found the SDK 1.5 ar drone guide. This will help us a great deal to figure out what we should be doing. It just leaves us wondering why it isn't just out there.. on the front page...

## Day 1, 04-01-11 ##
In the morning we followed a lecture on introduction to robotics by Arnoud Visser. Afterwards we continued with ROS tutorials. After doing the tutorials we decided to start with connecting to the Ar Drone. To do this we downloaded the ArDrone ROS driver and installed the ardrone stack (http://github.com/siue-cs/siue-ros-pkg/tree/experimental-ardrone). After a while we figured out the structure and what should be run to do what... The only problem was that it did not do anything.

The program told us it had connected and that commands were executed properly. However, the connection time was way to short and the commands were not actually executed. Using telnet we checked out information using netstat -a. We checked out the seriously weird c++ ros source. We looked online and found other open source scripts that allowed us to connect but in the end we did not got it to work... today.

## Day 0, 03-01-11 ##
We compiled a list of all the things we should figure out and do during the 4 weeks we have to accomplish our goal. We then thought which of these things we were going to do in which of the four weeks. The result can be seen in the [planning](http://code.google.com/p/uvhar/source/browse/planning/planning.pdf?r=4b0a7c00c22334a1a1b6c2dafa9188601fadfa97). After this we started by downloading and installing ROS and following the first couple of tutorials.