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


struct DemoSettings
{
    bool ShowGrid = true;
    bool AnimateScene = true;
    bool AutoOrbitCamera = true;
    bool ShowImGuiDemo = false;
    bool Vsync = true;

    float CubeSpeed = 1.0f;
    float CameraDistance = 8.0f;
    float CameraYawDegrees = -35.0f;
    float CameraPitchDegrees = 24.0f;
    float ClearColor[4] = { 0.015f, 0.019f, 0.028f, 1.0f };

    int SelectedSlot = 1;
};
extern void SetupGameStyle(float dpi_scale);
extern void DrawGameHud(DemoSettings& settings, float elapsed_seconds);
void DrawMiniMap(float elapsed_seconds)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImVec2 origin = ImGui::GetCursorScreenPos();
    const ImVec2 size(190.0f, 150.0f);
    const ImVec2 rect_max(origin.x + size.x, origin.y + size.y);

    ImGui::InvisibleButton("##minimap_canvas", size);
    draw_list->AddRectFilled(origin, rect_max, IM_COL32(7, 11, 18, 190), 6.0f);
    draw_list->AddRect(origin, rect_max, IM_COL32(84, 177, 203, 180), 6.0f, 0, 1.5f);

    for (int i = 1; i < 5; ++i)
    {
        const float x = origin.x + size.x * (float)i / 5.0f;
        const float y = origin.y + size.y * (float)i / 5.0f;
        draw_list->AddLine(ImVec2(x, origin.y + 8.0f), ImVec2(x, rect_max.y - 8.0f), IM_COL32(45, 67, 83, 130));
        draw_list->AddLine(ImVec2(origin.x + 8.0f, y), ImVec2(rect_max.x - 8.0f, y), IM_COL32(45, 67, 83, 130));
    }

    const ImVec2 center(origin.x + size.x * 0.5f, origin.y + size.y * 0.5f);
    draw_list->AddCircleFilled(center, 5.5f, IM_COL32(107, 230, 180, 255));

    const float patrol = elapsed_seconds * 1.4f;
    for (int i = 0; i < 4; ++i)
    {
        const float angle = patrol + i * 1.57f;
        const ImVec2 blip(
            center.x + std::cos(angle) * (42.0f + i * 6.0f),
            center.y + std::sin(angle * 0.85f) * (28.0f + i * 5.0f));
        draw_list->AddCircleFilled(blip, 3.5f, IM_COL32(255, 107, 116, 245));
    }
}

void DrawCrosshair()
{
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetForegroundDrawList();
    const ImVec2 center(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    const ImU32 color = IM_COL32(170, 245, 220, 210);

    draw_list->AddCircle(center, 3.0f, color, 18, 1.2f);
    draw_list->AddLine(ImVec2(center.x - 18.0f, center.y), ImVec2(center.x - 7.0f, center.y), color, 1.4f);
    draw_list->AddLine(ImVec2(center.x + 7.0f, center.y), ImVec2(center.x + 18.0f, center.y), color, 1.4f);
    draw_list->AddLine(ImVec2(center.x, center.y - 18.0f), ImVec2(center.x, center.y - 7.0f), color, 1.4f);
    draw_list->AddLine(ImVec2(center.x, center.y + 7.0f), ImVec2(center.x, center.y + 18.0f), color, 1.4f);
}


void SetupGameStyle(float dpi_scale)
{
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(dpi_scale);
    style.FontScaleDpi = dpi_scale;
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.025f, 0.034f, 0.050f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.24f, 0.62f, 0.70f, 0.46f);
    colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.96f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.42f, 0.50f, 0.56f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.10f, 0.16f, 0.23f, 0.90f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.17f, 0.37f, 0.44f, 0.95f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.22f, 0.55f, 0.64f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.08f, 0.11f, 0.16f, 0.90f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.13f, 0.25f, 0.31f, 0.95f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.35f, 0.42f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.39f, 0.92f, 0.78f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.32f, 0.77f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.94f, 1.00f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.24f, 0.62f, 0.70f, 0.38f);
}

