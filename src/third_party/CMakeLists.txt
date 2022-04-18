#----------------------------------------------------------------
# overwrite install() with empty macro to supress cmake errors in subdirectories
macro (install)
endmacro ()
#----------------------------------------------------------------
if(USE_EXTERNAL_DEPENDENCIES)
    message("USE_EXTERNAL_DEPENDENCIES ignored, sorry author was too lazy =(")
endif()
set(THIRDPARTY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/third_party)
#----------------------------------------------------------------
#glfw
set(GLFW3_PATH "${THIRDPARTY_SOURCE_DIR}/glfw3" CACHE INTERNAL "" FORCE) 
add_subdirectory(${GLFW3_PATH} "${GLFW3_PATH}/bin")
set(GLFW3_LIBRARY 
	"glfw")
set(GLFW3_INCLUDE_DIR 
	"${GLFW3_PATH}/include"
	CACHE INTERNAL "" FORCE)
#----------------------------------------------------------------
#gl3w - OpenGL
set(GL3W_PATH "${THIRDPARTY_SOURCE_DIR}/gl3w" CACHE INTERNAL "" FORCE) 
set(GL3W_SOURCES_C 
	"${GL3W_PATH}/src/gl3w.c"
)

set(GL3W_INCLUDE_DIR
	"${GL3W_PATH}/include"
)
#----------------------------------------------------------------
#stb-image / stb-image-write
set(STB_PATH "${THIRDPARTY_SOURCE_DIR}/stb" CACHE INTERNAL "" FORCE) 
set(STB_INCLUDE_DIR
	"${STB_PATH}"
)
#----------------------------------------------------------------
#imgui
set(IMGUI_PATH "${THIRDPARTY_SOURCE_DIR}/imgui" CACHE INTERNAL "" FORCE) 
set(IMGUI_SOURCES_CPP
	"${IMGUI_PATH}/imgui.cpp"
	"${IMGUI_PATH}/imgui_demo.cpp"
	"${IMGUI_PATH}/imgui_draw.cpp"
	"${IMGUI_PATH}/imgui_tables.cpp"
	"${IMGUI_PATH}/imgui_widgets.cpp"
	"${IMGUI_PATH}/backends/imgui_impl_glfw.cpp"
	"${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp"
)
set(IMGUI_INCLUDE_DIR
	"${IMGUI_PATH}"
	"${IMGUI_PATH}/backends"
)
#----------------------------------------------------------------
#json
set(JSON_PATH "${THIRDPARTY_SOURCE_DIR}/json" CACHE INTERNAL "" FORCE) 
set(JSON_INCLUDE_DIR
	"${JSON_PATH}/include"
)
#----------------------------------------------------------------

#concat all data
set(EXT_SOURCES_CPP 
	${EXT_SOURCES_CPP}
	${IMGUI_SOURCES_CPP}
)

set(EXT_SOURCES_C 
	${EXT_SOURCES_C}
	${GL3W_SOURCES_C}
)

set(EXT_INCLUDE_DIR 
	${EXT_INCLUDE_DIR}
	${GLFW3_INCLUDE_DIR} 
	${GL3W_INCLUDE_DIR} 
	${STB_INCLUDE_DIR}
	${IMGUI_INCLUDE_DIR}
	${JSON_INCLUDE_DIR}
)

set(EXT_LIBRARY 
	${EXT_LIBRARY}
	${GLFW3_LIBRARY}
)
#----------------------------------------------------------------
message(STATUS "EXT_SOURCES_CPP = ${EXT_SOURCES_CPP}")
message(STATUS "EXT_SOURCES_C = ${EXT_SOURCES_C}")
message(STATUS "EXT_INCLUDE_DIR = ${EXT_INCLUDE_DIR}")
message(STATUS "EXT_LIBRARY = ${EXT_LIBRARY}")
#----------------------------------------------------------------
# restoring install back
macro (install)
  _install(${ARGV})
endmacro(install)