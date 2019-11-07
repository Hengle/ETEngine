#include "stdafx.h"
#include "SceneEditor.h"

#include "EditorScene.h"

#include <EtCore/Helper/InputManager.h>

#include <Engine/SceneGraph/SceneManager.h>
#include <Engine/SceneRendering/SceneRenderer.h>
#include <Engine/Physics/PhysicsManager.h>
#include <Engine/Audio/AudioManager.h>

#include <EtEditor/Util/GtkUtil.h>
#include <EtEditor/Tools/SceneViewport.h>


//==========================
// Scene Editor
//==========================


// statics
std::string const SceneEditor::s_EditorName("Scene Editor");
std::string const SceneEditor::s_LayoutName("scene_editor");
std::vector<E_EditorTool> const SceneEditor::s_SupportedTools = {
	E_EditorTool::Outliner,
	E_EditorTool::SceneViewport
	};


//---------------------------
// SceneEditor::d-tor
//
SceneEditor::~SceneEditor()
{
	SceneManager::DestroyInstance();
	PhysicsManager::DestroyInstance();
	AudioManager::DestroyInstance();
}

//---------------------------
// SceneEditor::InitInternal
//
// create the tools and attach them to the parent frame
//
void SceneEditor::InitInternal()
{
	m_IsShown = true;
	for (I_SceneEditorListener* const listener : m_Listeners)
	{
		listener->OnShown();
	}

	AudioManager::GetInstance()->Initialize();
	PhysicsManager::GetInstance()->Initialize();

	SceneManager::GetInstance()->AddGameScene(new EditorScene());
	SceneManager::GetInstance()->SetActiveGameScene("EditorScene");
	m_SceneSelection.SetScene(SceneManager::GetInstance()->GetNewActiveScene());
	for (I_SceneEditorListener* const listener : m_Listeners)
	{
		listener->OnSceneSet();
	}
}

//----------------------------------------------------
// SceneEditor::OnKeyEvent
//
bool SceneEditor::OnKeyEvent(bool const pressed, GdkEventKey* const evnt)
{
	if (m_NavigatingViewport != nullptr)
	{
		return m_NavigatingViewport->OnKeyEvent(pressed, evnt);
	}

	return false;
}

//----------------------------------------------------
// SceneEditor::OnTick
//
void SceneEditor::OnTick()
{
	m_SceneSelection.UpdateOutliners();

	for (I_SceneEditorListener* const listener : m_Listeners)
	{
		listener->OnEditorTick();
	}
}

//----------------------------------------------------
// SceneEditor::RegisterListener
//
void SceneEditor::RegisterListener(I_SceneEditorListener* const listener)
{
	ET_ASSERT(std::find(m_Listeners.cbegin(), m_Listeners.cend(), listener) == m_Listeners.cend(), "Listener already registered!");

	m_Listeners.emplace_back(listener);
}

//----------------------------------------------------
// SceneEditor::UnregisterListener
//
void SceneEditor::UnregisterListener(I_SceneEditorListener const* const listener)
{
	// try finding the listener
	auto listenerIt = std::find(m_Listeners.begin(), m_Listeners.end(), listener);

	// it should have been found
	if (listenerIt == m_Listeners.cend())
	{
		LOG("SceneEditor::UnregisterListener > Listener not found", LogLevel::Warning);
		return;
	}

	// swap and remove - the order of the listener list doesn't matter
	if (m_Listeners.size() > 1u)
	{
		std::iter_swap(listenerIt, std::prev(m_Listeners.end()));
		m_Listeners.pop_back();
	}
	else
	{
		m_Listeners.clear();
	}
}