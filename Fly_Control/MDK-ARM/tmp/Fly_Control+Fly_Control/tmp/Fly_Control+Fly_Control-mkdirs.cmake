# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/JTJ/Desktop/Flight project/Fly_Control/MDK-ARM/tmp/Fly_Control+Fly_Control")
  file(MAKE_DIRECTORY "C:/Users/JTJ/Desktop/Flight project/Fly_Control/MDK-ARM/tmp/Fly_Control+Fly_Control")
endif()
file(MAKE_DIRECTORY
  "C:/Users/JTJ/Desktop/Flight project/Fly_Control/MDK-ARM/tmp/1"
  "C:/Users/JTJ/Desktop/Flight project/Fly_Control/MDK-ARM/tmp/Fly_Control+Fly_Control"
  "C:/Users/JTJ/Desktop/Flight project/Fly_Control/MDK-ARM/tmp/Fly_Control+Fly_Control/tmp"
  "C:/Users/JTJ/Desktop/Flight project/Fly_Control/MDK-ARM/tmp/Fly_Control+Fly_Control/src/Fly_Control+Fly_Control-stamp"
  "C:/Users/JTJ/Desktop/Flight project/Fly_Control/MDK-ARM/tmp/Fly_Control+Fly_Control/src"
  "C:/Users/JTJ/Desktop/Flight project/Fly_Control/MDK-ARM/tmp/Fly_Control+Fly_Control/src/Fly_Control+Fly_Control-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/JTJ/Desktop/Flight project/Fly_Control/MDK-ARM/tmp/Fly_Control+Fly_Control/src/Fly_Control+Fly_Control-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/JTJ/Desktop/Flight project/Fly_Control/MDK-ARM/tmp/Fly_Control+Fly_Control/src/Fly_Control+Fly_Control-stamp${cfgdir}") # cfgdir has leading slash
endif()
