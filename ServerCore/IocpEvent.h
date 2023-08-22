#pragma once
class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(eIocpEventType eventType);
	virtual ~IocpEvent();
public:
	void Init();
	eIocpEventType getEventType() const { return mEventType; }
private:
	eIocpEventType mEventType;
};

