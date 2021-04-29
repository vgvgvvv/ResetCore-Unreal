#pragma once

#include "Utility/DefaultValue.h"
#include "Containers/Array.h"
#include "Containers/Set.h"
#include "Containers/Map.h"

namespace Linq
{
	template<typename T>
	class TWhere;
	template<typename T, typename R>
	class TSelect;
	template<typename T1, typename T2, typename R>
	class TJoin;
	template<typename T>
	class TOfType;
	template<typename T, typename R>
	class TCastTo;
	template<typename T>
	class TArrayEnumerator;
	template<typename K,typename V>
	class TMapEnumerator;

	template<typename T>
	class TEnumerator
	{
	public:
		TEnumerator<T>(TEnumerator<T>* Parent = nullptr) : Parent(Parent)
		{
		}

		virtual bool MoveNext()
		{
			if (!Parent)
			{
				return false;
			}
			return Parent->MoveNext();
		};

		virtual T Current()
		{
			if(!Parent)
			{
				return TDefaultValue<T>::value;
			}
			return Parent->Current();
		};

		virtual void Reset()
		{
			if (!Parent)
				return;
			Parent->Reset();
		}

		virtual ~TEnumerator() {}

	public:
		TWhere<T> Where(TFunction<bool(T)> Condition)
		{
			return TWhere<T>(this, Condition);
		}

		template <typename R>
		TSelect<T, R> Select(TFunction<R(T)> Func)
		{
			return TSelect<T, R>(this, Func);
		}

		template <typename T2, typename R>
		TJoin<T, T2, R> Join(TEnumerator<T2>* Another, TFunction<R(T)> Func)
		{
			return TJoin<T, T2, R>(this, Another, Func);
		}
		
		TOfType<T> OfClass(UClass* Class)
		{
			return TOfType<T>(this, Class);
		}

		template<typename R>
		TCastTo<T,R> CastTo()
		{
			return CastTo<T, R>(this);
		}
		
		T Sum()
		{
			T Result;
			while (MoveNext())
			{
				Result += Current();
			}
			return Result;
		}

		template<typename R>
		R Sum(TFunction<R(T)> Func)
		{
			R Result;
			while (MoveNext())
			{
				Result = Result + Func(Current());
			}
			return Result;
		}

		T First()
		{
			Reset();
			if (!MoveNext())
			{
				return TDefaultValue<T>::value;
			}
			return Current();
		}

		T Last()
		{
			if (!MoveNext())
			{
				return TDefaultValue<T>::value;
			}
			while (MoveNext()) {}
			return Current();
		}

		T Average()
		{
			T Result;
			int Count = 0;
			while (MoveNext())
			{
				Result += Current();
				Count++;
			}
			return Result / Count;
		}
		
		template<typename R>
		R Average(TFunction<R(T)> Func)
		{
			R Result;
			int Count = 0;
			while (MoveNext())
			{
				Result += Func(Current());
				Count++;
			}
			return Result / Count;
		}

		bool Any(TFunction<bool(T)> Condition)
		{
			while (MoveNext())
			{
				if (Condition(Current()))
				{
					return true;
				}
			}
			return false;
		}

		T Max()
		{
			if (!MoveNext())
			{
				return TDefaultValue<T>::value;
			}
			T Max = Current();
			while (MoveNext())
			{
				if (Max < Current())
				{
					Max = Current();
				}
			}
			return Max;
		}

		T Min()
		{
			if (!MoveNext())
			{
				return TDefaultValue<T>::value;
			}
			T Min = Current();
			while (MoveNext())
			{
				if (Min > Current())
				{
					Min = Current();
				}
			}
			return Min;
		}

		T Most(TFunction<bool(T, T)> IsBetter)
		{
			if(!MoveNext())
			{
				return TDefaultValue<T>::value;
			}
			T Result = Current();
			while (MoveNext())
			{
				if (!IsBetter(Result, Current()))
				{
					Result = Current();
				}
			}
			return Result;
		}

		T Single(TFunction<bool(T)> Func)
		{
			while (MoveNext())
			{
				T Current = Current();
				if (Func(Current))
				{
					return Current;
				}
			}
			return TDefaultValue<T>::value;
		}

		TEnumerator& Skip(int Num)
		{
			for (int i = 0; i < Num; i++)
			{
				if (!MoveNext())
				{
					return *this;
				}
			}
			return *this;
		}

		void ForEach(TFunction<void(T)> Act)
		{
			while (MoveNext())
			{
				Act(Current());
			}
		}

		TArray<T> ToArray()
		{
			TArray<T> Result;
			while (MoveNext())
			{
				Result.Add(Current());
			}
			return Result;
		}

		void ToArray(TArray<T>& Out)
		{
			while (MoveNext())
			{
				Out.Add(Current());
			}
		}

		TSet<T> ToSet()
		{
			TSet<T> Result;
			while (MoveNext())
			{
				Result.Add(Current());
			} 
			return Result;
		}

	protected:
		TEnumerator<T>* Parent;
	};
	
	template<typename T>
	TArrayEnumerator<T> From(TArray<T> Array)
	{
		return TArrayEnumerator<T>(Array);
	}

