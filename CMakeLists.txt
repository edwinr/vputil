project(vputil)

add_library(vputil
	vputil/handlers.h
	vputil/handlers.cpp
	vputil/scheduler.h
	vputil/scheduler.cpp
)

target_include_directories(vputil INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(vputil PUBLIC
	vpsdk::vpsdk
)
