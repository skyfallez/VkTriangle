@ECHO OFF

SET INCLUDE= -I C:\VulkanSDK\1.3.204.0\Include\vulkan
SET LIBS=-L C:\VulkanSDK\1.3.204.0\Lib

SET SOURCE_FILES=^
 src/main.cpp^
 src/window.cpp^
 src/renderer.cpp


g++ -Wall -Werror -g %SOURCE_FILES% %LIBS% -lvulkan-1 %INCLUDE% -o program