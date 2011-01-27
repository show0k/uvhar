; Auto-generated. Do not edit!


(in-package ardrone_driver-msg)


;//! \htmlinclude ARDroneState.msg.html

(defclass <ARDroneState> (ros-message)
  ((header
    :reader header-val
    :initarg :header
    :type roslib-msg:<Header>
    :initform (make-instance 'roslib-msg:<Header>))
   (altitude
    :reader altitude-val
    :initarg :altitude
    :type float
    :initform 0.0)
   (is_airborne
    :reader is_airborne-val
    :initarg :is_airborne
    :type boolean
    :initform nil)
   (battery_percentage
    :reader battery_percentage-val
    :initarg :battery_percentage
    :type integer
    :initform 0)
   (battery_voltage
    :reader battery_voltage-val
    :initarg :battery_voltage
    :type float
    :initform 0.0)
   (estop_state
    :reader estop_state-val
    :initarg :estop_state
    :type boolean
    :initform nil)
   (vels
    :reader vels-val
    :initarg :vels
    :type ardrone_driver-msg:<Velocity>
    :initform (make-instance 'ardrone_driver-msg:<Velocity>)))
)
(defmethod serialize ((msg <ARDroneState>) ostream)
  "Serializes a message object of type '<ARDroneState>"
  (serialize (slot-value msg 'header) ostream)
  (let ((bits (roslisp-utils:encode-single-float-bits (slot-value msg 'altitude))))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream))
    (write-byte (ldb (byte 8 0) (if (slot-value msg 'is_airborne) 1 0)) ostream)
    (write-byte (ldb (byte 8 0) (slot-value msg 'battery_percentage)) ostream)
  (write-byte (ldb (byte 8 8) (slot-value msg 'battery_percentage)) ostream)
  (write-byte (ldb (byte 8 16) (slot-value msg 'battery_percentage)) ostream)
  (write-byte (ldb (byte 8 24) (slot-value msg 'battery_percentage)) ostream)
  (let ((bits (roslisp-utils:encode-single-float-bits (slot-value msg 'battery_voltage))))
    (write-byte (ldb (byte 8 0) bits) ostream)
    (write-byte (ldb (byte 8 8) bits) ostream)
    (write-byte (ldb (byte 8 16) bits) ostream)
    (write-byte (ldb (byte 8 24) bits) ostream))
    (write-byte (ldb (byte 8 0) (if (slot-value msg 'estop_state) 1 0)) ostream)
  (serialize (slot-value msg 'vels) ostream)
)
(defmethod deserialize ((msg <ARDroneState>) istream)
  "Deserializes a message object of type '<ARDroneState>"
  (deserialize (slot-value msg 'header) istream)
  (let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (slot-value msg 'altitude) (roslisp-utils:decode-single-float-bits bits)))
  (setf (slot-value msg 'is_airborne) (not (zerop (read-byte istream))))
  (setf (ldb (byte 8 0) (slot-value msg 'battery_percentage)) (read-byte istream))
  (setf (ldb (byte 8 8) (slot-value msg 'battery_percentage)) (read-byte istream))
  (setf (ldb (byte 8 16) (slot-value msg 'battery_percentage)) (read-byte istream))
  (setf (ldb (byte 8 24) (slot-value msg 'battery_percentage)) (read-byte istream))
  (let ((bits 0))
    (setf (ldb (byte 8 0) bits) (read-byte istream))
    (setf (ldb (byte 8 8) bits) (read-byte istream))
    (setf (ldb (byte 8 16) bits) (read-byte istream))
    (setf (ldb (byte 8 24) bits) (read-byte istream))
    (setf (slot-value msg 'battery_voltage) (roslisp-utils:decode-single-float-bits bits)))
  (setf (slot-value msg 'estop_state) (not (zerop (read-byte istream))))
  (deserialize (slot-value msg 'vels) istream)
  msg
)
(defmethod ros-datatype ((msg (eql '<ARDroneState>)))
  "Returns string type for a message object of type '<ARDroneState>"
  "ardrone_driver/ARDroneState")
(defmethod md5sum ((type (eql '<ARDroneState>)))
  "Returns md5sum for a message object of type '<ARDroneState>"
  "b785b22a90b6e1d04537476916861f8a")
(defmethod message-definition ((type (eql '<ARDroneState>)))
  "Returns full string definition for message of type '<ARDroneState>"
  (format nil "Header header~%~%#The current altitude, in meters~%float32 altitude~%~%#Indicates whether the drone is airborne~%bool is_airborne~%~%#Percentage of battery life remaining. Note that because of the way the hardware reports it's battery life~%# the drone could still be powered on, yet be reporting a battery percentage of 0. This means it has determined~%# it does not have enough battery to fly, and it will not takeoff if commanded to.~%int32 battery_percentage~%~%#Battery voltage~%float32 battery_voltage~%~%#Indicates if the drone is in emergency mode~%bool estop_state~%~%#Current speed in meters/sec, calculated by the onboard IMU data fused with optical flow~%# from the downward facing camera~%ardrone_driver/Velocity vels~%~%================================================================================~%MSG: roslib/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.secs: seconds (stamp_secs) since epoch~%# * stamp.nsecs: nanoseconds since stamp_secs~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: ardrone_driver/Velocity~%float32 x~%float32 y~%float32 z~%~%~%"))
(defmethod serialization-length ((msg <ARDroneState>))
  (+ 0
     (serialization-length (slot-value msg 'header))
     4
     1
     4
     4
     1
     (serialization-length (slot-value msg 'vels))
))
(defmethod ros-message-to-list ((msg <ARDroneState>))
  "Converts a ROS message object to a list"
  (list '<ARDroneState>
    (cons ':header (header-val msg))
    (cons ':altitude (altitude-val msg))
    (cons ':is_airborne (is_airborne-val msg))
    (cons ':battery_percentage (battery_percentage-val msg))
    (cons ':battery_voltage (battery_voltage-val msg))
    (cons ':estop_state (estop_state-val msg))
    (cons ':vels (vels-val msg))
))
