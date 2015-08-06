include_directories(../lib/)
add_executable(dump_resolutions dump_resolutions.cpp)
target_link_libraries(dump_resolutions qtcamera)
install(TARGETS dump_resolutions DESTINATION bin)
