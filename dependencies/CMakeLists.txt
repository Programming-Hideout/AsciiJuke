add_library(arena_allocator STATIC "arena-allocator/arena.h")
set_target_properties(arena_allocator PROPERTIES LINKER_LANGUAGE C)
add_library(miniaudio STATIC "miniaudio/miniaudio.h")
set_target_properties(miniaudio PROPERTIES LINKER_LANGUAGE C)
