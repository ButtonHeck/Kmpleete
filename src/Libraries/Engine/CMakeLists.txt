## ~/src/Libraries/Engine/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(Kmpleete LANGUAGES CXX)

add_library(Kmpleete)

AddTargetSourcesGroup(Kmpleete "Core"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Core/main.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Core/uuid.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Core/program_options.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Core/memory_checker.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Core/assertion.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Core/settings_document.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Core/settings_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Core/system_metrics_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Core/stacktrace.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Core/exception_handler.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Core/command.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/uuid.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/program_options.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/memory_checker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/assertion.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/settings_document.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/settings_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/system_metrics_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/stacktrace.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/exception_handler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/command.cpp
)
AddTargetSourcesGroup(Kmpleete "Application"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Application/application.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Application/application_context.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Application/window_application.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Application/frame_listener.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Application/frame_listener_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Application/frame_listener_command.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/application_context.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/window_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/frame_listener.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/frame_listener_manager.cpp
)
AddTargetSourcesGroup(Kmpleete "Window"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Window/window.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Window/window_backend.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Window/window_backend_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Window/window_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Window/window_cursor.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Window/window_cursor_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Window/window_native_platform_type.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Window/window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Window/window_backend.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Window/window_backend_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Window/window_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Window/window_cursor_glfw.cpp
)
AddTargetSourcesGroup(Kmpleete "Event"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Event/event.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Event/key_events.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Event/mouse_events.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Event/window_events.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Event/graphics_events.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Event/application_events.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Event/event_handler.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Event/event_handler_guard.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Event/event_dispatcher.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Event/event_queue.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Event/event_queue.cpp
)
AddTargetSourcesGroup(Kmpleete "Input"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Input/input_codes.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Input/input_base.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Input/input_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Input/input_manager.cpp
)
AddTargetSourcesGroup(Kmpleete "Graphics"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/graphics_base.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/graphics_parameters.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/graphics_backend.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/graphics_surface.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/graphics_chain_unit.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/graphics_chain_handler.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/physical_device.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/logical_device.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/swapchain.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/renderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/command_pool.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/image.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/texture.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/font.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/font_character.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/camera.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/orthographic_camera.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/perspective_camera.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/colors.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/graphics_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/graphics_backend.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/graphics_surface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/graphics_chain_unit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/graphics_chain_handler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/physical_device.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/logical_device.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/swapchain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/renderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/image.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/font.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/camera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/orthographic_camera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/perspective_camera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/colors.cpp
)
AddTargetSourcesGroup(Kmpleete "Graphics/Vulkan/Core"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_graphics_base.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_graphics_backend.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_graphics_parameters.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_graphics_surface.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_context.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_descriptor_set_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_fence.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_physical_device.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_logical_device.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_queue.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_renderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_samplers_storage.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_swapchain.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Core/vulkan_metrics_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_graphics_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_graphics_backend.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_graphics_surface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_context.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_descriptor_set_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_fence.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_physical_device.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_logical_device.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_queue.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_renderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_samplers_storage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_swapchain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Core/vulkan_metrics_manager.cpp
)
AddTargetSourcesGroup(Kmpleete "Graphics/Vulkan/Buffer"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Buffer/vulkan_buffer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Buffer/vulkan_vertex_buffer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Buffer/vulkan_buffer_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Buffer/vulkan_buffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Buffer/vulkan_vertex_buffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Buffer/vulkan_buffer_manager.cpp
)
AddTargetSourcesGroup(Kmpleete "Graphics/Vulkan/Command"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Command/vulkan_command_pool.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Command/vulkan_command_buffer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Command/vulkan_command_pool.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Command/vulkan_command_buffer.cpp
)
AddTargetSourcesGroup(Kmpleete "Graphics/Vulkan/Pipeline"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline_parameters.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Pipeline/vulkan_pipeline_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Pipeline/vulkan_pipeline_cache.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline_parameters.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Pipeline/vulkan_pipeline_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Pipeline/vulkan_pipeline_cache.cpp
)
AddTargetSourcesGroup(Kmpleete "Graphics/Vulkan/Shader"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Shader/vulkan_shader_module.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Shader/vulkan_shader_object.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Shader/vulkan_shader_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Shader/vulkan_shader_load_parameters.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Shader/vulkan_shader_module.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Shader/vulkan_shader_object.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Shader/vulkan_shader_manager.cpp
)
AddTargetSourcesGroup(Kmpleete "Graphics/Vulkan/Texture"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Texture/vulkan_image.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Texture/vulkan_texture_base.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Texture/vulkan_texture.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Texture/vulkan_texture_attachment.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Texture/vulkan_texture_attachment_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Texture/vulkan_image.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Texture/vulkan_texture_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Texture/vulkan_texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Texture/vulkan_texture_attachment.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Texture/vulkan_texture_attachment_manager.cpp
)
AddTargetSourcesGroup(Kmpleete "Graphics/Vulkan/Utils"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Utils/initializers.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Utils/function_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Utils/result_description.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Utils/extension_functions.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Utils/presets.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Utils/bits_aliases.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Utils/initializers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Utils/function_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Utils/result_description.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Utils/extension_functions.cpp
)
AddTargetSourcesGroup(Kmpleete "Graphics/Vulkan/Delegates"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Delegates/vulkan_format_delegate.cpp
)
AddTargetSourcesGroup(Kmpleete "Localization"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Localization/localization_base.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Localization/localization_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Localization/localization_translator.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Localization/localization_library.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Localization/localization_dictionary.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Localization/localization_unicode_map.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Localization/localization_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Localization/localization_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Localization/localization_translator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Localization/localization_library.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Localization/localization_dictionary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Localization/localization_unicode_map.cpp
)
AddTargetSourcesGroup(Kmpleete "Assets"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Assets/asset.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Assets/assets_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Assets/font_asset.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Assets/font_asset_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Assets/texture_asset.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Assets/texture_asset_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Assets/asset.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Assets/assets_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Assets/font_asset.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Assets/font_asset_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Assets/texture_asset.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Assets/texture_asset_manager.cpp
)
AddTargetSourcesGroup(Kmpleete "Internal"
    ${CMAKE_CURRENT_LIST_DIR}/internal/Kmpleete/Internal/default_window_icon_data.h
    ${CMAKE_CURRENT_LIST_DIR}/internal/Kmpleete/Internal/error_texture_data.h
)


