// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CGame : ModuleRules
{
    public CGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
            "GameplayTasks","GameplayCameras","NavigationSystem"
        });

        PublicIncludePaths.Add(ModuleDirectory);

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "AnimGraphRuntime", // ← 중복 하나만 유지
            "Niagara"
        });

        // --- Windows 전용: min/max 매크로 차단 + 헤더 슬림화 ---
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDefinitions.Add("NOMINMAX=1");
            PublicDefinitions.Add("WIN32_LEAN_AND_MEAN=1");
            // (선택) 구식 winsock API 경고 임시 억제
            // PrivateDefinitions.Add("_WINSOCK_DEPRECATED_NO_WARNINGS=1");
        }

        // // UI 쓸 때만 주석 해제
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}