set( SRCFILES MaxHeap.cpp
              SparseRow.cpp
              SparseColumn.cpp
              SparseMatrix.cpp
              RatingMatrix.cpp
              DataFrame.cpp )

add_library( datahandlers OBJECT ${SRCFILES} )

