; Auto-generated. Do not edit!


(in-package ardrone_driver-srv)


;//! \htmlinclude Land-request.msg.html

(defclass <Land-request> (ros-message)
  ()
)
(defmethod serialize ((msg <Land-request>) ostream)
  "Serializes a message object of type '<Land-request>"
)
(defmethod deserialize ((msg <Land-request>) istream)
  "Deserializes a message object of type '<Land-request>"
  msg
)
(defmethod ros-datatype ((msg (eql '<Land-request>)))
  "Returns string type for a service object of type '<Land-request>"
  "ardrone_driver/LandRequest")
(defmethod md5sum ((type (eql '<Land-request>)))
  "Returns md5sum for a message object of type '<Land-request>"
  "6ccdb752b565be6d250259ff3d0c7ea1")
(defmethod message-definition ((type (eql '<Land-request>)))
  "Returns full string definition for message of type '<Land-request>"
  (format nil "~%"))
(defmethod serialization-length ((msg <Land-request>))
  (+ 0
))
(defmethod ros-message-to-list ((msg <Land-request>))
  "Converts a ROS message object to a list"
  (list '<Land-request>
))
;//! \htmlinclude Land-response.msg.html

(defclass <Land-response> (ros-message)
  ((successful
    :reader successful-val
    :initarg :successful
    :type boolean
    :initform nil))
)
(defmethod serialize ((msg <Land-response>) ostream)
  "Serializes a message object of type '<Land-response>"
    (write-byte (ldb (byte 8 0) (if (slot-value msg 'successful) 1 0)) ostream)
)
(defmethod deserialize ((msg <Land-response>) istream)
  "Deserializes a message object of type '<Land-response>"
  (setf (slot-value msg 'successful) (not (zerop (read-byte istream))))
  msg
)
(defmethod ros-datatype ((msg (eql '<Land-response>)))
  "Returns string type for a service object of type '<Land-response>"
  "ardrone_driver/LandResponse")
(defmethod md5sum ((type (eql '<Land-response>)))
  "Returns md5sum for a message object of type '<Land-response>"
  "6ccdb752b565be6d250259ff3d0c7ea1")
(defmethod message-definition ((type (eql '<Land-response>)))
  "Returns full string definition for message of type '<Land-response>"
  (format nil "bool successful~%~%~%"))
(defmethod serialization-length ((msg <Land-response>))
  (+ 0
     1
))
(defmethod ros-message-to-list ((msg <Land-response>))
  "Converts a ROS message object to a list"
  (list '<Land-response>
    (cons ':successful (successful-val msg))
))
(defmethod service-request-type ((msg (eql 'Land)))
  '<Land-request>)
(defmethod service-response-type ((msg (eql 'Land)))
  '<Land-response>)
(defmethod ros-datatype ((msg (eql 'Land)))
  "Returns string type for a service object of type '<Land>"
  "ardrone_driver/Land")
