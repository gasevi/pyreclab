cmake_minimum_required( VERSION 2.8 )

project( pyreclab )

#
# Project version
#

set( MAJOR_VERSION "0" )
set( MINOR_VERSION "1" )
set( RELEASE_VERSION "14" )
set( PACKAGE_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${RELEASE_VERSION}" )

# Set CMake macros
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                       ${PROJECT_SOURCE_DIR}/cmake_scripts )

#
# Compilation options
#

set( PRLOPT_BUILD_WHEEL OFF CACHE BOOL "Build wheel package" )
set( CMAKE_PYTHON_VERSION 2.7 CACHE STRING "Python version" )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
  if( ${CMAKE_PYTHON_VERSION} VERSION_LESS "3.3" ) # Before Python 3.3, there were two ABI-incompatible ways of building CPython
    set( CMAKE_PYTHON_UCS4 ON CACHE BOOL "Enable unicode ucs4" )
  else( ${CMAKE_PYTHON_VERSION} VERSION_LESS "3.3" )
    unset( CMAKE_PYTHON_UCS4 CACHE )
  endif( ${CMAKE_PYTHON_VERSION} VERSION_LESS "3.3" )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )

# Compiler definitions
add_definitions( -DNDEBUG )

add_compile_options( -fPIC )
add_compile_options( -O3 )
add_compile_options( -Wall )

find_package( PythonLibs "${CMAKE_PYTHON_VERSION}"
              REQUIRED )

#
# Library building
#

add_subdirectory( dataio temp/dataio )
add_subdirectory( datahandlers temp/datahandlers )
add_subdirectory( algorithms temp/algorithms )
add_subdirectory( pyinterface temp/pyinterface )
add_subdirectory( eval_metrics temp/eval_metrics )
add_subdirectory( tools temp/tools )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  set( CMAKE_MACOSX_RPATH 0 )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )

add_library( pyreclab SHARED
                      $<TARGET_OBJECTS:dataio>
                      $<TARGET_OBJECTS:datahandlers>
                      $<TARGET_OBJECTS:tools>
                      $<TARGET_OBJECTS:algorithms>
                      $<TARGET_OBJECTS:eval_metrics>
                      $<TARGET_OBJECTS:pyinterface> )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  target_link_libraries( pyreclab ${PYTHON_LIBRARY} )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )

set_target_properties( pyreclab PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/pyreclab"
                                           SUFFIX .so
                                           SOVERSION "${MAJOR_VERSION}"
                                           VERSION "${PACKAGE_VERSION}" )

# Library compilation for native version of Python 2.7
if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND ${CMAKE_PYTHON_VERSION} VERSION_EQUAL "2.7" )

  find_package( Python27MacOSXNativeLibs "${CMAKE_PYTHON_VERSION}"
                REQUIRED )

  set( LIBNAME_SUFFIX "_native_py" )
  add_subdirectory( pyinterface temp/pyinterface${LIBNAME_SUFFIX} )

  add_library( pyreclab${LIBNAME_SUFFIX} SHARED
                                         $<TARGET_OBJECTS:dataio>
                                         $<TARGET_OBJECTS:datahandlers>
                                         $<TARGET_OBJECTS:tools>
                                         $<TARGET_OBJECTS:algorithms>
                                         $<TARGET_OBJECTS:eval_metrics>
                                         $<TARGET_OBJECTS:pyinterface${LIBNAME_SUFFIX}> )

  target_link_libraries( pyreclab${LIBNAME_SUFFIX} ${PYTHON_NATIVE_LIBRARY} )

  set_target_properties( pyreclab${LIBNAME_SUFFIX} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/pyreclab"
                                                   SUFFIX .so
                                                   SOVERSION "${MAJOR_VERSION}"
                                                   VERSION "${PACKAGE_VERSION}" )

endif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND ${CMAKE_PYTHON_VERSION} VERSION_EQUAL "2.7" )

#
# Python packaging
#

if( PYTHON )

   set( CMAKE_LIBRARY_LIST "'libpyreclab.so'" )
   if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND ${CMAKE_PYTHON_VERSION} VERSION_EQUAL "2.7" )
      set( CMAKE_LIBRARY_LIST "${CMAKE_LIBRARY_LIST}, 'libpyreclab${LIBNAME_SUFFIX}.so'" )
      set( PACKAGING_DEPS "pyreclab" "pyreclab_native_py" )
   else( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND ${CMAKE_PYTHON_VERSION} VERSION_EQUAL "2.7" )
      set( PACKAGING_DEPS "pyreclab" )
   endif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND ${CMAKE_PYTHON_VERSION} VERSION_EQUAL "2.7" )

   set( SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/pypackage/setup.py.in" )
   set( SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/pypackage/setup.py" )
   set( OUTPUT      "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp" )
   set( DEPS        "${PACKAGING_DEPS}" )

   configure_file( ${SETUP_PY_IN} ${SETUP_PY} )

   install( CODE "execute_process(COMMAND ${PYTHON} pypackage/setup.py install)" )

   if( ${PRLOPT_BUILD_WHEEL} )

      if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
         add_custom_command( OUTPUT ${OUTPUT}
                             COMMAND ${CMAKE_COMMAND} -E copy "pypackage/__init__.py" "${PROJECT_BINARY_DIR}/pyreclab/"
                             COMMAND ${PYTHON} ${SETUP_PY} bdist_wheel "--plat-name" manylinux1_${CMAKE_SYSTEM_PROCESSOR} # PEP 513
                             COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
                             DEPENDS ${DEPS} )

         add_custom_target( pypackage ALL
                                      DEPENDS ${OUTPUT} )
      endif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )

      if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
         execute_process( COMMAND python -c "import distutils.util; print(distutils.util.get_platform())" OUTPUT_VARIABLE PY_PLATFORM )
         string( REGEX REPLACE "\\." "_" PY_PLATFORM "${PY_PLATFORM}" )
         string( REGEX REPLACE "-" "_" PY_PLATFORM "${PY_PLATFORM}" )

         add_custom_command( OUTPUT ${OUTPUT}
                             COMMAND ${CMAKE_COMMAND} -E copy "pypackage/__init__.py" "${PROJECT_BINARY_DIR}/pyreclab/"
                             COMMAND ${PYTHON} ${SETUP_PY} bdist_wheel "--plat-name" ${PY_PLATFORM}
                             COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
                             DEPENDS ${DEPS} )

         add_custom_target( pypackage ALL
                                      DEPENDS ${OUTPUT} )
      endif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )

   endif( ${PRLOPT_BUILD_WHEEL})

endif( PYTHON )

