set( SRCFILES RMSE.cpp
              MAE.cpp
              MAP.cpp
              NDCG.cpp )

add_library( eval_metrics OBJECT ${SRCFILES} )

