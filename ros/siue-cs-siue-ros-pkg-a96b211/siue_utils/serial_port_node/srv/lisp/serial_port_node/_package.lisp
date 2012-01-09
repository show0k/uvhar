(defpackage serial_port_node-srv
  (:use cl
        roslisp-msg-protocol)
  (:export
   "SERIALWRITE"
   "<SERIALWRITE-REQUEST>"
   "<SERIALWRITE-RESPONSE>"
   "SERIALREAD"
   "<SERIALREAD-REQUEST>"
   "<SERIALREAD-RESPONSE>"
   "SERIALDATA"
   "<SERIALDATA-REQUEST>"
   "<SERIALDATA-RESPONSE>"
  ))

