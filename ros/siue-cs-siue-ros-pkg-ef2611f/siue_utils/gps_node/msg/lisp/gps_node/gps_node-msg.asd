
(in-package :asdf)

(defsystem "gps_node-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils)
  :components ((:file "_package")
    (:file "GPSPoint" :depends-on ("_package"))
    (:file "_package_GPSPoint" :depends-on ("_package"))
    ))
