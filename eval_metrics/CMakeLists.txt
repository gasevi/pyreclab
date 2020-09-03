set( SRCFILES RMSE.cpp
              MAE.cpp
              MAP.cpp
              NDCG.cpp
              Precision.cpp
              Recall.cpp
              AUC.cpp )

add_library( eval_metrics OBJECT ${SRCFILES} )

