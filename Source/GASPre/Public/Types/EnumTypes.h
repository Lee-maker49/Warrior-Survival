#pragma once

UENUM()
enum class EConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EValidType : uint8
{
    Valid,
    InValid
};

UENUM()
enum class ESuccessType : uint8
{
    Successful,
    Failed
};

UENUM()
enum class ECountDownCompleteType : uint8
{
	OnGoing,
	Completed
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
    Front = 0,
    Back = 1,
    Left = 2,
    Right = 3
};
//游戏难度枚举
UENUM(BlueprintType)
enum class ESurvivalGameDifficuty : uint8
{
    Easy,
    Normal,
    Hard,
    VeryHard
};
//游戏输入模式
UENUM(BlueprintType)
enum class EGameModeInput : uint8
{
    UIOnly,
    GameOnly
};

UENUM()
enum class EWarriorCountDownActionInput : uint8
{
    Start,
    Cancel
};

UENUM()
enum class EWarriorCountDownActionOutput : uint8
{
    Updated,
    Completed,
    Cancelled
};

