cmake_minimum_required (VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project (Drone)

#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
#set (LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}")

add_executable(Drone
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_ctrl.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_ctrl.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_pid.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_pid.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_stat.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_state.h
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_cmd.c
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_cmd.h
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_handler.c
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_handler.h
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_buffer.c
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_buffer.h
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_crc.c
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_crc.h
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_pub.h
		${PROJECT_SOURCE_DIR}/netmgr/netmgr_handler.c
		${PROJECT_SOURCE_DIR}/netmgr/netmgr_handler.h)


#target_link_libraries(Drone Threads)
