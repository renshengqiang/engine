include_directories(./include /usr/local/include/assimp /usr/include/ImageMagick)
link_libraries(/usr/local/lib)

set(ENGINE_SRCS
    src/util/UtilThread.cpp
    src/util/UtilTimer.cpp
    src/math/EngineVector.cpp
    src/math/EngineQuaternion.cpp
    src/math/EngineMatrix.cpp
    src/EngineAxisAlignedBox.cpp
    src/EngineRenderOp.cpp
    src/EngineRenderWindow.cpp
    src/EngineTexture.cpp
    src/effect/EngineEffect.cpp
    src/effect/EngineSimpleMeshEffect.cpp
    src/effect/EngineParticleSystemEffect.cpp
    src/effect/EngineEffectManager.cpp
    src/EngineMesh.cpp
    src/EngineSubMesh.cpp
    src/EngineMeshManager.cpp
    src/EngineEntity.cpp
    src/EngineSubEntity.cpp
    src/EngineNode.cpp
    src/EngineSceneNode.cpp
    src/EngineMoveableObject.cpp
    src/EngineFrustum.cpp
    src/EngineCamera.cpp
    src/EngineRay.cpp
    src/EngineRoot.cpp
    src/EngineSceneManager.cpp
    )

add_library(engine SHARED ${ENGINE_SRCS})
set_target_properties(engine PROPERTIES VERSION 0.0 SOVERSION 0)
target_link_libraries(engine assimp Magick++ SDL GLEW GL m pthread stdc++)

set(HEADERS
    include/UtilMutexLock.h
    include/UtilCondition.h
    include/UtilThread.h
    include/UtilTimer.h
    include/UtilSingleton.h
    include/EngineMath.h
    include/EngineVector.h
    include/EngineQuaternion.h
    include/EngineMatrix.h
    include/EnginePlane.h
    include/EngineAxisAlignedBox.h
    include/EngineRenderOp.h
    include/EngineRenderWindow.h
    include/EngineTexture.h
    include/EngineEffect.h
    include/EngineSimpleMeshEffect.h
    include/EngineParticleSystem.h
    include/EngineEffectManager.h
    include/EngineMesh.h
    include/EngineSubMesh.h
    include/EngineMeshManager.h
    include/EngineEntity.h
    include/EngineSubEntity.h
    include/EngineNode.h
    include/EngineSceneNode.h
    include/EngineMoveableObject.h
    include/EngineFrustum.h
    include/EngineCamera.h
    include/EngineRay.h
    include/EngineFrameListener.h
    include/EngineRoot.h
    include/EngineSceneManager.h
    include/EngineRenderable.h
    )
install(FILES ${HEADERS} DESTINATION include/3dengine) 
