include_directories(../engine/include ./)

set(DEMO_SRCS
    DemoApp.cpp
    main.cpp
   )

add_executable(demo ${DEMO_SRCS})
target_link_libraries(demo engine SDL)
