set( SRCFILES AlgUserAvg.cpp
              AlgItemAvg.cpp
              AlgSlopeOne.cpp
              AlgUserBasedKnn.cpp
              AlgItemBasedKnn.cpp
              AlgFunkSvd.cpp
              AlgMostPopular.cpp )

add_library( algorithms OBJECT ${SRCFILES} )

