# PRIMES EXAMPLE 

INCLUDE_DIRECTORIES(${FLOWVR_INCLUDE_DIR})

# MODULE  BINARIES

IF(BUILD_MODULES)
  
  INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
  
  FOREACH(target compute capture visu)
    ADD_EXECUTABLE(${target} ${target}.cpp)
    # Should work but Linux complain about not finding dl lib using by flowvr-plugd
    #  TARGET_LINK_LIBRARIES(${target} ${FLOWVR_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
    TARGET_LINK_LIBRARIES(${target}  ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${FLOWVR_base_LIBRARY} ${FLOWVR_ftl_LIBRARY} ${FLOWVR_mod_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} )
    INSTALL(TARGETS ${target}  DESTINATION ${MODULE_DIR})	
  ENDFOREACH(target)
  
ENDIF(BUILD_MODULES)
