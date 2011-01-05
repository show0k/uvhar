
(in-package :asdf)

(defsystem "serial_port_node-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils)
  :components ((:file "_package")
    (:file "SerialWrite" :depends-on ("_package"))
    (:file "_package_SerialWrite" :depends-on ("_package"))
    (:file "SerialRead" :depends-on ("_package"))
    (:file "_package_SerialRead" :depends-on ("_package"))
    (:file "SerialData" :depends-on ("_package"))
    (:file "_package_SerialData" :depends-on ("_package"))
    ))
