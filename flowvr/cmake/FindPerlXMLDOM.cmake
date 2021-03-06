# - Try to find Perl Module XML::DOM
# Once done this will define
#
#  PERLXMLDOM_FOUND - system has Perl module XML::DOM
#
MESSAGE(STATUS "Looking for Perl Module XML::DOM")

WRITE_FILE(conftest.pl "use XML::DOM;")
EXECUTE_PROCESS(
          COMMAND ${PERL_EXECUTABLE} conftest.pl
	  RESULT_VARIABLE RESULT
	  OUTPUT_QUIET
	  ERROR_QUIET
	  )	

IF(RESULT)
  SET(PERLXMLDOM_FOUND FALSE)
ELSE(RESULT)
  SET(PERLXMLDOM_FOUND TRUE)
ENDIF(RESULT)


IF(PERLXMLDOM_FOUND)
  IF(NOT PerlXMLDOM_FIND_QUIETLY)
  MESSAGE(STATUS "Found Perl Module XML::DOM")
  ENDIF(NOT PerlXMLDOM_FIND_QUIETLY)
ELSE(PERLXMLDOM_FOUND)
  IF(PerlXMLDOM_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Perl module XML::DOM")
  ENDIF(PerlXMLDOM_FIND_REQUIRED)
ENDIF(PERLXMLDOM_FOUND)


