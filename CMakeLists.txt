cmake_minimum_required(VERSION 3.7)
project(Project6)

set(CMAKE_CXX_STANDARD 20)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads)

find_package(SQLite3 REQUIRED)

include_directories(${SQLite3_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIRS})
add_executable(Project6 main.cpp DatabaseManipulation.cpp)

target_link_libraries(Project6 ${SQLITE3_LIBRARIES})
target_link_libraries(Project6 ${Boost_LIBRARIES} ${SQLite3_LIBRARIES} Threads::Threads)