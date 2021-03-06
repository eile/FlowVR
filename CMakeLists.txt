PROJECT(FlowVR-Suite)

########## CMAKE SETTINGS ################
# Requires cmake 2.8 at least
SET(flowvr_CMAKE_REQUIRED_VERSION "2.8")
# CMAKE_PATCH_VERSION only exists since early version of 2.4
IF(DEFINED CMAKE_PATCH_VERSION)
  # so we use CMAKE_MINIMUM_REQUIRED
  CMAKE_MINIMUM_REQUIRED(VERSION "${flowvr_CMAKE_REQUIRED_VERSION}" FATAL_ERROR)
ELSE(DEFINED CMAKE_PATCH_VERSION)
  # to early version of cmake, aborting
  MESSAGE(FATAL_ERROR "Your version of CMake is too old, please use at least ${flowvr_CMAKE_REQUIRED_VERSION}")
ENDIF(DEFINED CMAKE_PATCH_VERSION)

##########################################

SET(PROJECT_VERSION_MAJOR 2)
SET(PROJECT_VERSION_MINOR 1)
SET(PROJECT_VERSION_PATCH 0)
SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

##########################################
## TESTING ON
##########################################
ENABLE_TESTING()
INCLUDE(CTest)

SET( BUILD_DASHBOARD_REPORTS 1 ) 
SET( CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE 1)


##########################################
## FLOWVR-PROJECT SETTINGS
##########################################
SET(FlowVR_DIR ${CMAKE_SOURCE_DIR}/flowvr/cmake)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${FlowVR_DIR})
INCLUDE(MacroLogFeature) # this is found in the FlowVR_cmake_DIR/cmake 

####################################################################
#General Debug options
OPTION(BUILD_FLOWVR_DEBUG_MESSAGES "Enable -DDEBUG flag" FALSE)

IF (BUILD_FLOWVR_DEBUG_MESSAGES)
  ADD_DEFINITIONS( -DDEBUG )
ENDIF(BUILD_FLOWVR_DEBUG_MESSAGES)

####################################################################

# Warning MESSAGE is CMAKE_BUILD_TYPE not set
IF(NOT CMAKE_BUILD_TYPE)
  MESSAGE("CMAKE_BUILD_TYPE not set. Performance may be affected. Possible values are  Debug, Release, RelWithDebInfo or MinSizeRel\n")
ELSE(NOT CMAKE_BUILD_TYPE)
  MESSAGE("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}   (Possible values are  Debug, Release, RelWithDebInfo or MinSizeRel)\n")
ENDIF(NOT CMAKE_BUILD_TYPE)


####################################################################
## OPTIONAL COMPONENTS TO BE BUILD
####################################################################

OPTION(BUILD_CONTRIB "Flowvr Contribs contains various components, API extensions, tests, utility tools, etc." FALSE)
MACRO_LOG_FEATURE(BUILD_CONTRIB "Flowvr Contribs" 
                                       "contrib contains various components, API extensions, tests, utility tools, etc."  
                                       "http://flowvr.sf.net" 
                                       FALSE 
                                       "Optional")


####################################
# Set the components to compile
####################################
SET(components flowvr)

SET(ContribName contrib)
IF(BUILD_CONTRIB)
  SET(components ${components} ${ContribName})
  SET(FlowVRContrib_DIR ${CMAKE_SOURCE_DIR}/${ContribName}/cmake)
  IF(BUILD_CONTRIB_FLOWVR_VRPN)
    SET(components ${components} flowvr-vrpn)
  ENDIF(BUILD_CONTRIB_FLOWVR_VRPN)
  
  IF(BUILD_CONTRIB_FLOWVR_RENDER)
    SET(components ${components} flowvr-render)
  ENDIF(BUILD_CONTRIB_FLOWVR_RENDER)
  
  IF(BUILD_CONTRIB_VTK_FLOWVR)
    SET(components ${components} vtk-flowvr)
  ENDIF(BUILD_CONTRIB_VTK_FLOWVR)
