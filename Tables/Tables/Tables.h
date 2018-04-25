#pragma once

template <class TKey, class TValue>
struct TRecord
{
	TKey key;
	TValue value;
};

template <class TKey, class TValue>
class TTable
{
protected:
	int dataCount, eff;
public:
	TTable()
	{
		dataCount = 0;
		eff = 0;
	}
	bool IsEmpty() { return dataCount == 0; }
	virtual void IsFull() = 0;
	virtual bool Find(TKey k) = 0;
	virtual bool Insert(TRecord<TKey, TValue> tr) = 0;
	virtual void Reset() = 0;
	virtual bool IsEnd() = 0;
	virtual void GoNext() = 0;
	virtual TRecord<TKey, TValue> GetCurrent() = 0;
	virtual void SetCurrValue(TValue val) = 0;
	void Print()
	{
		TRecord<TKey, TValue> tmp;
		for (Reset(); !IsEnd();GoNext())
		{
			tmp = GetCurrent();
			cout << tmp.value << endl;
		}
	}
};

template <class TKey, class TValue>
class TArrayTable : public TTable<TKey, TValue>
{
	TRecord<TKey, TValue> *arr;
	int dataCount, curr;

public:
	TArrayTable()
	{
		arr = new TRecord[100];
		dataCount = 0;
		curr = 0;
	}

	~TArrayTable()
	{
		delete[] arr;
	}

	TArrayTable(TArrayTable table)
	{
		dataCount = table.dataCount;
		curr = table.curr;
		arr = new TRecord[100];
		
		for (int i = 0; i < dataCount; i++)
			arr[i] = table.arr[i];
	}

	TArrayTable* operator =(TArrayTable table)
	{
		dataCount = table.dataCount;
		curr = table.curr;
		arr = new TRecord[100];

		for (int i = 0; i < dataCount; i++)
			arr[i] = table.arr[i];
		
		return &this;
	}

	int GetCurr() { return curr; }

	void Reset() { curr = 0; }
	void GoNext() { curr++; }
	void IsEnd() { return curr == dataCount; }
};

template<class TKey, class TValue>
class TScanTable : public TArrayTable<TKey, TValue>
{
public:
	TScanTable()::TArrayTable();

	bool Find(TKey k)
	{
		for(int i = 0; i < dataCount; i++)
			if (arr[i].key == k) {
				curr = i;
				return true;
			}
		Eff++;
		curr = dataCount;
		return false;
	}

	void Insert(TRecord<Tkey, TValue> tr)
	{
		if (!Find(tr.key))
		{
			arr[curr] = tr;
			dataCount++;
			return true;
		}
		return false;
	}

	void Delete(TKey k)
	{
		if (Find(k))
		{
			arr[curr] = arr[dataCount--];
			dataCount--;
		}
	}
};

template<class TKey, class TValue>
class TSortTable : public TArrayTable<TKey, TValue>
{
	bool Find(Tkey k)
	{
		int l = 0, r = dataCount - 1, m;

		while (l <= r)
		{
			m = (r + l) / 2;
			if (arr[m].key == k)
			{
				curr = m;
				return true;
			}
			if (arr[m].key < k) l++;
			else r--;
		}
		curr = l;
		return false;
	}

	bool Insert(TRecord<TKey, TValue> tr)
	{
		if (!Find(tr.key))
		{
			for (int i = dataCount; i > curr; i--)
			{
				arr[i] = arr[i - 1];
				Eff++;
			}
			dataCount++;
			arr[curr] = tr;
			return true;
		}
		return false;
	}

	void Delete()
	{
		if (Find(tr.key))
		{
			for (int i = curr; i < dataCount; i++)
			{
				arr[i] = arr[i + 1];
				Eff++;
			}
			dataCount--;
		}
	}

	void QSort(int l, int r)
	{
		int m = (r + l) / 2;
		int i = l, j = r;
		TKey mk = arr[m].key;
		TRecord<TKey, TValue> tmp;

		while (i <= j)
		{
			while(arr[i].key < mk){i++, Eff++}
			while (arr[j].key > mk) { j--, Eff++ }

			if(i <= j)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
		if (j > l) QSort(l, j);
		if (i < r)Qsort(i, r);
	}
};