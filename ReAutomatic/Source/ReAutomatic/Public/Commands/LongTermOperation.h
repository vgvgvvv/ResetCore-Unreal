#pragma once

/// 长时间操作
class REAUTOMATIC_API FLongTermOperation
{
public:
	virtual ~FLongTermOperation() = default;
	virtual void OnBegin() = 0;
	virtual void OnTick() = 0;
	virtual void OnEnd() = 0;

	virtual bool IsFinished() = 0;
};
