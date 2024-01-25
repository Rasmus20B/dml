

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PUBLIC
  FILE_SET CXX_MODULES FILES
  opcodes.cc
  dml.cc
  data_structures/sparse_set.cc
  data_structures/fill_array.cc
  )
