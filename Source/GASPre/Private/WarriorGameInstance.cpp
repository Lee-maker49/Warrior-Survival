// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorGameInstance.h"
#include "MoviePlayer.h"

//初始化
void UWarriorGameInstance::Init()
{
    Super::Init();
    //为加载关卡前的事件绑定函数
    FCoreUObjectDelegates::PreLoadMap.AddUObject(this,&ThisClass::OnPreLoadMapFunction);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&ThisClass::OnDestinationWorldLoaded);
}
//加载地图前委托的回调函数
void UWarriorGameInstance::OnPreLoadMapFunction(const FString& MapName)
{
    //设置参数
    FLoadingScreenAttributes LoadingScreenAttributes;
    LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true; 
    LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;
    LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
    //GetMoviePlayer返回全局单例IGameMoviePlayer* 是引擎内置的过场和加载动画播放器 使用不同的线程
    //播放加载画面
    GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}
//加载地图结束后回调函数
void UWarriorGameInstance::OnDestinationWorldLoaded(UWorld *LoadedWorld)
{
    //停止播放加载画面
    GetMoviePlayer()->StopMovie();
}
// 通过Tag获取Level
TSoftObjectPtr<UWorld> UWarriorGameInstance::GetGameLevelByTag(FGameplayTag InTag)
{
    for(const FWarriorGameLevel &GameLevel : GameLevelSets)
    {
        if(!GameLevel.IsValid()) continue;

        if(InTag == GameLevel.LevelTag)
        {
            return GameLevel.Level;
        }
    }

    return TSoftObjectPtr<UWorld>();
}
