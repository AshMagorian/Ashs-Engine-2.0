cmake_minimum_required(VERSION 2.6)
project(MYENGINE)

include_directories(
src
contrib
)

if(MSVC)
  include_directories(
    contrib/msvc-windows-x86/glew/include
    contrib/msvc-windows-x86/SDL2/include
    contrib/msvc-windows-x86/openal-soft/include
    contrib/msvc-windows-x86/ogg/include
    contrib/msvc-windows-x86/vorbis/include
  )

  link_directories(
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/SDL2/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/glew/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/openal-soft/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/ogg/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/vorbis/lib
  )
endif()

if(MSVC)
  add_definitions(-DSDL_MAIN_HANDLED)
endif()

add_library(myEngine
	src/myEngine/myEngine.h
	src/myEngine/Component.cpp
	src/myEngine/Application.cpp
	src/myEngine/Entity.cpp
	src/myEngine/Component.h
	src/myEngine/Application.h
	src/myEngine/Entity.h
	src/myEngine/VertexBuffer.cpp
	src/myEngine/VertexArray.cpp
	src/myEngine/VertexBuffer.h
	src/myEngine/VertexArray.h
	src/myEngine/ShaderProgram.h
	src/myEngine/ShaderProgram.cpp
	src/myEngine/Renderer.h
	src/myEngine/Renderer.cpp
	src/myEngine/Transform.h
	src/myEngine/Transform.cpp
	src/myEngine/stb_image.h
	src/myEngine/stb_image.cpp
	src/myEngine/Texture.h
	src/myEngine/Texture.cpp
	src/myEngine/Camera.h
	src/myEngine/Camera.cpp
	src/myEngine/Environment.h
	src/myEngine/Environment.cpp
	src/myEngine/Input.h
	src/myEngine/Input.cpp
	src/myEngine/Resource.h
	src/myEngine/Resources.h
	src/myEngine/Resources.cpp
	src/myEngine/Exception.h
	src/myEngine/Exception.cpp
	src/myEngine/Sound.h
	src/myEngine/Sound.cpp
	src/myEngine/SoundComponent.h
	src/myEngine/SoundComponent.cpp
	src/myEngine/BoxCollider.h
	src/myEngine/BoxCollider.cpp
	src/myEngine/Material.h
	src/myEngine/Material.cpp
	src/myEngine/Lights.h
	src/myEngine/Lights.cpp
	src/myEngine/PointLight.h
	src/myEngine/SpotLight.h
)	

if(MSVC)
  target_link_libraries(myEngine
    SDL2 glew32 opengl32 OpenAL32 ogg vorbis vorbisfile
  )
else()
  target_link_libraries(myEngine
    SDL2 GLU GLEW GL AL ogg vorbis vorbisfile
  )
endif()

add_executable(game
	src/game/main.cpp
	src/game/Camera_FirstPerson.h
	src/game/Camera_FirstPerson.cpp
	src/game/SoundToggle.h
	src/game/SoundToggle.cpp
	src/game/SpinComponent.cpp
	src/game/SpinComponent.h
)

target_link_libraries(game myEngine)

if(MSVC)
  add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/SDL2/bin/SDL2.dll"
      "$<TARGET_FILE_DIR:game>"
  )

  add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/glew/bin/glew32.dll"
      "$<TARGET_FILE_DIR:game>"
  )

  add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/openal-soft/bin/OpenAL32.dll"
      "$<TARGET_FILE_DIR:game>"
  )
endif()