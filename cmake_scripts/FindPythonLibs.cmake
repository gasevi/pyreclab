set( PACKAGE_NAME PYTHON )

# Executable file
set( ${PACKAGE_NAME} ${PACKAGE_NAME}-NOTFOUND )
find_program( ${PACKAGE_NAME} python${CMAKE_PYTHON_VERSION}
                              /usr/bin
                              /usr/local/bin )
mark_as_advanced( ${PACKAGE_NAME} ) 

# Header path
set( ${PACKAGE_NAME}_INCLUDE_DIR ${PACKAGE_NAME}_INCLUDE_DIR-NOTFOUND )
find_path( ${PACKAGE_NAME}_INCLUDE_DIR NAMES Python.h
                                       PATHS /usr/include/python${CMAKE_PYTHON_VERSION}
                                             /usr/local/include/python${CMAKE_PYTHON_VERSION}
                                       DOC   "Directory containing ${PACKAGE_NAME}_HEADERS for ${PACKAGE_NAME} library" )
mark_as_advanced( ${PACKAGE_NAME}_INCLUDE_DIR )

# Library path
set( ${PACKAGE_NAME}_LIBRARY ${PACKAGE_NAME}_LIBRARY-NOTFOUND )
find_library( ${PACKAGE_NAME}_LIBRARY NAMES python${CMAKE_PYTHON_VERSION}
                                            python${CMAKE_PYTHON_VERSION}m
                                      PATHS /usr/lib
                                            /usr/local/lib
                                            /usr/lib/i386-linux-gnu
                                            /usr/lib/x86_64-linux-gnu
                                      DOC   "${PACKAGE_NAME} library file" )
mark_as_advanced( ${PACKAGE_NAME}_LIBRARY )

# Debug library path
set( ${PACKAGE_NAME}_LIBRARY_DEBUG ${PACKAGE_NAME}_LIBRARY_DEBUG-NOTFOUND )
mark_as_advanced( ${PACKAGE_NAME}_LIBRARY_DEBUG )

