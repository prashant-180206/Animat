# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appAnimat_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appAnimat_autogen.dir\\ParseCache.txt"
  "appAnimat_autogen"
  "lib\\CMakeFiles\\muparser_autogen.dir\\AutogenUsed.txt"
  "lib\\CMakeFiles\\muparser_autogen.dir\\ParseCache.txt"
  "lib\\muparser_autogen"
  )
endif()
