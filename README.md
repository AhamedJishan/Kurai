# Kurai
Project 2

## TODO:
- Optimize VertexSkinData to use uint8_t instead of glm::vec4
- Physics and ParticleSystem temporarily disabled, will be reintegrated later.
- Add Init() for component, and it should be called by scene after loading all actors and components, and just before scene update cycle begins.
- Think about RemoveActor and RemoveComponent
- Refactor Rendering to fit the new MeshRender.