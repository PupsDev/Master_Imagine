set(GLM_VERSION "0.9.9")
set(GLM_INCLUDE_DIRS "E:/Users/pups/Documents/Univ/Master_Imagine/S1/HAI719I Programmation 3D/BaseGL/External/glm")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
