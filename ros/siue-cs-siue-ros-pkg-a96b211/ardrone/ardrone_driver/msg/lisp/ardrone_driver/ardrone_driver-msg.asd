
(in-package :asdf)

(defsystem "ardrone_driver-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :roslib-msg
)
  :components ((:file "_package")
    (:file "Velocity" :depends-on ("_package"))
    (:file "_package_Velocity" :depends-on ("_package"))
    (:file "ARDroneState" :depends-on ("_package"))
    (:file "_package_ARDroneState" :depends-on ("_package"))
    ))
