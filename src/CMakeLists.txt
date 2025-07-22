cmake_minimum_required (VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project (Drone)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_C_FLAGS "-pthread -ggdb")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}")

list(APPEND EXTRA_LIBS librobotcontrol.so.1)

add_executable(Drone
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_ctrl.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_ctrl.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_msr.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_msr.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_pid.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_pid.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_hw.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_hw.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_hw_build.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_loop.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_loop.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_stat.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_state.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_bat.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_bat.h
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_cmd.c
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_cmd.h
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_handler.c
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_handler.h
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_buffer.c
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_buffer.h
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_crc.c
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_crc.h
		${PROJECT_SOURCE_DIR}/cmdmgr/cmdmgr_pub.h
		${PROJECT_SOURCE_DIR}/iomgr/netmgr_handler.c
		${PROJECT_SOURCE_DIR}/iomgr/netmgr_handler.h
		${PROJECT_SOURCE_DIR}/iomgr/serializer.c
		${PROJECT_SOURCE_DIR}/iomgr/serializer.h
		${PROJECT_SOURCE_DIR}/iomgr/confmgr.c
		${PROJECT_SOURCE_DIR}/iomgr/confmgr.h
		${PROJECT_SOURCE_DIR}/iomgr/logmgr.h)


target_link_libraries(Drone ${EXTRA_LIBS})
#target_include_directories(Drone PUBLIC ${EXTRA_INCLUDES})
