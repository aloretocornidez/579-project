set(PROJECT_NAME "campusNavigatorBuild")


add_executable(
  ${PROJECT_NAME}
  main.cpp
  AStar.h
  AStar.cpp
  CommonUtilities.h
  CommonUtilities.cpp
)

# adding lib directory for
set(SUB_DIR "db")
add_subdirectory(${SUB_DIR})
target_link_libraries( ${PROJECT_NAME} PRIVATE ${SUB_DIR})

set(SUB_DIR "graph")
add_subdirectory(${SUB_DIR})
target_link_libraries( ${PROJECT_NAME} PRIVATE ${SUB_DIR})
