#include "EditorLayout.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/WorkspaceItem.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/MetaverseOutliner.cpp"
#include "Widgets/NeuralAssetBrowser.cpp"

namespace UE6 {
namespace Editor {

static const FName MetaverseOutlinerTabId("MetaverseOutliner");
static const FName NeuralBrowserTabId("NeuralAssetBrowser");
static const FName VerseIDETabId("VerseScriptingIDE");

class UE6EditorLayout {
public:
    void InitializeUnifiedEditor(const TSharedRef<FTabManager>& GlobalTabManager) {
        std::cout << "[SLATE-UI] Registering Unified UE6 Editor Tabs...\n";

        // 1. Register the Metaverse Outliner
        GlobalTabManager->RegisterTabSpawner(MetaverseOutlinerTabId, FOnSpawnTab::CreateLambda(
            [](const FSpawnTabArgs& Args) {
                return SNew(SDockTab)
                    .TabRole(ETabRole::PanelTab)
                    [
                        SNew(SMetaverseOutliner)
                    ];
            }))
            .SetDisplayName(FText::FromString("Verse Entity Outliner"));

        // 2. Register the Neural Content Browser
        GlobalTabManager->RegisterTabSpawner(NeuralBrowserTabId, FOnSpawnTab::CreateLambda(
            [](const FSpawnTabArgs& Args) {
                return SNew(SDockTab)
                    .TabRole(ETabRole::PanelTab)
                    [
                        SNew(SNeuralAssetBrowser)
                    ];
            }))
            .SetDisplayName(FText::FromString("Neural Content Hallucinator"));

        // 3. Define the default workspace layout (How the windows are arranged on startup)
        TSharedRef<FTabManager::FLayout> UnifiedDefaultLayout = FTabManager::NewLayout("UE6_UnifiedLayout_v1")
            ->AddArea
            (
                FTabManager::NewPrimaryArea()
                ->SetOrientation(Orient_Horizontal)
                ->Split
                (
                    // Left Side: Verse Scripting & Viewport
                    FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
                    ->SetSizeCoefficient(0.75f)
                    ->Split
                    (
                        FTabManager::NewStack()
                        ->AddTab(VerseIDETabId, ETabState::OpenedTab)
                    )
                    ->Split
                    (
                        FTabManager::NewStack()
                        ->AddTab(NeuralBrowserTabId, ETabState::OpenedTab)
                    )
                )
                ->Split
                (
                    // Right Side: Outliner and Details
                    FTabManager::NewStack()
                    ->SetSizeCoefficient(0.25f)
                    ->AddTab(MetaverseOutlinerTabId, ETabState::OpenedTab)
                )
            );

        std::cout << "[SLATE-UI] Unified Layout initialized. Ready for user interaction.\n";
    }
};

} // namespace Editor
} // namespace UE6
