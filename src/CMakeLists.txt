cmake_minimum_required (VERSION 2.6)
project (Drone)

set (CMAKE_CXX_FLAGS "-g")

set(CMAKE_SOURCE_DIR $(CMAKE_CURRENT_LIST_DIR))

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/../bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}")

add_executable(Drone 
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_ctrl.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_ctrl.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_stat.h
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_cmd.c
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_cmd.h
		)
