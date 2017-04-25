cmake_minimum_required( VERSION 2.8 )

project( pyreclab )

set( PACKAGE_VERSION "0.1.2" )

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

add_library( pyreclab SHARED
                      $<TARGET_OBJECTS:dataio>
                      $<TARGET_OBJECTS:datahandlers>
                      $<TARGET_OBJECTS:algorithms>
                      $<TARGET_OBJECTS:eval_metrics>
                      $<TARGET_OBJECTS:pyinterface> )

set_target_properties( pyreclab PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/pyreclab" )

find_program( PYTHON "python" )
if( PYTHON )
   set( SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/pypackage/setup.py.in" )
   set( SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/pypackage/setup.py" )
   set( OUTPUT      "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp" )
   set( DEPS        "pyreclab" )

   configure_file( ${SETUP_PY_IN} ${SETUP_PY} )

   add_custom_command( OUTPUT ${OUTPUT}
                       COMMAND ${CMAKE_COMMAND} -E copy "pypackage/__init__.py" "${PROJECT_BINARY_DIR}/pyreclab/"
                       COMMAND ${PYTHON} ${SETUP_PY} bdist_egg
                       COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
                       DEPENDS ${DEPS} )

   add_custom_target( target ALL DEPENDS ${OUTPUT} )

   install( CODE "execute_process(COMMAND python pypackage/setup.py install)" )
endif()


