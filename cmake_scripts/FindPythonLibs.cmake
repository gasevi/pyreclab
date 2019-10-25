
#
# Preamble
#

set( PACKAGE_NAME PYTHON )

string( REPLACE "." ";" PY_VERSION_LIST ${CMAKE_PYTHON_VERSION} )
list( GET PY_VERSION_LIST 0 CMAKE_PYTHON_VERSION_MAJ )
list( GET PY_VERSION_LIST 1 CMAKE_PYTHON_VERSION_MIN )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
  string( REGEX REPLACE "\\." "" PYTHON_TAG "cp${CMAKE_PYTHON_VERSION}" )

  if( CMAKE_PYTHON_UCS4 )
    if( ${CMAKE_PYTHON_UCS4} )
      string( REGEX REPLACE "\\." "" ABI_TAG "cp${CMAKE_PYTHON_VERSION}mu" )
    else( ${CMAKE_PYTHON_UCS4} )
      string( REGEX REPLACE "\\." "" ABI_TAG "cp${CMAKE_PYTHON_VERSION}m" )
    endif( ${CMAKE_PYTHON_UCS4} )
  else( CMAKE_PYTHON_UCS4 )
    string( REGEX REPLACE "\\." "" ABI_TAG "cp${CMAKE_PYTHON_VERSION}m" )
  endif( CMAKE_PYTHON_UCS4 )

  set( MANYLNX_SUBDIR "${PYTHON_TAG}-${ABI_TAG}" )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )

#
# Executable file
#

if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  # brew package
  set( ${PACKAGE_NAME}_EXE_PATHS ${${PACKAGE_NAME}_EXE_PATHS} /usr/local/bin/ )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
  set( ${PACKAGE_NAME}_EXE_PATHS ${${PACKAGE_NAME}_EXE_PATHS} /opt/python/${MANYLNX_SUBDIR}/bin )
  set( ${PACKAGE_NAME}_EXE_PATHS ${${PACKAGE_NAME}_EXE_PATHS} /usr/bin )
  set( ${PACKAGE_NAME}_EXE_PATHS ${${PACKAGE_NAME}_EXE_PATHS} /usr/local/bin )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )

set( ${PACKAGE_NAME} ${PACKAGE_NAME}-NOTFOUND )
find_program( ${PACKAGE_NAME} python${CMAKE_PYTHON_VERSION}
                              ${${PACKAGE_NAME}_EXE_PATHS} )
mark_as_advanced( ${PACKAGE_NAME} ) 

#
# Header path
#

if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  # brew package
  set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /usr/local/opt/python${CMAKE_PYTHON_VERSION_MAJ}/Frameworks/Python.framework/Versions/${CMAKE_PYTHON_VERSION}/Headers )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
  set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /opt/python/${MANYLNX_SUBDIR}/include/python${CMAKE_PYTHON_VERSION} )
  set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /opt/python/${MANYLNX_SUBDIR}/include/python${CMAKE_PYTHON_VERSION}m )
  set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /usr/include/python${CMAKE_PYTHON_VERSION} )
  set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /usr/local/include/python${CMAKE_PYTHON_VERSION} )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )

set( ${PACKAGE_NAME}_INCLUDE_DIR ${PACKAGE_NAME}_INCLUDE_DIR-NOTFOUND )
find_path( ${PACKAGE_NAME}_INCLUDE_DIR NAMES Python.h
                                       PATHS ${${PACKAGE_NAME}_HEADER_PATHS}
                                       NO_DEFAULT_PATH
                                       DOC   "Directory containing ${PACKAGE_NAME}_HEADERS for ${PACKAGE_NAME} library" )
mark_as_advanced( ${PACKAGE_NAME}_INCLUDE_DIR )

#
# Library path
#

if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  # brew package
  set( ${PACKAGE_NAME}_LIBRARY_PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS} /usr/local/opt/python${CMAKE_PYTHON_VERSION_MAJ}/Frameworks/Python.framework/Versions/${CMAKE_PYTHON_VERSION}/lib )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
  set( ${PACKAGE_NAME}_LIBRARY_PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS} /opt/python/${MANYLNX_SUBDIR}/lib/python${PKG_VERSION_MAJ} )
  set( ${PACKAGE_NAME}_LIBRARY_PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS} /usr/lib )
  set( ${PACKAGE_NAME}_LIBRARY_PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS} /usr/lib/${CMAKE_SYSTEM_PROCESSOR}-linux-gnu )
  set( ${PACKAGE_NAME}_LIBRARY_PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS} /usr/local/lib )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )

set( ${PACKAGE_NAME}_LIBRARY ${PACKAGE_NAME}_LIBRARY-NOTFOUND ) # Force serarching each time
find_library( ${PACKAGE_NAME}_LIBRARY NAMES python${CMAKE_PYTHON_VERSION}
                                      PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS}
                                      NO_CMAKE_SYSTEM_PATH
                                      DOC   "${PACKAGE_NAME} library file" )
mark_as_advanced( ${PACKAGE_NAME}_LIBRARY )

#
# Debug library path
#

set( ${PACKAGE_NAME}_LIBRARY_DEBUG ${PACKAGE_NAME}_LIBRARY_DEBUG-NOTFOUND )
mark_as_advanced( ${PACKAGE_NAME}_LIBRARY_DEBUG )

