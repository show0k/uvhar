import spam;

class One:
     
     counter = 1

     def __init__(self):
         print "created one and counter is:"
         print self.counter
         print "setting counter to 2\n"
         self.counter = 2

     def printieprint(self):
         print "called printieprint and counter is: "
         print self.counter 
         print "now returning 819\n"
         return 819

     def argumentsMethod(self, argument):
         print "called argumentsMethod, counter is %d and argument is %s. Return 321\n" % (self.counter, argument)
         return 321


class two:
     def __init__(self):
         print "two"


def my_set_callback(number):
     print number
     return



def ghello():
     print "wtd?\n"
     return

def printnr(i):
     print i
     return i

#printnr(123)
#ghello();
