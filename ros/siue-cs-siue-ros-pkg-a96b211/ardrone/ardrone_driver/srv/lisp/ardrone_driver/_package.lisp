(defpackage ardrone_driver-srv
  (:use cl
        roslisp-msg-protocol)
  (:export
   "LAND"
   "<LAND-REQUEST>"
   "<LAND-RESPONSE>"
   "TAKEOFF"
   "<TAKEOFF-REQUEST>"
   "<TAKEOFF-RESPONSE>"
   "RESET"
   "<RESET-REQUEST>"
   "<RESET-RESPONSE>"
  ))

