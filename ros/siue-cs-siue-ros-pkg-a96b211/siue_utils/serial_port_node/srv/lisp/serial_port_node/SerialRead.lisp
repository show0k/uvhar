; Auto-generated. Do not edit!


(in-package serial_port_node-srv)


;//! \htmlinclude SerialRead-request.msg.html

(defclass <SerialRead-request> (ros-message)
  ((length
    :reader length-val
    :initarg :length
    :type fixnum
    :initform 0))
)
(defmethod serialize ((msg <SerialRead-request>) ostream)
  "Serializes a message object of type '<SerialRead-request>"
    (write-byte (ldb (byte 8 0) (slot-value msg 'length)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'length)) ostream)
)
(defmethod deserialize ((msg <SerialRead-request>) istream)
  "Deserializes a message object of type '<SerialRead-request>"
  (setf (ldb (byte 8 0) (slot-value msg 'length)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'length)) (read-byte istream))
  msg
)
(defmethod ros-datatype ((msg (eql '<SerialRead-request>)))
  "Returns string type for a service object of type '<SerialRead-request>"
  "serial_port_node/SerialReadRequest")
(defmethod md5sum ((type (eql '<SerialRead-request>)))
  "Returns md5sum for a message object of type '<SerialRead-request>"
  "2f7fdba859bced703f7f6326ffb3f829")
(defmethod message-definition ((type (eql '<SerialRead-request>)))
  "Returns full string definition for message of type '<SerialRead-request>"
  (format nil "int16 length~%~%"))
(defmethod serialization-length ((msg <SerialRead-request>))
  (+ 0
     2
))
(defmethod ros-message-to-list ((msg <SerialRead-request>))
  "Converts a ROS message object to a list"
  (list '<SerialRead-request>
    (cons ':length (length-val msg))
))
;//! \htmlinclude SerialRead-response.msg.html

(defclass <SerialRead-response> (ros-message)
  ((data
    :reader data-val
    :initarg :data
    :type string
    :initform "")
   (bytesRead
    :reader bytesRead-val
    :initarg :bytesRead
    :type fixnum
    :initform 0)
   (status
    :reader status-val
    :initarg :status
    :type boolean
    :initform nil))
)
(defmethod serialize ((msg <SerialRead-response>) ostream)
  "Serializes a message object of type '<SerialRead-response>"
  (let ((__ros_str_len (length (slot-value msg 'data))))
    (write-byte (ldb (byte 8 0) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 8) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 16) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 24) __ros_str_len) ostream))
  (map nil #'(lambda (c) (write-byte (char-code c) ostream)) (slot-value msg 'data))
    (write-byte (ldb (byte 8 0) (slot-value msg 'bytesRead)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'bytesRead)) ostream)
    (write-byte (ldb (byte 8 0) (if (slot-value msg 'status) 1 0)) ostream)
)
(defmethod deserialize ((msg <SerialRead-response>) istream)
  "Deserializes a message object of type '<SerialRead-response>"
  (let ((__ros_str_len 0))
    (setf (ldb (byte 8 0) __ros_str_len) (read-byte istream))
    (setf (ldb (byte 8 8) __ros_str_len) (read-byte istream))
    (setf (ldb (byte 8 16) __ros_str_len) (read-byte istream))
    (setf (ldb (byte 8 24) __ros_str_len) (read-byte istream))
    (setf (slot-value msg 'data) (make-string __ros_str_len))
    (dotimes (__ros_str_idx __ros_str_len msg)
      (setf (char (slot-value msg 'data) __ros_str_idx) (code-char (read-byte istream)))))
  (setf (ldb (byte 8 0) (slot-value msg 'bytesRead)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'bytesRead)) (read-byte istream))
  (setf (slot-value msg 'status) (not (zerop (read-byte istream))))
  msg
)
(defmethod ros-datatype ((msg (eql '<SerialRead-response>)))
  "Returns string type for a service object of type '<SerialRead-response>"
  "serial_port_node/SerialReadResponse")
(defmethod md5sum ((type (eql '<SerialRead-response>)))
  "Returns md5sum for a message object of type '<SerialRead-response>"
  "2f7fdba859bced703f7f6326ffb3f829")
(defmethod message-definition ((type (eql '<SerialRead-response>)))
  "Returns full string definition for message of type '<SerialRead-response>"
  (format nil "string data~%int16 bytesRead~%bool status~%~%~%"))
(defmethod serialization-length ((msg <SerialRead-response>))
  (+ 0
     4 (length (slot-value msg 'data))
     2
     1
))
(defmethod ros-message-to-list ((msg <SerialRead-response>))
  "Converts a ROS message object to a list"
  (list '<SerialRead-response>
    (cons ':data (data-val msg))
    (cons ':bytesRead (bytesRead-val msg))
    (cons ':status (status-val msg))
))
(defmethod service-request-type ((msg (eql 'SerialRead)))
  '<SerialRead-request>)
(defmethod service-response-type ((msg (eql 'SerialRead)))
  '<SerialRead-response>)
(defmethod ros-datatype ((msg (eql 'SerialRead)))
  "Returns string type for a service object of type '<SerialRead>"
  "serial_port_node/SerialRead")