ENDIF(BUILD_CONTRIB)



#####################################
# Additional components, config files
#####################################


#########################################################
# Display FLowVR - List  Enabled/Disabled components 
# (need to be called before to process components to 
# avoid messing-up with project names)
#########################################################
MACRO_DISPLAY_FEATURE_LOG()


####################################################################
# generate a string enumerating components from ${components} list
# used for flowvr-suite-config.* generation (components_STR is a
# variable used in these scripts, it will be replaced by a bash-
# readable string to loop over enabled components.
####################################################################
STRING(REPLACE ";" " " components_STR "${components}")

# CONFIGURE these scripts
FOREACH(config flowvr-suite-config.sh )
 # create a script in the current binary directory
 SET(INSTALL_PREFIX ${CMAKE_BINARY_DIR}) 
 CONFIGURE_FILE(${config}.in bin/${config} @ONLY) 
  
  # now create one for distribution, here, we replace the used
  # prefix to match the final destination
  SET(INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  
  # call it .dist in the binary-directory
  CONFIGURE_FILE(${config}.in ${config}.dist @ONLY)
  
  # create an installation rule to copy the .dist to the resulting
  # .sh  script in the installation directory
  INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/${config}.dist
          DESTINATION bin
          RENAME ${config})
ENDFOREACH(config)

##############################################################################
## ENABLE SUBDIRECTORIES / COMPONENTS OF FLOWVR
##############################################################################

FOREACH(component ${components})
  IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${component})
    ADD_SUBDIRECTORY(${component})
  ENDIF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${component})
ENDFOREACH(component)

###############################################################################
# Doxygen documentation
SET(DOCLIST doc-flowvr)

# IF( BUILD_CONTRIB_FLOWVR_RENDER)
#   SET(DOCLIST ${DOCLIST} doc-flowvr-render)
# ENDIF( BUILD_CONTRIB_FLOWVR_RENDER)

# IF( BUILD_CONTRIB_VTK_FLOWVR)
#   SET(DOCLIST ${DOCLIST} doc-vtk-flowvr)
# ENDIF(BUILD_CONTRIB_VTK_FLOWVR)

# IF(BUILD_CONTRIB_FLOWVR_VRPN)
#   SET(DOCLIST ${DOCLIST} doc-flowvr-vrpn)
# ENDIF(BUILD_CONTRIB_FLOWVR_VRPN)

IF(BUILD_CONTRIB)
  SET(DOCLIST ${DOCLIST} doc-flowvr-contrib)
ENDIF(BUILD_CONTRIB)


ADD_CUSTOM_TARGET(doc)
ADD_DEPENDENCIES(doc  ${DOCLIST})


################################################################
# packaging
################################################################
SET(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
SET(CPACK_PACKAGE_VENDOR "INRIA - Grenoble Universities")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "FlowVR-Suite: the FlowVR middleware and its associated tools for high performance interactive computing")
SET(CPACK_SOURCE_IGNORE_FILES "/CVS/;/.svn/;/build/;.swp;.#;\#" )
SET(CPACK_RESOURCE_FILE_LICENSE_PROVIDED "0")

# Pick a generator
IF(UNIX)
  IF(APPLE)
    SET(CPACK_GENERATOR "PackageMaker;STGZ")
  ELSE(APPLE)
    SET(CPACK_GENERATOR "STGZ;TGZ")
  ENDIF(APPLE)
  SET(CPACK_SOURCE_GENERATOR "TGZ")
ELSE(UNIX)
  SET(CPACK_GENERATOR "NSIS;ZIP")
  SET(CPACK_SOURCE_GENERATOR "ZIP")
ENDIF(UNIX)

INCLUDE(InstallRequiredSystemLibraries)
INCLUDE(CPack)

################################################################
