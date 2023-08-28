#pragma once
class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(eIocpEventType eventType);
public:
	void Init();
	eIocpEventType GetEventType() const { return mEventType; }
private:
	eIocpEventType mEventType;
};

