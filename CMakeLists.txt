cmake_minimum_required(VERSION 3.7)
project(Project6)

set(CMAKE_CXX_STANDARD 20)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads)
find_package(ODBC) 

include_directories(${Boost_INCLUDE_DIRS})
add_executable(Project6 main.cpp DatabaseManipulation.cpp)

# Link libraries
target_link_libraries(Project6 
    ${Boost_LIBRARIES} 
    Threads::Threads 
    ODBC::ODBC  # Link against the ODBC library
)
