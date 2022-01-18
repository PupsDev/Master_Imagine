set(GLM_VERSION "0.9.9")
set(GLM_INCLUDE_DIRS "/mnt/e/Users/pups/Documents/Univ/Master_Imagine/BaseGL (1)/BaseGL/External/glm")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
