#include "API/AutomaticCommonHelper.h"


bool UAutomaticCommonHelper::IsClassOf(const UObject* Object, const UClass* Class)
{
	if (Object == nullptr || Class == nullptr)
	{
		return false;
	}

	if (Object->GetClass() == Class || Object->GetClass()->IsChildOf(Class))
	{
		return true;
	}

	return false;
}
