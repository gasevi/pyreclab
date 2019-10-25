
#
# Preamble
#

set( PACKAGE_NAME PYTHON_NATIVE )

string( REPLACE "." ";" PY_VERSION_LIST ${CMAKE_PYTHON_VERSION} )
list( GET PY_VERSION_LIST 0 CMAKE_PYTHON_VERSION_MAJ )
list( GET PY_VERSION_LIST 1 CMAKE_PYTHON_VERSION_MIN )

#
# Executable file
#

set( ${PACKAGE_NAME}_EXE_PATHS ${${PACKAGE_NAME}_EXE_PATHS} /usr/bin )
set( ${PACKAGE_NAME}_EXE_PATHS ${${PACKAGE_NAME}_EXE_PATHS} /System/Library/Frameworks/Python.framework/Versions/2.7/bin/ )

set( ${PACKAGE_NAME} ${PACKAGE_NAME}-NOTFOUND )
find_program( ${PACKAGE_NAME} python${CMAKE_PYTHON_VERSION}
                              ${${PACKAGE_NAME}_EXE_PATHS} )
mark_as_advanced( ${PACKAGE_NAME} ) 

#
# Header path
#

set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /usr/include/python${CMAKE_PYTHON_VERSION} )
set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /System/Library/Frameworks/Python.framework/Versions/${CMAKE_PYTHON_VERSION}/include/python${CMAKE_PYTHON_VERSION} )

set( ${PACKAGE_NAME}_INCLUDE_DIR ${PACKAGE_NAME}_INCLUDE_DIR-NOTFOUND )
find_path( ${PACKAGE_NAME}_INCLUDE_DIR NAMES Python.h
                                       PATHS ${${PACKAGE_NAME}_HEADER_PATHS}
                                       NO_DEFAULT_PATH
                                       DOC   "Directory containing ${PACKAGE_NAME}_HEADERS for ${PACKAGE_NAME} library" )
mark_as_advanced( ${PACKAGE_NAME}_INCLUDE_DIR )

#
# Library path
#

set( ${PACKAGE_NAME}_LIBRARY_PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS} /usr/lib/ )
set( ${PACKAGE_NAME}_LIBRARY_PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS} /System/Library/Frameworks/Python.framework/Versions/${CMAKE_PYTHON_VERSION}/lib/ )

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

