set(
  CLIENT_SOURCES
  
  agent.c
  agent.h
  api.c
  arc4.c
  arc4.h
  auth.c
  auth.h
  bag.h
  character.h
  chat.c
  chat.h
  client.c
  client.h
  command.c
  command.h
  dialog.c
  dialog.h
  effect.c
  effect.h
  error.c
  error.h
  event.c
  event.h
  friend.c
  friend.h
  game.c
  game.h
  guild.c
  guild.h
  hero.c
  instance.c
  instance.h
  inventory.h
  item.c
  item.h
  kstr.c
  kstr.h
  main.c
  merchant.c
  merchant.h
  network.c
  network.h
  object.c
  object.h
  opcodes.h
  packets.c
  packets.h
  party.c
  party.h
  player.c
  player.h
  plugins.c
  plugins.h
  portal.c
  portal.h
  quest.c
  quest.h
  skill.c
  skill.h
  title.c
  title.h
  trade.c
  trade.h
  world.c
  world.h
)

set_source_files_properties(
  ${COMMON_SOURCES}
  PROPERTIES HEADER_FILE_ONLY TRUE
)

add_executable(
  client

  build.c
  ${COMMON_SOURCES}
)

target_link_libraries(client PRIVATE common mbedcrypto)

target_include_directories(
  client

  PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)

