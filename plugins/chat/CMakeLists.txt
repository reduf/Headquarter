add_library(
  chat

  SHARED
  build_chat.c
)

target_link_libraries(chat PRIVATE common client)
