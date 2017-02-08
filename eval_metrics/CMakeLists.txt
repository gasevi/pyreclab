set( SRCFILES RMSE.cpp
              MAE.cpp )

add_library( eval_metrics OBJECT ${SRCFILES} )

