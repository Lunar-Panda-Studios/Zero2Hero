// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Zero2Hero/Zero2HeroGameModeBase.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeZero2HeroGameModeBase() {}
// Cross Module References
	ZERO2HERO_API UClass* Z_Construct_UClass_AZero2HeroGameModeBase_NoRegister();
	ZERO2HERO_API UClass* Z_Construct_UClass_AZero2HeroGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_Zero2Hero();
// End Cross Module References
	void AZero2HeroGameModeBase::StaticRegisterNativesAZero2HeroGameModeBase()
	{
	}
	UClass* Z_Construct_UClass_AZero2HeroGameModeBase_NoRegister()
	{
		return AZero2HeroGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AZero2HeroGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AZero2HeroGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Zero2Hero,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AZero2HeroGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "Zero2HeroGameModeBase.h" },
		{ "ModuleRelativePath", "Zero2HeroGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AZero2HeroGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AZero2HeroGameModeBase>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AZero2HeroGameModeBase_Statics::ClassParams = {
		&AZero2HeroGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_AZero2HeroGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AZero2HeroGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AZero2HeroGameModeBase()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AZero2HeroGameModeBase_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AZero2HeroGameModeBase, 4009129590);
	template<> ZERO2HERO_API UClass* StaticClass<AZero2HeroGameModeBase>()
	{
		return AZero2HeroGameModeBase::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AZero2HeroGameModeBase(Z_Construct_UClass_AZero2HeroGameModeBase, &AZero2HeroGameModeBase::StaticClass, TEXT("/Script/Zero2Hero"), TEXT("AZero2HeroGameModeBase"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AZero2HeroGameModeBase);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
