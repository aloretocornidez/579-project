
# adds this directory to the cmake project
add_library(db "")


# adds the source files to the directory when compiling
target_sources(db
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/PathDatabase.cpp
  ${CMAKE_CURRENT_LIST_DIR}/CatTranDatabase.cpp
  ${CMAKE_CURRENT_LIST_DIR}/LocationDatabase.cpp 
  ${CMAKE_CURRENT_LIST_DIR}/TrafficDatabase.cpp
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/PathDatabase.h
  ${CMAKE_CURRENT_LIST_DIR}/CatTranDatabase.h
  ${CMAKE_CURRENT_LIST_DIR}/LocationDatabase.h
  ${CMAKE_CURRENT_LIST_DIR}/TrafficDatabase.h
  )

# adds the directory to the linker step of the compilation process
target_include_directories(db
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