void DrawGameHud(DemoSettings& settings, float elapsed_seconds)
{
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiWindowFlags overlay_flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing;

    DrawCrosshair();

    ImGui::SetNextWindowPos(ImVec2(16.0f, 16.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - 32.0f, 64.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.58f);
    if (ImGui::Begin("##top_hud", nullptr, overlay_flags))
    {
        const float health = 0.72f + std::sin(elapsed_seconds * 0.9f) * 0.08f;
        const float shield = 0.48f + std::cos(elapsed_seconds * 0.7f) * 0.06f;

        ImGui::TextUnformatted("ALPHA-7");
        ImGui::SameLine(120.0f);
        ImGui::ProgressBar(health, ImVec2(180.0f, 14.0f), "HP");
        ImGui::SameLine();
        ImGui::ProgressBar(shield, ImVec2(160.0f, 14.0f), "SHIELD");
        ImGui::SameLine();
        ImGui::Text("AMMO %03d / 180", 96 + (int)(std::sin(elapsed_seconds * 1.8f) * 8.0f));
        ImGui::SameLine();
        ImGui::Text("FPS %.0f", io.Framerate);
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(16.0f, 96.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(222.0f, 214.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.62f);
    if (ImGui::Begin("##minimap_panel", nullptr, overlay_flags))
    {
        ImGui::TextUnformatted("TACTICAL MAP");
        DrawMiniMap(elapsed_seconds);
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 292.0f, 96.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(276.0f, 300.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.66f);
    if (ImGui::Begin("Tactical Panel", nullptr, overlay_flags | ImGuiWindowFlags_NoResize))
    {
        ImGui::TextUnformatted("SCENE");
        ImGui::Separator();
        ImGui::Checkbox("Grid", &settings.ShowGrid);
        ImGui::Checkbox("Animate", &settings.AnimateScene);
        ImGui::Checkbox("Camera orbit", &settings.AutoOrbitCamera);
        ImGui::Checkbox("VSync", &settings.Vsync);
        ImGui::Checkbox("ImGui demo", &settings.ShowImGuiDemo);
        ImGui::SliderFloat("Cube speed", &settings.CubeSpeed, 0.0f, 3.0f, "%.2f");
        ImGui::SliderFloat("Distance", &settings.CameraDistance, 3.0f, 14.0f, "%.1f");
        ImGui::SliderFloat("Yaw", &settings.CameraYawDegrees, -180.0f, 180.0f, "%.0f deg");
        ImGui::SliderFloat("Pitch", &settings.CameraPitchDegrees, 8.0f, 60.0f, "%.0f deg");
        ImGui::ColorEdit3("Clear", settings.ClearColor, ImGuiColorEditFlags_NoInputs);
    }
    ImGui::End();

    const float bar_width = 380.0f;
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f - bar_width * 0.5f, io.DisplaySize.y - 92.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(bar_width, 70.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.58f);
    if (ImGui::Begin("##quick_slots", nullptr, overlay_flags))
    {
        const char* labels[] = { "1\nBlade", "2\nPulse", "3\nKit", "4\nDrone", "5\nMine" };
        for (int i = 0; i < 5; ++i)
        {
            if (i > 0)
                ImGui::SameLine();

            if (settings.SelectedSlot == i)
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.13f, 0.48f, 0.54f, 0.92f));
            else
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.09f, 0.12f, 0.17f, 0.88f));

            if (ImGui::Button(labels[i], ImVec2(66.0f, 48.0f)))
                settings.SelectedSlot = i;
            ImGui::PopStyleColor();
        }
    }
    ImGui::End();

    if (settings.ShowImGuiDemo)
        ImGui::ShowDemoWindow(&settings.ShowImGuiDemo);
}
    
