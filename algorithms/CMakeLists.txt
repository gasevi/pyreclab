set( SRCFILES RecSysAlgorithm.cpp
              AlgUserAvg.cpp
              AlgItemAvg.cpp
              AlgSlopeOne.cpp
              AlgUserBasedKnn.cpp
              AlgItemBasedKnn.cpp
              AlgFunkSvd.cpp
              AlgMostPopular.cpp
              AlgFactory.cpp )

add_library( algorithms OBJECT ${SRCFILES} )

