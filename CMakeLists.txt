# CMakeList.txt : CMake project for SQLSimulatorCMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.14)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SQLSimulatorCMake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
	asiocmake
	GIT_REPOSITORY "https://github.com/OlivierLDff/asio.cmake"
	GIT_TAG "main"
)
FetchContent_MakeAvailable(asiocmake)
# Add source to this project's executable.
add_executable (SQLSimulatorCMake
	src/main.cpp
	src/db/BazaDeDate.cpp
	src/db/Coloana.cpp
	src/db/Interogare.cpp
	src/db/Rand.cpp
	src/db/Tabela.cpp
	src/db/Conditie.cpp
    src/common/ResultSet.cpp
	src/networking/Server.cpp
	src/networking/Session.cpp
)

add_executable(client 
	src/networking/Client.cpp
	src/common/ResultSet.cpp
	src/db/Rand.cpp 
	src/db/Coloana.cpp 
	src/db/Tabela.cpp
)

find_package(Threads REQUIRED)
target_include_directories(SQLSimulatorCMake PRIVATE include)
target_link_libraries(SQLSimulatorCMake PRIVATE asio::asio Threads::Threads)
target_include_directories(client PRIVATE include)
target_link_libraries(client PRIVATE asio::asio Threads::Threads)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SQLSimulatorCMake PROPERTY CXX_STANDARD 20)
  if(MSVC)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
  endif()
endif()

# TODO: Add tests and install targets if needed.
