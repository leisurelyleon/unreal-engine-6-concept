#include "NeuralAssetBrowser.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "NeuralRenderer/RealtimeTextureGen.h"

namespace UE6 {
namespace Editor {

class SNeuralAssetBrowser : public SCompoundWidget {
public:
    SLATE_BEGIN_ARGS(SNeuralAssetBrowser) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs) {
        std::cout << "[SLATE-UI] Booting Generative Neural Asset Browser...\n";

        ChildSlot
        [
            SNew(SVerticalBox)
            
            // Text-to-3D Generation Input
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(8.0f)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .FillWidth(1.0f)
                [
                    SAssignNew(PromptInputBox, SEditableTextBox)
                    .HintText(FText::FromString("e.g., 'A rusted cyberpunk vending machine, 4k textures, overgrown with neon moss'"))
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(4.0f, 0.0f, 0.0f, 0.0f)
                [
                    SNew(SButton)
                    .Text(FText::FromString("Hallucinate Asset"))
                    .OnClicked(this, &SNeuralAssetBrowser::OnGenerateClicked)
                ]
            ]

            // Conceptually, an STileView goes here to display the generated assets
            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            [
                SNew(STextBlock)
                .Text(FText::FromString("Awaiting NPU Latent Inference..."))
                .Justification(ETextJustify::Center)
            ]
        ];
    }

private:
    TSharedPtr<SEditableTextBox> PromptInputBox;

    FReply OnGenerateClicked() {
        FString UserPrompt = PromptInputBox->GetText().ToString();
        std::cout << "[SLATE-UI] Dispatching Text-to-3D Prompt to Project Helix NPU: " 
                  << TCHAR_TO_UTF8(*UserPrompt) << "\n";

        // Dispatch to the NeuralRenderer module we built earlier
        // NeuralRenderer::RealtimeTextureGen::HallucinateModelAsync(UserPrompt);

        return FReply::Handled();
    }
};

} // namespace Editor
} // namespace UE6
