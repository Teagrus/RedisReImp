add_executable(Client)
add_executable(GetCommands)
# add_library(Client SHARED)


target_sources(Client PUBLIC ClientMain.cpp)
target_sources(GetCommands PUBLIC GetCommands.cpp)

target_include_directories(Client PUBLIC ${PROJECT_SOURCE_DIR}/include/Client 
                                         ${PROJECT_SOURCE_DIR}/include/General)

target_link_libraries(Client PUBLIC General)
target_link_libraries(GetCommands PUBLIC General)