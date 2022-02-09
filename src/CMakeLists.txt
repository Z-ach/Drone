cmake_minimum_required (VERSION 3.5)
option(HW_BUILD "Use tutorial provided math implementation" OFF)


set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project (Drone)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_C_FLAGS -pthread)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
#set (LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}")

configure_file(${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_hw.h.in ${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_hw.h)

if(HW_BUILD)
  #add_subdirectory(/home/debian/librobotcontrol/library/include)
  link_directories(home/debian/librobotcontrol/library/include/lib)
  #list(APPEND EXTRA_LIBS /home/debian/librobotcontrol/library/include/lib//librobotcontrol.so.1)
  list(APPEND EXTRA_LIBS librobotcontrol.so.1)
  #list(APPEND EXTRA_INCLUDES "/home/debian/librobotcontrol/library/include")
endif()



add_executable(Drone
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_ctrl.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_ctrl.h
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_hw.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_hw.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_loop.c
		${PROJECT_SOURCE_DIR}/ctrlmgr/ctrlmgr_loop.h
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
		${PROJECT_SOURCE_DIR}/netmgr/netmgr_handler.h
		${PROJECT_SOURCE_DIR}/logmgr/logmgr.h)


target_link_libraries(Drone ${EXTRA_LIBS})
target_include_directories(Drone PUBLIC ${EXTRA_INCLUDES})
