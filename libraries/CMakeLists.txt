cmake_minimum_required(VERSION 3.2)
add_subdirectory(fgsig)
add_subdirectory(sigc++)

set( WFL_TESTING_ENABLED OFF CACHE BOOL "" )
add_subdirectory(wfl/cmake)
