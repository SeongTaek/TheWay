// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TheWayDataTable.h"
#include "DataTableManager.generated.h"

template<typename TRow>
struct TRowKeyField
{
	template<typename TRow, typename TEnableIf<TIsPointer<TRow>::Value, bool>::Type = true>
	static auto Get(const TRow& InRow) -> decltype(InRow->Id)
	{
		return InRow->Id;
	}

	template<typename TRow, typename TEnableIf<TNot<TIsPointer<TRow>>::Value, bool>::Type = true>
	static auto Get(const TRow& InRow) -> decltype(InRow.Id)
	{
		return InRow.Id;
	}
};

// FWeaponStatsData는 Id, Level로 키값 사용
template<>
struct TRowKeyField<FWeaponStatsData>
{
	static TPair<int32, int32> Get(const FWeaponStatsData& InRow)
	{
		return TPair<int32, int32>(InRow.Id, InRow.Level);
	}

	static TPair<int32, int32> Get(const FWeaponStatsData* InRow)
	{
		return TPair<int32, int32>(InRow->Id, InRow->Level);
	}
};


// FSpawnerData는 Id, Level로 키값 사용
template<>
struct TRowKeyField<FSpawnerData>
{
	static TPair<int32, int32> Get(const FSpawnerData& InRow)
	{
		return TPair<int32, int32>(InRow.Id, InRow.Level);
	}

	static TPair<int32, int32> Get(const FSpawnerData* InRow)
	{
		return TPair<int32, int32>(InRow->Id, InRow->Level);
	}
};

template<typename TRow>
struct TRowKeyType
{
	typedef decltype(TRowKeyField<TRow>::Get(DeclVal<TRow>())) type;
};

struct FDataTableBase
{
	virtual ~FDataTableBase() 
	{
	}
};

template<typename TRow>
struct FDataTable : public FDataTableBase
{
	typedef typename TRowKeyType<TRow>::type TRowKey;
	TMap<TRowKey, TSharedPtr<TRow>> RowMap;
};

/**
 * 
 */
UCLASS(Config = Game)
class UDataTableManager : public UObject
{
	GENERATED_BODY()
	
public:
	UDataTableManager();

	static UDataTableManager* Get(const UObject* InWorldContextObj);
	static UDataTableManager* Get(const class UTheWayGameInstance* InGameInstance);

public:
	void Init();

public:
	template<typename TRow>
	void AddDataTable(const TArray<TRow>& InArray)
	{
		typedef typename TRowKeyType<TRemovePointer<TRow>::Type>::type TRowKey;
		TSharedRef<FDataTable<TRemovePointer<TRow>::Type>> NewTable = MakeShared<FDataTable<TRemovePointer<TRow>::Type>>();

		for (const TRow& TableRow : InArray)
		{
			if constexpr (TIsPointer<TRow>::Value)
			{
				NewTable->RowMap.Add(TRowKeyField<TRemovePointer<TRow>::Type>::Get(TableRow), MakeShareable(new TRemovePointer<TRow>::Type(*TableRow)));
			}
			else
			{
				NewTable->RowMap.Add(TRowKeyField<TRemovePointer<TRow>::Type>::Get(TableRow), MakeShareable(new TRow(TableRow)));
			}
		}

		WholeDataTables.Add(TRemovePointer<TRow>::Type::StaticStruct(), StaticCastSharedRef<FDataTableBase>(NewTable));
	}

	template<typename TRow>
	const TMap<typename TRowKeyType<TRow>::type, TSharedPtr<TRow>>& GetDataTable() const
	{
		typedef typename TRowKeyType<TRow>::type TRowKey;
		const TSharedPtr<FDataTableBase>* FoundTable = WholeDataTables.Find(TRow::StaticStruct());

		if (FoundTable)
		{
			return StaticCastSharedPtr<const FDataTable<TRow>>(*FoundTable)->RowMap;
		}
		else
		{
			// 인터페이스를 맞추기 위해 더미 만들어서 반환
			static TMap<TRowKey, TSharedPtr<TRow>> Dummy;
			return Dummy;
		}
	}

	template<typename TRow>
	const TSharedPtr<const TRow> GetTableCsv(const typename TRowKeyType<TRow>::type& Id) const
	{
		typedef typename TRowKeyType<TRow>::type TRowKey;
		const TMap<TRowKey, TSharedPtr<TRow>>& DataTable = GetDataTable<TRow>();
		const TSharedPtr<TRow>* FoundRow = DataTable.Find(Id);

		return FoundRow ? *FoundRow : nullptr;
	}

	template<typename TRow>
	const bool HasTableCsv(const typename TRowKeyType<TRow>::type& Id) const
	{
		return GetTableCsv<TRow>(Id) != nullptr;
	}

protected:
	TMap<UScriptStruct*, TSharedPtr<FDataTableBase>> WholeDataTables;
};
