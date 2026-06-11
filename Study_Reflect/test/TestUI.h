#pragma once
#include "../ui/core/WidgetManager.h"
#include "../ui/component/CanvasPanel.h"
#include "../ui/component/VerticalBox.h"
#include "../ui/component/HorizontalBox.h"
#include "../ui/component/SizeBox.h"
#include "../ui/widget/Button.h"
#include "../ui/widget/TextBlock.h"
#include "../ui/animation/WidgetAnimation.h"
#include "../ui/component/FullScreenPanel.h"

inline void SetupTestUI()
{
    auto& manager = WidgetManager::Get();

    // 创建主Canvas
    auto mainCanvas = manager.CreateWidget<CanvasPanel>("MainCanvas");
    mainCanvas->SetSize(ImVec2(800, 600));
    manager.AddToViewport(mainCanvas, 0);

    // 创建垂直盒子布局
    auto vbox = manager.CreateWidget<VerticalBox>("MainVBox");
    vbox->SetSize(ImVec2(300, 400));
    vbox->SetSpacing(10.0f);
    mainCanvas->AddChildToCanvas(vbox, ImVec2(50, 50), ImVec2(300, 400));

    // 添加标题文本
    auto titleText = manager.CreateWidget<TextBlock>("TitleText");
    titleText->SetText("My Game UI");
    titleText->SetColor(ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
    titleText->SetFontScale(1.5f);
    titleText->SetSize(ImVec2(300, 40));
    vbox->AddChildToVerticalBox(titleText);

    // 添加水平盒子用于按钮
    auto hbox = manager.CreateWidget<HorizontalBox>("ButtonHBox");
    hbox->SetSize(ImVec2(300, 50));
    hbox->SetSpacing(10.0f);
    vbox->AddChildToVerticalBox(hbox);

    // 添加播放按钮
    auto playButton = manager.CreateWidget<Button>("PlayButton");
    playButton->SetText("Play");
    playButton->SetSize(ImVec2(140, 50));
    playButton->OnClicked = []() {
        printf("Play clicked!\n");
        };
    hbox->AddChildToHorizontalBox(playButton);

    // 添加设置按钮
    auto settingsButton = manager.CreateWidget<Button>("SettingsButton");
    settingsButton->SetText("Settings");
    settingsButton->SetSize(ImVec2(140, 50));
    settingsButton->OnClicked = []() {
        printf("Settings clicked!\n");
        };
    hbox->AddChildToHorizontalBox(settingsButton);

    // 创建平移动画示例
    auto moveAnimation = std::make_shared<WidgetAnimation>("MoveRight");
    moveAnimation->SetDuration(1.0f);
    moveAnimation->SetLooping(true);
    moveAnimation->SetEasingFunction([](float t) {
        return t * t * (3.0f - 2.0f * t); // Smooth step
        });

    ImVec2 startPos = playButton->GetPosition();
    moveAnimation->OnUpdate = [startPos](Widget* widget, float progress) {
        ImVec2 newPos = startPos;
        newPos.x += sin(progress * 3.14159f * 2.0f) * 20.0f;
        widget->SetPosition(newPos);
        };

    playButton->AddAnimation(moveAnimation);
    playButton->PlayAnimation("MoveRight");

    // 创建SizeBox示例
    auto sizeBox = manager.CreateWidget<SizeBox>("ConstrainedBox");
    sizeBox->SetWidthOverride(200);
    sizeBox->SetHeightOverride(100);
    sizeBox->SetMinDesiredHeight(50);
    sizeBox->SetMaxDesiredHeight(200);
    mainCanvas->AddChildToCanvas(sizeBox, ImVec2(400, 100), ImVec2(200, 100));

    auto innerText = manager.CreateWidget<TextBlock>("InnerText");
    innerText->SetText("Size Box Content");
    innerText->SetColor(ImVec4(0.5f, 0.8f, 1.0f, 1.0f));
    innerText->SetSize(ImVec2(200, 30));
    sizeBox->AddChild(innerText);
}

void SetupFullScreenUI()
{
    auto& manager = WidgetManager::Get();

    // ========== 1. 创建全屏面板 ==========
    auto fullScreenPanel = manager.CreateWidget<FullScreenPanel>("MainFullScreen");

    // 添加到视口（作为根控件）
    manager.AddToViewport(fullScreenPanel, 0);

    // ========== 2. 在面板中添加内容 ==========

    // 创建垂直布局（自动填满整个屏幕）
    auto mainVBox = manager.CreateWidget<VerticalBox>("MainLayout");
    mainVBox->SetSpacing(10.0f);
    fullScreenPanel->AddChild(mainVBox);

    // 添加标题
    auto titleText = manager.CreateWidget<TextBlock>("Title");
    titleText->SetText("我的游戏");
    titleText->SetColor(ImVec4(1.0f, 0.8f, 0.0f, 1.0f)); // 金色
    titleText->SetFontScale(2.0f);
    mainVBox->AddChildToVerticalBox(titleText);

    // 添加内容区域（自动填充剩余空间）
    auto contentHBox = manager.CreateWidget<HorizontalBox>("Content");
    contentHBox->SetSpacing(10.0f);
    mainVBox->AddChildToVerticalBox(contentHBox, BoxSlot::SizeRule::Fill, 1.0f);

    // 左侧面板（占30%宽度）
    auto leftPanel = manager.CreateWidget<VerticalBox>("LeftPanel");
    contentHBox->AddChildToHorizontalBox(leftPanel, BoxSlot::SizeRule::Fill, 0.3f);

    auto leftLabel = manager.CreateWidget<TextBlock>("LeftLabel");
    leftLabel->SetText("左侧菜单");
    leftLabel->SetColor(ImVec4(0.5f, 0.8f, 1.0f, 1.0f));
    leftPanel->AddChildToVerticalBox(leftLabel);

    // 右侧面板（占70%宽度）
    auto rightPanel = manager.CreateWidget<VerticalBox>("RightPanel");
    contentHBox->AddChildToHorizontalBox(rightPanel, BoxSlot::SizeRule::Fill, 0.7f);

    auto rightLabel = manager.CreateWidget<TextBlock>("RightLabel");
    rightLabel->SetText("主要内容区域");
    rightLabel->SetColor(ImVec4(0.8f, 1.0f, 0.5f, 1.0f));
    rightPanel->AddChildToVerticalBox(rightLabel);

    // 底部按钮栏
    auto bottomHBox = manager.CreateWidget<HorizontalBox>("BottomButtons");
    bottomHBox->SetSpacing(20.0f);
    mainVBox->AddChildToVerticalBox(bottomHBox);

    auto playButton = manager.CreateWidget<Button>("PlayButton");
    playButton->SetText("开始游戏");
    playButton->SetSize(ImVec2(150, 50));
    playButton->OnClicked = []() {
        printf("开始游戏！\n");
        };
    bottomHBox->AddChildToHorizontalBox(playButton);

    auto quitButton = manager.CreateWidget<Button>("QuitButton");
    quitButton->SetText("退出游戏");
    quitButton->SetSize(ImVec2(150, 50));
    quitButton->SetColor(ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); // 红色
    quitButton->OnClicked = []() {
        printf("退出游戏！\n");
        };
    bottomHBox->AddChildToHorizontalBox(quitButton);
}