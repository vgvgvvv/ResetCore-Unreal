# UE4 Control Center 文件夹

对远端消息进行抽象，将消息分发到工程侧。

Demo代码：

工程侧仅需调用启动、关闭、以及注册任务三种接口。
可以完全进行测试代码分离。

UE4 Control Center 可以通过RegisterRouteClient注册多个端同时进行控制

```C++
void ARemoteControlGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	CurrentClient = MakeShared<FRouteClient>();

	CurrentClient->Init("UE4", "121.196.216.106", 8000);
	CurrentClient->RunWithBackground();

	FUE4ControlCenter::Get().RegisterRouteClient(*CurrentClient);

	FUE4ControlCenter::Get().RegisterCmdEvent("TestHandler", &ARemoteControlGameModeBase::TestHandler);
	
	FUE4ControlCenter::Get().OnRunLua.AddUObject(this, &ARemoteControlGameModeBase::HandleRunLua);
}

void ARemoteControlGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(CurrentClient.IsValid())
	{
		FUE4ControlCenter::Get().RemoveRouteClient(*CurrentClient);
		CurrentClient->Stop();
	}
}
```