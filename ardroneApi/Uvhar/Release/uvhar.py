import Image
from opencv import cv

def init():
     counter = -1
     image = None

"""
def update(newCounter):
     if (counter != newCounter):
         counter = newCounter
         loadNewImage()
"""

def update(counter):
     print counter
     return 1337
      
    
def loadNewImage():
     if (image != None):
         image.close()
     filename = "frame" + counter + ".jpg"
     image = Image.open(filename)
     image.show();
 


#image = cv.LoadImageM("images/frame00001.jpg")

if __name__ == "__main__":
     init()
     update(00001)
