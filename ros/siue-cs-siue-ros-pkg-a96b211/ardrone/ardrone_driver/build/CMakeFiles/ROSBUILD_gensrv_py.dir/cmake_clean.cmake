FILE(REMOVE_RECURSE
  "CMakeFiles/ROSBUILD_gensrv_py"
  "../src/ardrone_driver/srv/__init__.py"
  "../src/ardrone_driver/srv/_Land.py"
  "../src/ardrone_driver/srv/_Takeoff.py"
  "../src/ardrone_driver/srv/_Reset.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_gensrv_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
