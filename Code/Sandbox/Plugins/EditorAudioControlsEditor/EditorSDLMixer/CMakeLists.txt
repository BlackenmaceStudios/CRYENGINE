#START-FILE-LIST
# File List auto generated by waf2cmake.py, do not modify manually.

start_sources()
# -- editorsdlmixer.waf_files --
sources_platform(ALL)
add_sources("EditorSDLMixer_uber_0.cpp"
	SOURCE_GROUP "Root"
		"Impl.cpp"
		"Impl.h"
		"Item.cpp"
		"Item.h"
		"main.cpp"
		"ProjectLoader.cpp"
		"ProjectLoader.h"
		"Settings.cpp"
		"Settings.h"
		"StdAfx.cpp"
		"StdAfx.h"
	SOURCE_GROUP "Connections"
		"BaseConnection.cpp"
		"BaseConnection.h"
		"EventConnection.cpp"
		"EventConnection.h"
		"ParameterConnection.cpp"
		"ParameterConnection.h"
		"StateConnection.cpp"
		"StateConnection.h"
)

end_sources()
#END-FILE-LIST

CryEditorPlugin(EditorCryAudioImplSDLMixer SOLUTION_FOLDER "Sandbox/Plugins/EditorAudioControlsEditor/Impl" OUTDIR EditorPlugins/ace)
target_include_directories(${THIS_PROJECT} PRIVATE ../common "${CRYENGINE_DIR}/Code/CryEngine/CryAudioSystem/implementations")
target_compile_definitions(${THIS_PROJECT} PRIVATE -DNOT_USE_CRY_MEMORY_MANAGER -DPLUGIN_EXPORTS -DUSE_BACKWARDS_COMPATIBILITY)

