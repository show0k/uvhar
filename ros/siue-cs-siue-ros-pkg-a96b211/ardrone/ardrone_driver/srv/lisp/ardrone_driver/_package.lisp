(defpackage ardrone_driver-srv
  (:use cl
        roslisp-msg-protocol)
  (:export
   "TAKEOFF"
   "<TAKEOFF-REQUEST>"
   "<TAKEOFF-RESPONSE>"
   "RESET"
   "<RESET-REQUEST>"
   "<RESET-RESPONSE>"
   "LAND"
   "<LAND-REQUEST>"
   "<LAND-RESPONSE>"
  ))