	template<typename T>
	TArrayEnumerator<T> From(TSet<T> Set)
	{
		return TArrayEnumerator<T>(Set);
	}

	template<typename K, typename V>
	TMapEnumerator<K,V> From(TMap<K,V> Map)
	{
		return TMapEnumerator<K, V>(Map);
	}

	template <typename T>
	class TWhere : public TEnumerator<T>
	{
	public:
		TWhere(TEnumerator<T>* Enumerator, TFunction<bool(T)> Condition)
			: TEnumerator<T>(Enumerator),
			Condition(Condition)
		{
		}

		bool MoveNext() override
		{
			while (this->Parent->MoveNext())
			{
				if (Condition(this->Parent->Current()))
				{
					return true;
				}
			}
			return false;
		}

	private:
		TFunction<bool(T)> Condition;
	};

	template <typename T1, typename T2, typename R>
	class TJoin : public TEnumerator<T1>
	{
	public:
		TJoin(TEnumerator<T1>* Parent, TEnumerator<T2>* Another, TFunction<R(T1, T2)> Func) : TEnumerator<T1>(Parent),
			Another(Another),
			Func(Func)
		{
		}


		bool MoveNext() override
		{
			return this->Parent->MoveNext() && Another->MoveNext();
		}

		T1 Current() override
		{
			return Func(this->Parent->Current(), Another->Current());
		}

		void Reset() override
		{
			this->Parent->Reset();
			Another->Reset();
		}

	private:
		TEnumerator<T2>* Another;
		TFunction<R(T1, T2)> Func;
	};


	template <typename T>
	class TOfType : public TEnumerator<T>
	{
	public:

		TOfType(TEnumerator<T>* Parent, UClass* Class)
			: TEnumerator<T>(Parent),
			Class(Class)
		{
		}

		bool MoveNext() override
		{
			bool Result = false;
			do
			{
				Result = this->Parent->MoveNext();
				if (!Result)
				{
					break;
				}
			} while (Class != this->Current()->GetClass());
			return Result;
		}
	private:
		UClass* Class;
	};

	template <typename T, typename R>
	class TSelect : public TEnumerator<R>
	{
	public:
		TSelect(TEnumerator<T>* Enumerator, TFunction<R(T)> Func)
			: TEnumerator<R>(nullptr),
			ParentOldType(Enumerator),
			Func(Func)
		{
		}


		bool MoveNext() override
		{
			if (!ParentOldType)
			{
				return false;
			}
			return ParentOldType->MoveNext();
		}

		void Reset() override
		{
			ParentOldType->Reset();
		}

		R Current() override
		{
			return Func(this->ParentOldType->Current());
		}

	private:
		TEnumerator<T>* ParentOldType;
		TFunction<R(T)> Func;
	};

	template <typename T, typename R>
	class TCastTo : public TEnumerator<R>
	{
	public:
		TCastTo(TEnumerator<T>* Enumerator) :
			TEnumerator<R>(nullptr),
			ParentOldType(Enumerator)
		{
		}

		bool MoveNext() override
		{
			if (!ParentOldType)
			{
				return false;
			}
			return ParentOldType->MoveNext();
		}

		void Reset() override
		{
			ParentOldType->Reset();
		}

		R Current() override
		{
			return Cast<R>(this->Parent->Current());
		}
	private:
		TEnumerator<T>* ParentOldType;
	};


	template<typename T>
	class TArrayEnumerator : public TEnumerator<T>
	{
	public:
		TArrayEnumerator(TArray<T>& Array)
			: TEnumerator<T>(),
			Array(Array),
			CurrentIndex(-1)
		{
		}

		TArrayEnumerator(TSet<T>& Set)
			: TEnumerator<T>(),
			Array(Set.Array()),
			CurrentIndex(-1)
		{
		}

		bool MoveNext() override
		{
			if (CurrentIndex != Array.Num())
			{
				CurrentIndex++;
			}
			return CurrentIndex < Array.Num();
		}

		T Current() override
		{
			if(CurrentIndex < 0 || CurrentIndex == Array.Num() || Array.Num() == 0)
			{
				return TDefaultValue<T>::value;
			}
			return Array[CurrentIndex];
		}

		void Reset() override
		{
			CurrentIndex = -1;
		}
	private:
		TArray<T> Array;
		int CurrentIndex;
	};

	template<typename K, typename V>
	class TMapEnumerator : public TArrayEnumerator<TPair<K,V>>
	{
	public:
		TMapEnumerator(TMap<K,V>& Map)
			: TArrayEnumerator<TTuple<K, V>>(Map.Pairs.Array())
		{
		}

		TSelect<TPair<K,V>,K> Keys()
		{
			return TSelect<TPair<K, V>,K>(this, [](TPair<K,V> Kvp)->K
				{
					return Kvp.Key;
				});
		}

		TSelect<TPair<K, V>, V> Values()
		{
			return TSelect<TPair<K, V>, V>(this, [](TPair<K, V> Kvp)->V
				{
					return Kvp.Value;
				});
		}

	private:
		TArray<TPair<K, V>> Pairs;
	};
	
}
