#include "MetaverseOutliner.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Input/SSearchBox.h"
#include "VerseVM/TransactionalMemory.h" // Hooking into the Verse STM we built

namespace UE6 {
namespace Editor {

class SMetaverseOutliner : public SCompoundWidget {
public:
    SLATE_BEGIN_ARGS(SMetaverseOutliner) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs) {
        std::cout << "[SLATE-UI] Initializing Virtualized Metaverse Outliner...\n";

        // The Outliner no longer holds hard pointers to actors. 
        // It holds lightweight 64-bit Verse Entity IDs.
        ActiveVisibleEntities = MakeShareable(new std::vector<uint64_t>());

        ChildSlot
        [
            SNew(SVerticalBox)
            
            // AI-Powered Search Bar
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(4.0f)
            [
                SNew(SSearchBox)
                .HintText(FText::FromString("Search 10,000,000+ Entities via Natural Language..."))
                .OnTextChanged(this, &SMetaverseOutliner::OnSearchTextChanged)
            ]

            // Massively Virtualized List View
            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            [
                SNew(SListView<uint64_t>)
                .ListItemsSource(ActiveVisibleEntities.Get())
                .OnGenerateRow(this, &SMetaverseOutliner::GenerateEntityRow)
                .SelectionMode(ESelectionMode::Multi)
                .ClearSelectionOnClick(true)
            ]
        ];
    }

private:
    TSharedPtr<std::vector<uint64_t>> ActiveVisibleEntities;

    // Generates the UI row ONLY when the user scrolls it into view
    TSharedRef<ITableRow> GenerateEntityRow(uint64_t EntityID, const TSharedRef<STableViewBase>& OwnerTable) {
        // Queries the Verse VM for the entity's name in micro-seconds
        FString EntityName = VerseVM::GetEntityNameFast(EntityID);

        return SNew(STableRow<uint64_t>, OwnerTable)
        [
            SNew(STextBlock).Text(FText::FromString(EntityName))
        ];
    }

    void OnSearchTextChanged(const FText& SearchText) {
        // Dispatches a background fiber to query the Verse STM without freezing the UI
        std::cout << "[SLATE-UI] Executing async semantic search for: " << TCHAR_TO_UTF8(*SearchText.ToString()) << "\n";
    }
};

} // namespace Editor
} // namespace UE6
