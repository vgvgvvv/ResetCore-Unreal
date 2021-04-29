#pragma once

template<typename T>
class TDefaultValue
{
public:
	static T value;
};

template<typename T>
T TDefaultValue<T>::value = T();

template<typename T>
class TDefaultValue<T&>
{
public:
	static T value;
};

template<typename T>
T TDefaultValue<T&>::value = T();

template<typename T>
class TDefaultValue<T*>
{
public:
	static T* value;
};

template<typename T>
T* TDefaultValue<T*>::value = nullptr;