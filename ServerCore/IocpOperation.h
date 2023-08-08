#pragma once
class IocpOperation : public OVERLAPPED
{
public:
	IocpOperation(eOperationType operationType);
	virtual ~IocpOperation();
public:
	void Init();
	eOperationType getOperationType() const { return mOperationType; }
private:
	eOperationType mOperationType;
};

