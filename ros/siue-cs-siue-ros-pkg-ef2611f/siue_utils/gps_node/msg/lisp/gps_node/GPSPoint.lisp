; Auto-generated. Do not edit!


(in-package gps_node-msg)


;//! \htmlinclude GPSPoint.msg.html

(defclass <GPSPoint> (ros-message)
  ((lat
    :reader lat-val
    :initarg :lat
    :type string
    :initform "")
   (lon
    :reader lon-val
    :initarg :lon
    :type string
    :initform "")
   (speed
    :reader speed-val
    :initarg :speed
    :type float
    :initform 0.0)
   (status
    :reader status-val
    :initarg :status
    :type fixnum
    :initform 0)
   (bearing
    :reader bearing-val
    :initarg :bearing
    :type float
    :initform 0.0))
)
(defmethod serialize ((msg <GPSPoint>) ostream)
  "Serializes a message object of type '<GPSPoint>"
  (let ((__ros_str_len (length (slot-value msg 'lat))))
    (write-byte (ldb (byte 8 0) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 8) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 16) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 24) __ros_str_len) ostream))
  (map nil #'(lambda (c) (write-byte (char-code c) ostream)) (slot-value msg 'lat))
  (let ((__ros_str_len (length (slot-value msg 'lon))))
    (write-byte (ldb (byte 8 0) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 8) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 16) __ros_str_len) ostream)
    (write-byte (ldb (byte 8 24) __ros_str_len) ostream))
  (map nil #'(lambda (c) (write-byte (char-code c) ostream)) (slot-value msg 'lon))
  (let ((bits (roslisp-utils:encode-single-float-bits (slot-value msg 'speed))))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream))
    (write-byte (ldb (byte 8 0) (slot-value msg 'status)) ostream)
  (let ((bits (roslisp-utils:encode-single-float-bits (slot-value msg 'bearing))))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream))
)
(defmethod deserialize ((msg <GPSPoint>) istream)
  "Deserializes a message object of type '<GPSPoint>"
  (let ((__ros_str_len 0))
    (setf (ldb (byte 8 0) __ros_str_len) (read-byte istream))
    (setf (ldb (byte 8 8) __ros_str_len) (read-byte istream))
    (setf (ldb (byte 8 16) __ros_str_len) (read-byte istream))
    (setf (ldb (byte 8 24) __ros_str_len) (read-byte istream))
    (setf (slot-value msg 'lat) (make-string __ros_str_len))
    (dotimes (__ros_str_idx __ros_str_len msg)
      (setf (char (slot-value msg 'lat) __ros_str_idx) (code-char (read-byte istream)))))
  (let ((__ros_str_len 0))
    (setf (ldb (byte 8 0) __ros_str_len) (read-byte istream))
    (setf (ldb (byte 8 8) __ros_str_len) (read-byte istream))
    (setf (ldb (byte 8 16) __ros_str_len) (read-byte istream))
    (setf (ldb (byte 8 24) __ros_str_len) (read-byte istream))
    (setf (slot-value msg 'lon) (make-string __ros_str_len))
    (dotimes (__ros_str_idx __ros_str_len msg)
      (setf (char (slot-value msg 'lon) __ros_str_idx) (code-char (read-byte istream)))))
  (let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (slot-value msg 'speed) (roslisp-utils:decode-single-float-bits bits)))
  (setf (ldb (byte 8 0) (slot-value msg 'status)) (read-byte istream))
  (let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (slot-value msg 'bearing) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(defmethod ros-datatype ((msg (eql '<GPSPoint>)))
  "Returns string type for a message object of type '<GPSPoint>"
  "gps_node/GPSPoint")
(defmethod md5sum ((type (eql '<GPSPoint>)))
  "Returns md5sum for a message object of type '<GPSPoint>"
  "153c325296e395b378bbb3ea6de6f97f")
(defmethod message-definition ((type (eql '<GPSPoint>)))
  "Returns full string definition for message of type '<GPSPoint>"
  (format nil "string lat~%string lon~%float32 speed~%int8 status~%float32 bearing~%~%~%"))
(defmethod serialization-length ((msg <GPSPoint>))
  (+ 0
     4 (length (slot-value msg 'lat))
     4 (length (slot-value msg 'lon))
     4
     1
     4
))
(defmethod ros-message-to-list ((msg <GPSPoint>))
  "Converts a ROS message object to a list"
  (list '<GPSPoint>
    (cons ':lat (lat-val msg))
    (cons ':lon (lon-val msg))
    (cons ':speed (speed-val msg))
    (cons ':status (status-val msg))
    (cons ':bearing (bearing-val msg))
))
