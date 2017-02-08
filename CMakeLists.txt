cmake_minimum_required( VERSION 2.8 )

project( pyreclab )

# Python version
set( PY_MAJOR_VERSION 2 CACHE STRING "Python version" )
find_package( PythonLibs ${PY_MAJOR_VERSION} REQUIRED )

# Compiler definitions
add_definitions( -DPY_MAJOR_VERSION=${PY_MAJOR_VERSION} )
add_definitions( -DNDEBUG )

add_compile_options( -fPIC )
add_compile_options( -O3 )

# header directories
include_directories( ${PROJECT_SOURCE_DIR} )
include_directories( dataio )
include_directories( datahandlers )
include_directories( algorithms )
include_directories( eval_metrics )

# External paths
if( ${PY_MAJOR_VERSION} EQUAL "2" )
   include_directories( ${PYTHON_INCLUDE_DIRS} )
   set( PYTHON_LIB_DIR /usr/local/lib/python2.7/ )
elseif( ${PY_MAJOR_VERSION} EQUAL "3" )
   include_directories( /usr/include/python3.5 )
   set( PYTHON_LIB_DIR /usr/local/lib/python3.5/ )
endif( ${PY_MAJOR_VERSION} EQUAL "2" )

add_subdirectory( dataio temp/dataio )
add_subdirectory( datahandlers temp/datahandlers )
add_subdirectory( algorithms temp/algorithms )
add_subdirectory( pyinterface temp/pyinterface )
add_subdirectory( eval_metrics temp/eval_metrics )

add_library( pylibrec SHARED
                      $<TARGET_OBJECTS:dataio>
                      $<TARGET_OBJECTS:datahandlers>
                      $<TARGET_OBJECTS:algorithms>
                      $<TARGET_OBJECTS:eval_metrics>
                      $<TARGET_OBJECTS:pyinterface> )

set_target_properties( pylibrec PROPERTIES PREFIX ""
                                           LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/pylibrec" )

install( TARGETS pylibrec DESTINATION ${PYTHON_LIB_DIR}/dist-packages/ )

#add_executable( tests/test_itemavg tests/test_itemavg.cpp )

#target_link_libraries( tests/test_itemavg pylibrec )
#target_link_libraries( tests/test_itemavg python2.7 )


