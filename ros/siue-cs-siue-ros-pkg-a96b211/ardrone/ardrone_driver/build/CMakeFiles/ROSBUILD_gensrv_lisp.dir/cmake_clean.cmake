FILE(REMOVE_RECURSE
  "CMakeFiles/ROSBUILD_gensrv_lisp"
  "../srv/lisp/ardrone_driver/Land.lisp"
  "../srv/lisp/ardrone_driver/_package.lisp"
  "../srv/lisp/ardrone_driver/_package_Land.lisp"
  "../srv/lisp/ardrone_driver/Takeoff.lisp"
  "../srv/lisp/ardrone_driver/_package.lisp"
  "../srv/lisp/ardrone_driver/_package_Takeoff.lisp"
  "../srv/lisp/ardrone_driver/Reset.lisp"
  "../srv/lisp/ardrone_driver/_package.lisp"
  "../srv/lisp/ardrone_driver/_package_Reset.lisp"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_gensrv_lisp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
