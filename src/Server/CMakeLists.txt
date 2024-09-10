add_executable(Server)
target_sources(Server PRIVATE CoreDataManager.cpp Executors.cpp ServerMain.cpp)



target_include_directories(Server PUBLIC ${PROJECT_SOURCE_DIR}/include/NetCore 
                             ${PROJECT_SOURCE_DIR}/include/General
                             ${PROJECT_SOURCE_DIR}/include/Server)


target_link_libraries(Server PUBLIC General)
target_link_libraries(Server PUBLIC NetCore)

