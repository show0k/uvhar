; Auto-generated. Do not edit!


(in-package ardrone_driver-srv)


;//! \htmlinclude Takeoff-request.msg.html

(defclass <Takeoff-request> (ros-message)
  ()
)
(defmethod serialize ((msg <Takeoff-request>) ostream)
  "Serializes a message object of type '<Takeoff-request>"
)
(defmethod deserialize ((msg <Takeoff-request>) istream)
  "Deserializes a message object of type '<Takeoff-request>"
  msg
)
(defmethod ros-datatype ((msg (eql '<Takeoff-request>)))
  "Returns string type for a service object of type '<Takeoff-request>"
  "ardrone_driver/TakeoffRequest")
(defmethod md5sum ((type (eql '<Takeoff-request>)))
  "Returns md5sum for a message object of type '<Takeoff-request>"
  "6ccdb752b565be6d250259ff3d0c7ea1")
(defmethod message-definition ((type (eql '<Takeoff-request>)))
  "Returns full string definition for message of type '<Takeoff-request>"
  (format nil "~%"))
(defmethod serialization-length ((msg <Takeoff-request>))
  (+ 0
))
(defmethod ros-message-to-list ((msg <Takeoff-request>))
  "Converts a ROS message object to a list"
  (list '<Takeoff-request>
))
;//! \htmlinclude Takeoff-response.msg.html

(defclass <Takeoff-response> (ros-message)
  ((successful
    :reader successful-val
    :initarg :successful
    :type boolean
    :initform nil))
)
(defmethod serialize ((msg <Takeoff-response>) ostream)
  "Serializes a message object of type '<Takeoff-response>"
    (write-byte (ldb (byte 8 0) (if (slot-value msg 'successful) 1 0)) ostream)
)
(defmethod deserialize ((msg <Takeoff-response>) istream)
  "Deserializes a message object of type '<Takeoff-response>"
  (setf (slot-value msg 'successful) (not (zerop (read-byte istream))))
  msg
)
(defmethod ros-datatype ((msg (eql '<Takeoff-response>)))
  "Returns string type for a service object of type '<Takeoff-response>"
  "ardrone_driver/TakeoffResponse")
(defmethod md5sum ((type (eql '<Takeoff-response>)))
  "Returns md5sum for a message object of type '<Takeoff-response>"
  "6ccdb752b565be6d250259ff3d0c7ea1")
(defmethod message-definition ((type (eql '<Takeoff-response>)))
  "Returns full string definition for message of type '<Takeoff-response>"
  (format nil "bool successful~%~%~%"))
(defmethod serialization-length ((msg <Takeoff-response>))
  (+ 0
     1
))
(defmethod ros-message-to-list ((msg <Takeoff-response>))
  "Converts a ROS message object to a list"
  (list '<Takeoff-response>
    (cons ':successful (successful-val msg))
))
(defmethod service-request-type ((msg (eql 'Takeoff)))
  '<Takeoff-request>)
(defmethod service-response-type ((msg (eql 'Takeoff)))
  '<Takeoff-response>)
(defmethod ros-datatype ((msg (eql 'Takeoff)))
  "Returns string type for a service object of type '<Takeoff>"
  "ardrone_driver/Takeoff")
