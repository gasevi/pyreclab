
#
# Preamble
#

set( PACKAGE_NAME PYTHON )

string( REGEX REPLACE "\\." "" MANYLNX_SUBDIR "${CMAKE_PYTHON_VERSION}" )
set( MANYLNX_SUBDIR "cp${MANYLNX_SUBDIR}-cp${MANYLNX_SUBDIR}m" )

#
# Executable file
#

set( ${PACKAGE_NAME}_EXE_PATHS ${${PACKAGE_NAME}_EXE_PATHS} /opt/python/${MANYLNX_SUBDIR}/bin )
set( ${PACKAGE_NAME}_EXE_PATHS ${${PACKAGE_NAME}_EXE_PATHS} /usr/bin )
set( ${PACKAGE_NAME}_EXE_PATHS ${${PACKAGE_NAME}_EXE_PATHS} /usr/local/bin )

set( ${PACKAGE_NAME} ${PACKAGE_NAME}-NOTFOUND )
find_program( ${PACKAGE_NAME} python${CMAKE_PYTHON_VERSION}
                              ${${PACKAGE_NAME}_EXE_PATHS} )
mark_as_advanced( ${PACKAGE_NAME} ) 

#
# Header path
#

set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /usr/include/python${CMAKE_PYTHON_VERSION} )
set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /usr/local/include/python${CMAKE_PYTHON_VERSION} )
set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /opt/python/${MANYLNX_SUBDIR}/include/python${CMAKE_PYTHON_VERSION} )
set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /opt/python/${MANYLNX_SUBDIR}/include/python${CMAKE_PYTHON_VERSION}m )
if( APPLE )

   execute_process( COMMAND python${CMAKE_PYTHON_VERSION} -V
                    OUTPUT_VARIABLE PKG_VERSION
                    ERROR_VARIABLE PKG_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE )
   string( REPLACE "Python " "" PKG_VERSION ${PKG_VERSION} )
   string( REPLACE "." ";" VERSION_LIST ${PKG_VERSION} )
   list( GET VERSION_LIST 0 PKG_VERSION_MAJ )
   list( GET VERSION_LIST 1 PKG_VERSION_MIN )
   list( GET VERSION_LIST 2 PKG_VERSION_REL )

   set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /usr/local/Cellar/python${PKG_VERSION_MAJ}/${PKG_VERSION}/Frameworks/Python.framework/Versions/${PKG_VERSION_MAJ}.${PKG_VERSION_MIN}/include/python${PKG_VERSION_MAJ}.${PKG_VERSION_MIN}m )
   set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} /usr/local/Cellar/python${PKG_VERSION_MAJ}/${PKG_VERSION}/Frameworks/Python.framework/Versions/${PKG_VERSION_MAJ}.${PKG_VERSION_MIN}/include/python${PKG_VERSION_MAJ}.${PKG_VERSION_MIN} )

endif( APPLE )

if( WIN32 )
   set( ${PACKAGE_NAME}_HEADER_PATHS ${${PACKAGE_NAME}_HEADER_PATHS} "c:/Archivos de programa/Anaconda2/include" )
endif( WIN32 )

set( ${PACKAGE_NAME}_INCLUDE_DIR ${PACKAGE_NAME}_INCLUDE_DIR-NOTFOUND )
find_path( ${PACKAGE_NAME}_INCLUDE_DIR NAMES Python.h
                                       PATHS ${${PACKAGE_NAME}_HEADER_PATHS}
                                       NO_DEFAULT_PATH
                                       DOC   "Directory containing ${PACKAGE_NAME}_HEADERS for ${PACKAGE_NAME} library" )
mark_as_advanced( ${PACKAGE_NAME}_INCLUDE_DIR )

#
# Library path
#

set( ${PACKAGE_NAME}_LIBRARY_PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS} /usr/lib )
set( ${PACKAGE_NAME}_LIBRARY_PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS} /usr/local/lib )
set( ${PACKAGE_NAME}_LIBRARY_PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS} /usr/lib/${CMAKE_SYSTEM_PROCESSOR}-linux-gnu )
set( ${PACKAGE_NAME}_LIBRARY_PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS} /opt/python/${MANYLNX_SUBDIR}/lib/python${PKG_VERSION_MAJ} )

if( APPLE )

   set( ${PACKAGE_NAME}_LIBRARY_PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS} /usr/local/Cellar/python${PKG_VERSION_MAJ}/${PKG_VERSION}/Frameworks/Python.framework/Versions/${PKG_VERSION_MAJ}.${PKG_VERSION_MIN}/lib )

endif( APPLE )

set( ${PACKAGE_NAME}_LIBRARY ${PACKAGE_NAME}_LIBRARY-NOTFOUND )
find_library( ${PACKAGE_NAME}_LIBRARY NAMES python${CMAKE_PYTHON_VERSION}
                                      PATHS ${${PACKAGE_NAME}_LIBRARY_PATHS}
                                      DOC   "${PACKAGE_NAME} library file" )
mark_as_advanced( ${PACKAGE_NAME}_LIBRARY )

#
# Debug library path
#

set( ${PACKAGE_NAME}_LIBRARY_DEBUG ${PACKAGE_NAME}_LIBRARY_DEBUG-NOTFOUND )
mark_as_advanced( ${PACKAGE_NAME}_LIBRARY_DEBUG )

