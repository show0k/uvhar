
(in-package :asdf)

(defsystem "ardrone_driver-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils)
  :components ((:file "_package")
    (:file "Land" :depends-on ("_package"))
    (:file "_package_Land" :depends-on ("_package"))
    (:file "Takeoff" :depends-on ("_package"))
    (:file "_package_Takeoff" :depends-on ("_package"))
    (:file "Reset" :depends-on ("_package"))
    (:file "_package_Reset" :depends-on ("_package"))
    ))
