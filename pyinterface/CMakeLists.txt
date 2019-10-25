set( SRCFILES PyMostPopular.cpp
              PyUserAvg.cpp
              PyItemAvg.cpp
              PyUserKnn.cpp
              PyItemKnn.cpp
              PySlopeOne.cpp
              PyFunkSvd.cpp
              PyIFAls.cpp
              PyIFAlsConjugateGradient.cpp
              SigHandler.cpp
              pyinterface.cpp )

set( PYINTERFACE_NAME pyinterface${LIBNAME_SUFFIX} )

add_library( ${PYINTERFACE_NAME} OBJECT ${SRCFILES} )

target_compile_definitions( ${PYINTERFACE_NAME} PUBLIC "-DPYRECLAB_LIBNAME=libpyreclab${LIBNAME_SUFFIX}" )

target_include_directories( ${PYINTERFACE_NAME} PUBLIC ${PYTHON_INCLUDE_DIR} )
target_include_directories( ${PYINTERFACE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/algorithms )
target_include_directories( ${PYINTERFACE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/datahandlers )
target_include_directories( ${PYINTERFACE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/dataio )
target_include_directories( ${PYINTERFACE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/eval_metrics )

