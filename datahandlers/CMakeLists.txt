set( SRCFILES MaxHeap.cpp
              DataFrame.cpp
              DenseMatrix.cpp
              SymmMatrix.cpp )

add_library( datahandlers OBJECT ${SRCFILES} )

target_include_directories( datahandlers PUBLIC ${PROJECT_SOURCE_DIR}/dataio )

