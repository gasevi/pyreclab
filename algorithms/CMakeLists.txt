set( SRCFILES AlgUserAvg.cpp
              AlgItemAvg.cpp
              AlgSlopeOne.cpp
              AlgUserBasedKnn.cpp
              AlgItemBasedKnn.cpp
              AlgFunkSvd.cpp
              AlgIFAls.cpp
              AlgIFAlsConjugateGradient.cpp
              AlgMostPopular.cpp
              AlgBprMf.cpp )

add_library( algorithms OBJECT ${SRCFILES} )

target_include_directories( algorithms PUBLIC ${PROJECT_SOURCE_DIR}/datahandlers )
target_include_directories( algorithms PUBLIC ${PROJECT_SOURCE_DIR}/dataio )

