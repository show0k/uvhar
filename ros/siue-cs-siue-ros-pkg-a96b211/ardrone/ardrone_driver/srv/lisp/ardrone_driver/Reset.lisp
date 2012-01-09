; Auto-generated. Do not edit!


(in-package ardrone_driver-srv)


;//! \htmlinclude Reset-request.msg.html

(defclass <Reset-request> (ros-message)
  ()
)
(defmethod serialize ((msg <Reset-request>) ostream)
  "Serializes a message object of type '<Reset-request>"
)
(defmethod deserialize ((msg <Reset-request>) istream)
  "Deserializes a message object of type '<Reset-request>"
  msg
)
(defmethod ros-datatype ((msg (eql '<Reset-request>)))
  "Returns string type for a service object of type '<Reset-request>"
  "ardrone_driver/ResetRequest")
(defmethod md5sum ((type (eql '<Reset-request>)))
  "Returns md5sum for a message object of type '<Reset-request>"
  "6ccdb752b565be6d250259ff3d0c7ea1")
(defmethod message-definition ((type (eql '<Reset-request>)))
  "Returns full string definition for message of type '<Reset-request>"
  (format nil "~%"))
(defmethod serialization-length ((msg <Reset-request>))
  (+ 0
))
(defmethod ros-message-to-list ((msg <Reset-request>))
  "Converts a ROS message object to a list"
  (list '<Reset-request>
))
;//! \htmlinclude Reset-response.msg.html

(defclass <Reset-response> (ros-message)
  ((successful
    :reader successful-val
    :initarg :successful
    :type boolean
    :initform nil))
)
(defmethod serialize ((msg <Reset-response>) ostream)
  "Serializes a message object of type '<Reset-response>"
    (write-byte (ldb (byte 8 0) (if (slot-value msg 'successful) 1 0)) ostream)
)
(defmethod deserialize ((msg <Reset-response>) istream)
  "Deserializes a message object of type '<Reset-response>"
  (setf (slot-value msg 'successful) (not (zerop (read-byte istream))))
  msg
)
(defmethod ros-datatype ((msg (eql '<Reset-response>)))
  "Returns string type for a service object of type '<Reset-response>"
  "ardrone_driver/ResetResponse")
(defmethod md5sum ((type (eql '<Reset-response>)))
  "Returns md5sum for a message object of type '<Reset-response>"
  "6ccdb752b565be6d250259ff3d0c7ea1")
(defmethod message-definition ((type (eql '<Reset-response>)))
  "Returns full string definition for message of type '<Reset-response>"
  (format nil "bool successful~%~%~%"))
(defmethod serialization-length ((msg <Reset-response>))
  (+ 0
     1
))
(defmethod ros-message-to-list ((msg <Reset-response>))
  "Converts a ROS message object to a list"
  (list '<Reset-response>
    (cons ':successful (successful-val msg))
))
(defmethod service-request-type ((msg (eql 'Reset)))
  '<Reset-request>)
(defmethod service-response-type ((msg (eql 'Reset)))
  '<Reset-response>)
(defmethod ros-datatype ((msg (eql 'Reset)))
  "Returns string type for a service object of type '<Reset>"
  "ardrone_driver/Reset")
