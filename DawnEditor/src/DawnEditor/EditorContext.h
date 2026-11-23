#pragma once

namespace Dawn
{
	// Forward declarations
	class Actor;

	enum class EditorState { Edit, Play };
	enum class EditorAction { None, Play, Stop };

	struct EditorContext
	{
	public:
		bool IsPlaying() const { return mState == EditorState::Play; }

	public:
		EditorAction requestAction = EditorAction::None;
		Actor* selectedActor = nullptr;

	private:
		friend class EditorLayer;
		// EditorLayer should be the only one editing this.
		EditorState mState = EditorState::Edit;
	};
}