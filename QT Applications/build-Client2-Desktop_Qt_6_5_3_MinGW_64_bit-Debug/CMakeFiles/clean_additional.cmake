# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Client2_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Client2_autogen.dir\\ParseCache.txt"
  "Client2_autogen"
  )
endif()
