#pragma once
#include <Windows.h>
#include "API\ARK\Ark.h"

struct FConfigCacheIniNew
{
	//FieldValue<FConfigCacheIniVtbl *> vfptrField() { return { this, "FConfigCacheIni.vfptr" }; }
	FieldValue<bool> bAreFileOperationsDisabledField() { return { this, "FConfigCacheIni.bAreFileOperationsDisabled" }; }
	FieldValue<bool> bIsReadyForUseField() { return { this, "FConfigCacheIni.bIsReadyForUse" }; }

	// Functions

	//FConfigFile * Find(FString * Filename, bool CreateIfNotFound) { return NativeCall<FConfigFile *, FString *, bool>(this, "FConfigCacheIni.Find", Filename, CreateIfNotFound); }
	void Flush(bool Read, FString * Filename) { NativeCall<void, bool, FString *>(this, "FConfigCacheIni.Flush", Read, Filename); }
	void DisableFileOperations() { NativeCall<void>(this, "FConfigCacheIni.DisableFileOperations"); }
	void EnableFileOperations() { NativeCall<void>(this, "FConfigCacheIni.EnableFileOperations"); }
	bool AreFileOperationsDisabled() { return NativeCall<bool>(this, "FConfigCacheIni.AreFileOperationsDisabled"); }
	void Parse1ToNSectionOfNames(const wchar_t * Section, const wchar_t * KeyOne, const wchar_t * KeyN, TMap<FName, TArray<FName>, FDefaultSetAllocator, TDefaultMapKeyFuncs<FName, TArray<FName>, 0> > * OutMap, FString * Filename) { NativeCall<void, const wchar_t *, const wchar_t *, const wchar_t *, TMap<FName, TArray<FName>, FDefaultSetAllocator, TDefaultMapKeyFuncs<FName, TArray<FName>, 0> > *, FString *>(this, "FConfigCacheIni.Parse1ToNSectionOfNames", Section, KeyOne, KeyN, OutMap, Filename); }
	void Parse1ToNSectionOfStrings(const wchar_t * Section, const wchar_t * KeyOne, const wchar_t * KeyN, TMap<FString, TArray<FString>, FDefaultSetAllocator, TDefaultMapKeyFuncs<FString, TArray<FString>, 0> > * OutMap, FString * Filename) { NativeCall<void, const wchar_t *, const wchar_t *, const wchar_t *, TMap<FString, TArray<FString>, FDefaultSetAllocator, TDefaultMapKeyFuncs<FString, TArray<FString>, 0> > *, FString *>(this, "FConfigCacheIni.Parse1ToNSectionOfStrings", Section, KeyOne, KeyN, OutMap, Filename); }
	bool GetString(const wchar_t * Section, const wchar_t * Key, FString * Value, FString * Filename) { return NativeCall<bool, const wchar_t *, const wchar_t *, FString *, FString *>(this, "FConfigCacheIni.GetString", Section, Key, Value, Filename); }
	bool GetSection(const wchar_t * Section, TArray<FString> * Result, FString * Filename) { return NativeCall<bool, const wchar_t *, TArray<FString> *, FString *>(this, "FConfigCacheIni.GetSection", Section, Result, Filename); }
	//FConfigSection * GetSectionPrivate(const wchar_t * Section, bool Force, bool Const, FString * Filename) { return NativeCall<FConfigSection *, const wchar_t *, bool, bool, FString *>(this, "FConfigCacheIni.GetSectionPrivate", Section, Force, Const, Filename); }
	void SetString(const wchar_t * Section, const wchar_t * Key, const wchar_t * Value, FString * Filename) { NativeCall<void, const wchar_t *, const wchar_t *, const wchar_t *, FString *>(this, "FConfigCacheIni.SetString", Section, Key, Value, Filename); }
	void RemoveKey(const wchar_t * Section, const wchar_t * Key, FString * Filename) { NativeCall<void, const wchar_t *, const wchar_t *, FString *>(this, "FConfigCacheIni.RemoveKey", Section, Key, Filename); }
	void EmptySection(const wchar_t * Section, FString * Filename) { NativeCall<void, const wchar_t *, FString *>(this, "FConfigCacheIni.EmptySection", Section, Filename); }
	bool GetSectionNames(FString * Filename, TArray<FString> * out_SectionNames) { return NativeCall<bool, FString *, TArray<FString> *>(this, "FConfigCacheIni.GetSectionNames", Filename, out_SectionNames); }
	
	bool GetFloat(const wchar_t * Section, const wchar_t * Key, float * Value, FString * Filename) { return NativeCall<bool, const wchar_t *, const wchar_t *, float *, FString *>(this, "FConfigCacheIni.GetFloat", Section, Key, Value, Filename); }
	bool GetBool(const wchar_t * Section, const wchar_t * Key, bool * Value, FString * Filename) { return NativeCall<bool, const wchar_t *, const wchar_t *, bool *, FString *>(this, "FConfigCacheIni.GetBool", Section, Key, Value, Filename); }
	int GetArray(const wchar_t * Section, const wchar_t * Key, TArray<FString> * out_Arr, FString * Filename) { return NativeCall<int, const wchar_t *, const wchar_t *, TArray<FString> *, FString *>(this, "FConfigCacheIni.GetArray", Section, Key, out_Arr, Filename); }
	bool GetVector(const wchar_t * Section, const wchar_t * Key, FVector * Value, FString * Filename) { return NativeCall<bool, const wchar_t *, const wchar_t *, FVector *, FString *>(this, "FConfigCacheIni.GetVector", Section, Key, Value, Filename); }
	void SetInt(const wchar_t * Section, const wchar_t * Key, int Value, FString * Filename) { NativeCall<void, const wchar_t *, const wchar_t *, int, FString *>(this, "FConfigCacheIni.SetInt", Section, Key, Value, Filename); }
	void SetFloat(const wchar_t * Section, const wchar_t * Key, float Value, FString * Filename) { NativeCall<void, const wchar_t *, const wchar_t *, float, FString *>(this, "FConfigCacheIni.SetFloat", Section, Key, Value, Filename); }
	void SetArray(const wchar_t * Section, const wchar_t * Key, TArray<FString> * Value, FString * Filename) { NativeCall<void, const wchar_t *, const wchar_t *, TArray<FString> *, FString *>(this, "FConfigCacheIni.SetArray", Section, Key, Value, Filename); }
	void SetVector(const wchar_t * Section, const wchar_t * Key, FVector Value, FString * Filename) { NativeCall<void, const wchar_t *, const wchar_t *, FVector, FString *>(this, "FConfigCacheIni.SetVector", Section, Key, Value, Filename); }
	unsigned __int64 GetMaxMemoryUsage() { return NativeCall<unsigned __int64>(this, "FConfigCacheIni.GetMaxMemoryUsage"); }
	static bool LoadGlobalIniFile(FString * FinalIniFilename, const wchar_t * BaseIniName, const wchar_t * Platform, const wchar_t * GameName, bool bForceReload, bool bRequireDefaultIni, bool bAllowGeneratedIniWhenCooked, const wchar_t * GeneratedConfigDir) { return NativeCall<bool, FString *, const wchar_t *, const wchar_t *, const wchar_t *, bool, bool, bool, const wchar_t *>(nullptr, "FConfigCacheIni.LoadGlobalIniFile", FinalIniFilename, BaseIniName, Platform, GameName, bForceReload, bRequireDefaultIni, bAllowGeneratedIniWhenCooked, GeneratedConfigDir); }
};