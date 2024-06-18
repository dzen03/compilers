cmake_minimum_required(VERSION 3.27)
project(compilers)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

set(SOURCE_FILES ast.c)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.c)
FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

add_executable(compilers
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Lexer_OUTPUTS}
        ${SOURCE_FILES}
)

target_include_directories(compilers PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(compilers PRIVATE ${CMAKE_SOURCE_DIR})