SetupBoost(Kmpleete)
SetupCompilerOptions(Kmpleete)

target_include_directories(Kmpleete
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
    PUBLIC "${GLFW_SOURCE_DIR}/include"
    
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/internal"
)

if(${KMPLEETE_USE_WINDOW_BACKEND_GLFW})
    target_compile_definitions(Kmpleete PUBLIC 
        KMP_WINDOW_BACKEND_GLFW
    )
else()
    message(SEND_ERROR "No window backend is chosen")
endif()

target_compile_definitions(Kmpleete
    PRIVATE KMP_EXPORTS
    PRIVATE KMP_ENGINE_PRIVATE
)

target_link_libraries(Kmpleete 
    PUBLIC BaseLib
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>
    PUBLIC UtilsLib
    PUBLIC VersionLib
    PUBLIC FilesystemLib
    PUBLIC JsonLib
    PUBLIC MathLib
    PUBLIC AssetsInterfaceLib
    PUBLIC $<IF:$<CONFIG:Production>,ProfilerInterfaceLib,ProfilerLib>
    PUBLIC TimeLib
    PUBLIC ShaderCompilerLib

    PUBLIC glfw
    PUBLIC Vulkan::Vulkan
    
    PRIVATE stb_image
    PRIVATE freetype
    PRIVATE cpptrace-lib
)

if(UNIX)
    #libfontconfig1-dev
    target_link_libraries(Kmpleete PRIVATE fontconfig)
endif()

set_target_properties(Kmpleete PROPERTIES 
    FOLDER Kmpleete/Libraries/Engine
)

if(${KMPLEETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()


# Localization
# --------------------------

set(KmpleeteDomain "Engine")
target_compile_definitions(Kmpleete
    PUBLIC KMP_TR_DOMAIN_ENGINE=\"${KmpleeteDomain}\"
)

CreateTranslationHelperTargets(
    Kmpleete Kmpleete/Libraries/Engine
    "--mode=Update --input_directories=\"${CMAKE_CURRENT_LIST_DIR}/src\" \"${CMAKE_CURRENT_LIST_DIR}/include\" --extensions=.cpp .h --recursive --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${KmpleeteDomain} --gettext_binaries_paths=\"${CMAKE_BINARY_DIR}/src/Tools/Translator/translator_gettext_paths.json\""
    "--mode=Compile --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${KmpleeteDomain} --gettext_binaries_paths=\"${CMAKE_BINARY_DIR}/src/Tools/Translator/translator_gettext_paths.json\""
)


# Installation
# --------------------------

install(TARGETS Kmpleete
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${KmpleeteInstallEditorComponentName}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${KmpleeteInstallEditorComponentName}
)
install(DIRECTORY locale
    DESTINATION ${CMAKE_INSTALL_BINDIR} 
    COMPONENT ${KmpleeteInstallEditorComponentName}
    FILES_MATCHING PATTERN "*.mo"
)
install(TARGETS glfw
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${KmpleeteInstallEditorComponentName}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${KmpleeteInstallEditorComponentName}
)
install(TARGETS freetype
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${KmpleeteInstallEditorComponentName}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${KmpleeteInstallEditorComponentName}
)