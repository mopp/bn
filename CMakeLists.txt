project( bn )
cmake_minimum_required( VERSION 2.8 )

set( CURSES_NEED_NCURSES TRUE )
find_package( Curses )

add_definitions(
  -std=c11
  )

add_executable( bn bn.c )
target_link_libraries( bn ${CURSES_NCURSES_LIBRARY} )

install( TARGETS bn DESTINATION bin )