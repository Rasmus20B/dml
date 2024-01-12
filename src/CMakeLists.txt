

set(SOURCE_FILES 
  dml.cc
  dml.h
  data_structures/sparse_set.h
  data_structures/fill_array.h
  )
add_library(${PROJECT_NAME} ${SOURCE_FILES})
