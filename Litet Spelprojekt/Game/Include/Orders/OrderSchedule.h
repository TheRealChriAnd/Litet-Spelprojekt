#pragma once
#include "OrderSleep.h"

class OrderSchedule
{
public:
	OrderSchedule() = delete;
	~OrderSchedule() = delete;
	OrderSchedule(OrderSchedule&& other) = delete;
	OrderSchedule(const OrderSchedule& other) = delete;
	OrderSchedule& operator=(OrderSchedule&& other) = delete;
	OrderSchedule& operator=(const OrderSchedule& other) = delete;

public:
	static void Init(Scene* pScene);
	static void Release();
	static IOrder* GetIdleOrder(); 

private:
	static std::vector<GameObject*> s_Beds;
	static std::vector<GameObject*> s_Toilets;
	static std::vector<GameObject*> s_Instruments;
	static std::vector<GameObject*> s_Chairs;
};