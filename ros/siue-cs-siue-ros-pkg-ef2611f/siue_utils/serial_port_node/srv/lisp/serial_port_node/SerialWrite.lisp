; Auto-generated. Do not edit!


(in-package serial_port_node-srv)


;//! \htmlinclude SerialWrite-request.msg.html

(defclass <SerialWrite-request> (ros-message)
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
(defmethod serialize ((msg <SerialWrite-request>) ostream)
  "Serializes a message object of type '<SerialWrite-request>"
  (let ((__ros_str_len (length (slot-value msg 'data))))
    (write-byte (ldb (byte 8 0) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 8) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 16) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 24) __ros_str_len) ostream))
  (map nil #'(lambda (c) (write-byte (char-code c) ostream)) (slot-value msg 'data))
    (write-byte (ldb (byte 8 0) (slot-value msg 'length)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'length)) ostream)
)
(defmethod deserialize ((msg <SerialWrite-request>) istream)
  "Deserializes a message object of type '<SerialWrite-request>"
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
(defmethod ros-datatype ((msg (eql '<SerialWrite-request>)))
  "Returns string type for a service object of type '<SerialWrite-request>"
  "serial_port_node/SerialWriteRequest")
(defmethod md5sum ((type (eql '<SerialWrite-request>)))
  "Returns md5sum for a message object of type '<SerialWrite-request>"
  "9ddbb730a027ebe9b37465e267e69b16")
(defmethod message-definition ((type (eql '<SerialWrite-request>)))
  "Returns full string definition for message of type '<SerialWrite-request>"
  (format nil "string data~%int16 length~%~%"))
(defmethod serialization-length ((msg <SerialWrite-request>))
  (+ 0
     4 (length (slot-value msg 'data))
     2
))
(defmethod ros-message-to-list ((msg <SerialWrite-request>))
  "Converts a ROS message object to a list"
  (list '<SerialWrite-request>
    (cons ':data (data-val msg))
    (cons ':length (length-val msg))
))
;//! \htmlinclude SerialWrite-response.msg.html

(defclass <SerialWrite-response> (ros-message)
  ((bytesWritten
    :reader bytesWritten-val
    :initarg :bytesWritten
    :type fixnum
    :initform 0)
   (status
    :reader status-val
    :initarg :status
    :type boolean
    :initform nil))
)
(defmethod serialize ((msg <SerialWrite-response>) ostream)
  "Serializes a message object of type '<SerialWrite-response>"
    (write-byte (ldb (byte 8 0) (slot-value msg 'bytesWritten)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'bytesWritten)) ostream)
    (write-byte (ldb (byte 8 0) (if (slot-value msg 'status) 1 0)) ostream)
)
(defmethod deserialize ((msg <SerialWrite-response>) istream)
  "Deserializes a message object of type '<SerialWrite-response>"
  (setf (ldb (byte 8 0) (slot-value msg 'bytesWritten)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'bytesWritten)) (read-byte istream))
  (setf (slot-value msg 'status) (not (zerop (read-byte istream))))
  msg
)
(defmethod ros-datatype ((msg (eql '<SerialWrite-response>)))
  "Returns string type for a service object of type '<SerialWrite-response>"
  "serial_port_node/SerialWriteResponse")
(defmethod md5sum ((type (eql '<SerialWrite-response>)))
  "Returns md5sum for a message object of type '<SerialWrite-response>"
  "9ddbb730a027ebe9b37465e267e69b16")
(defmethod message-definition ((type (eql '<SerialWrite-response>)))
  "Returns full string definition for message of type '<SerialWrite-response>"
  (format nil "int16 bytesWritten~%bool status~%~%~%"))
(defmethod serialization-length ((msg <SerialWrite-response>))
  (+ 0
     2
     1
))
(defmethod ros-message-to-list ((msg <SerialWrite-response>))
  "Converts a ROS message object to a list"
  (list '<SerialWrite-response>
    (cons ':bytesWritten (bytesWritten-val msg))
    (cons ':status (status-val msg))
))
(defmethod service-request-type ((msg (eql 'SerialWrite)))
  '<SerialWrite-request>)
(defmethod service-response-type ((msg (eql 'SerialWrite)))
  '<SerialWrite-response>)
(defmethod ros-datatype ((msg (eql 'SerialWrite)))
  "Returns string type for a service object of type '<SerialWrite>"
  "serial_port_node/SerialWrite")
