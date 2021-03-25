#include "Utility/EnumUtil.h"


#include "CommonLib.h"

FString FEnumUtil::ExpandEnumString(const FString& name, const FString& enumName)
{
	FString expanded(name);
	FString spaceLetter("");
	FString spaceNumber("");
	FString search("");
	expanded.ReplaceInline(*enumName, TEXT(""), ESearchCase::CaseSensitive);
	expanded.ReplaceInline(TEXT("::"), TEXT(""), ESearchCase::CaseSensitive);
	for (TCHAR letter = 'A'; letter <= 'Z'; ++letter)
	{
		search = FString::Printf(TEXT("%c"), letter);
		spaceLetter = FString::Printf(TEXT(" %c"), letter);
		expanded.ReplaceInline(*search, *spaceLetter, ESearchCase::CaseSensitive);
	}
	for (TCHAR number = '0'; number <= '9'; ++number)
	{
		search = FString::Printf(TEXT("%c"), number);
		spaceNumber = FString::Printf(TEXT(" %c"), number);
		expanded.ReplaceInline(*search, *spaceNumber, ESearchCase::CaseSensitive);
	}
	expanded.ReplaceInline(TEXT("_"), TEXT(" -"), ESearchCase::CaseSensitive);
	expanded = expanded.RightChop(1).TrimStart().TrimEnd();
	return expanded;
}
