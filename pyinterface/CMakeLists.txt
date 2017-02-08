set( SRCFILES PyMostPopular.cpp
              PyUserAvg.cpp
              PyItemAvg.cpp
              PyUserKnn.cpp
              PyItemKnn.cpp
              PySlopeOne.cpp
              PyFunkSvd.cpp
              pyinterface.cpp )

add_library( pyinterface OBJECT ${SRCFILES} )

