PROJECT(fluid)

INCLUDE(CTest)
ENABLE_TESTING()

SET(PROJECT_VERSION_MAJOR "1")
SET(PROJECT_VERSION_MINOR "0")
SET(PROJECT_VERSION_PATCH "0")
SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")


# CMAKE SETTINGS

#Require cmake 2.8 at least
IF(DEFINED CMAKE_PATCH_VERSION)
  CMAKE_MINIMUM_REQUIRED(VERSION "2.8" FATAL_ERROR)
ELSE(DEFINED CMAKE_PATCH_VERSION)
  MESSAGE(FATAL_ERROR "Your version of CMake is too old, please use at least cmake 2.6")
ENDIF(DEFINED CMAKE_PATCH_VERSION)


# Application  Name

SET(app fluid)

# INSTALLATION DIRECTORIES

SET(INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
SET(LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
SET(COMP_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
SET(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
SET(MODULE_DIR ${CMAKE_INSTALL_PREFIX}/bin) 
SET(CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/share/fluid/cmake) # Use namespace to define directory: share/name/space/cmake





# CHECK FLOWVR DEPENDENCY

FIND_PACKAGE(FlowVR REQUIRED)
IF(FLOWVR_FOUND)
 MESSAGE(STATUS "FlowVR Found")
ELSE(FLOWVR_FOUND)
 MESSAGE(FATAL_ERROR "FlowVR Not Found")
ENDIF(FLOWVR_FOUND)


FIND_PACKAGE(MPI)
IF(MPI_FOUND)
  OPTION(BUILD_MPI_FLUID "Build the MPI version of the fluid example" TRUE)
ELSE(MPI_FOUND)
  OPTION(BUILD_MPI_FLUID "Build the MPI version of the fluid example" FALSE)
ENDIF(MPI_FOUND)
	

#CHECK MPI  DEPENDENCY
IF(BUILD_MPI_FLUID)
  SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH} ) # To be able to load FindMPI.cmake and avoid the buggy version provided with cmake 2.4.7
  FIND_PACKAGE(MPI REQUIRED)
  IF (MPI_CXX_FOUND) 
    MESSAGE(STATUS  "MPI Found")
  ELSE (MPI_CXX_FOUND) 
    # Disable  Option if missing dependencies
    SET(BUILD_MPI_FLUID FALSE CACHE BOOL "Build the MPI version of the fluid example" FORCE)
    MESSAGE(STATUS "Parallel FLUID Example (MPI based): MPI CXX missing. BUILD_MPI_FLUID option turned OFF")
  ENDIF (MPI_CXX_FOUND) 
ENDIF (BUILD_MPI_FLUID) 



OPTION(BUILD_MODULES "build application  module binaries" TRUE)

IF(BUILD_MODULES)

  #CHECK OPENGL  DEPENDENCY
  FIND_PACKAGE(OpenGL REQUIRED)
  IF (OPENGL_FOUND) 
    MESSAGE(STATUS  "OpenGL Found")
  ELSE (OPENGL_FOUND) 
    MESSAGE(FATAL_ERROR "OpenGL Not Found (required to compile modules)")
    SET(BUILD_MODULE FALSE)
  ENDIF (OPENGL_FOUND) 
  
  
  #CHECK GLUT  DEPENDENCY
  FIND_PACKAGE(GLUT REQUIRED)
  IF (GLUT_FOUND) 
    MESSAGE(STATUS  "GLUT Found")
  ELSE (GLUT_FOUND) 
    MESSAGE(FATAL_ERROR "GLUT Not Found (required to compile modules)" )
    SET(BUILD_MODULE FALSE)
  ENDIF (GLUT_FOUND) 

  #CHECK THREADS DEPENDENCY
  FIND_PACKAGE(Threads REQUIRED)

ENDIF(BUILD_MODULES)

# Configuration Files 

FOREACH(shell sh )
  CONFIGURE_FILE(config/app-config.${shell}.in ${CMAKE_CURRENT_BINARY_DIR}/${app}-config.${shell} @ONLY)
  INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${app}-config.${shell} DESTINATION ${BIN_DIR})
ENDFOREACH(shell sh )

CONFIGURE_FILE(config/appConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/${app}Config.cmake @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${app}Config.cmake   DESTINATION ${CMAKE_DIR})


ADD_SUBDIRECTORY(src)


# INSTALL INCLUDE FILES
FILE(GLOB include-files ${COMP_INCLUDE_DIR}/*.h) 
INSTALL(FILES ${include-files} DESTINATION ${COMP_INCLUDE_DIR})

INSTALL(CODE "MESSAGE(STATUS \"!!!Rember to execute \n. ${BIN_DIR}/flowvr-${app}-config.sh\n to properly run the examples \n(include it in your .bashrc for a permanent installation)\") ")

