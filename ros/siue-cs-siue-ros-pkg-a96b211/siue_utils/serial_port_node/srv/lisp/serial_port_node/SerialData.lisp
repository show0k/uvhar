; Auto-generated. Do not edit!


(in-package serial_port_node-srv)


;//! \htmlinclude SerialData-request.msg.html

(defclass <SerialData-request> (ros-message)
  ((data
    :reader data-val
    :initarg :data
    :type string
    :initform "")
   (length
    :reader length-val
    :initarg :length
    :type fixnum
    :initform 0))
)
(defmethod serialize ((msg <SerialData-request>) ostream)
  "Serializes a message object of type '<SerialData-request>"
  (let ((__ros_str_len (length (slot-value msg 'data))))
    (write-byte (ldb (byte 8 0) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 8) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 16) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 24) __ros_str_len) ostream))
  (map nil #'(lambda (c) (write-byte (char-code c) ostream)) (slot-value msg 'data))
    (write-byte (ldb (byte 8 0) (slot-value msg 'length)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'length)) ostream)
)
(defmethod deserialize ((msg <SerialData-request>) istream)
  "Deserializes a message object of type '<SerialData-request>"
  (let ((__ros_str_len 0))
    (setf (ldb (byte 8 0) __ros_str_len) (read-byte istream))
    (setf (ldb (byte 8 8) __ros_str_len) (read-byte istream))
    (setf (ldb (byte 8 16) __ros_str_len) (read-byte istream))
    (setf (ldb (byte 8 24) __ros_str_len) (read-byte istream))
    (setf (slot-value msg 'data) (make-string __ros_str_len))
    (dotimes (__ros_str_idx __ros_str_len msg)
      (setf (char (slot-value msg 'data) __ros_str_idx) (code-char (read-byte istream)))))
  (setf (ldb (byte 8 0) (slot-value msg 'length)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'length)) (read-byte istream))
  msg
)
(defmethod ros-datatype ((msg (eql '<SerialData-request>)))
  "Returns string type for a service object of type '<SerialData-request>"
  "serial_port_node/SerialDataRequest")
(defmethod md5sum ((type (eql '<SerialData-request>)))
  "Returns md5sum for a message object of type '<SerialData-request>"
  "32dda19cd14525db231dce93a2ad9e7d")
(defmethod message-definition ((type (eql '<SerialData-request>)))
  "Returns full string definition for message of type '<SerialData-request>"
  (format nil "string data~%int16 length~%~%"))
(defmethod serialization-length ((msg <SerialData-request>))
  (+ 0
     4 (length (slot-value msg 'data))
     2
))
(defmethod ros-message-to-list ((msg <SerialData-request>))
  "Converts a ROS message object to a list"
  (list '<SerialData-request>
    (cons ':data (data-val msg))
    (cons ':length (length-val msg))
))
;//! \htmlinclude SerialData-response.msg.html

(defclass <SerialData-response> (ros-message)
  ((status
    :reader status-val
    :initarg :status
    :type boolean
    :initform nil))
)
(defmethod serialize ((msg <SerialData-response>) ostream)
  "Serializes a message object of type '<SerialData-response>"
    (write-byte (ldb (byte 8 0) (if (slot-value msg 'status) 1 0)) ostream)
)
(defmethod deserialize ((msg <SerialData-response>) istream)
  "Deserializes a message object of type '<SerialData-response>"
  (setf (slot-value msg 'status) (not (zerop (read-byte istream))))
  msg
)
(defmethod ros-datatype ((msg (eql '<SerialData-response>)))
  "Returns string type for a service object of type '<SerialData-response>"
  "serial_port_node/SerialDataResponse")
(defmethod md5sum ((type (eql '<SerialData-response>)))
  "Returns md5sum for a message object of type '<SerialData-response>"
  "32dda19cd14525db231dce93a2ad9e7d")
(defmethod message-definition ((type (eql '<SerialData-response>)))
  "Returns full string definition for message of type '<SerialData-response>"
  (format nil "bool status~%~%~%"))
(defmethod serialization-length ((msg <SerialData-response>))
  (+ 0
     1
))
(defmethod ros-message-to-list ((msg <SerialData-response>))
  "Converts a ROS message object to a list"
  (list '<SerialData-response>
    (cons ':status (status-val msg))
))
(defmethod service-request-type ((msg (eql 'SerialData)))
  '<SerialData-request>)
(defmethod service-response-type ((msg (eql 'SerialData)))
  '<SerialData-response>)
(defmethod ros-datatype ((msg (eql 'SerialData)))
  "Returns string type for a service object of type '<SerialData>"
  "serial_port_node/SerialData")
