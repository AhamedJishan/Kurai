# Kurai
Project 2

## TODO:
- Check the null scene case in game and engine.
- Play game button
- Fix, AddComponent component autofill.
- Fix, drag n drop not working for skeleton and animation clip type.
- Inspector shold display the name of the active actor.
- Fix SetListner for Engine and EditorCamera both. Don't use view matrix.
- Make resolution as uvec2 everywhere, renderer, texture, etc.
- Rename scene  from Scene Hierarchy.
- Make all windows title bold + component table ratio change
- Add blender like transform control, g to grab, s to scale, r to rotate
- Add gui option to tweak editor camera settings.
- Add file hierarchy window in assetbrowser
- Refine the F to focus, for precise focus using bounding boxes.
- Make a centralized place to edit and store asset paths. (Audio, Font, Models, etc)
- Optimize VertexSkinData to use uint8_t instead of glm::vec4
- Physics and ParticleSystem temporarily disabled, will be reintegrated later.
- Make Console window
- Scene dirty flag should not stay in Dawn project. Editor project should keep track of it.
- Have a fixed layout for the editor to start with. [probably using the dockbuilder api]