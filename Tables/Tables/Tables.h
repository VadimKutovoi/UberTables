#pragma once
#include<stack>
#include<iostream>
#include<string>

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
	int dataCount, Eff, curr;
public:
	TTable()
	{
		dataCount = 0;
		Eff = 0;
	}
	bool IsEmpty() { return dataCount == 0; }
	virtual bool IsFull() = 0;
	virtual bool Find(TKey k) = 0;
	virtual void Insert(TRecord<TKey, TValue> tr) = 0;
	virtual void Reset() = 0;
	virtual bool IsEnd() = 0;
	virtual void GoNext() = 0;
	virtual TRecord<TKey, TValue> GetCurr() = 0;
	virtual void SetCurr(TValue val) = 0;
	void Print()
	{
		TRecord<TKey, TValue> tmp;
		int l, l2;

		std::cout << "+--------------------------+-------+" << std::endl;
		for (Reset(); !IsEnd();GoNext())
		{
			tmp = GetCurr();

			l = 25 - tmp.key.length();
			
			std::cout << "| "<< tmp.key;
			
			for (int i = 0; i < l; i++) std::cout << " ";

			std::cout << "|" << '\t' << tmp.value << "  |" <<std::endl;

			std::cout << "+--------------------------+-------+" << std::endl;
		}
	}
};

template <class TKey, class TValue>
class TArrayTable : public TTable<TKey, TValue>
{
protected:
	int curr, size;
	TRecord<TKey, TValue> *arr;
public:
	
	TArrayTable(int _size = 100)
	{
		size = _size;
		arr = new TRecord<TKey, TValue>[size];
		dataCount = 0;
		curr = 0;
	}

	~TArrayTable()
	{
		delete[] arr;
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

	TRecord<TKey, TValue> GetCurr() 
	{
		return arr[curr]; 
	}

	bool Find(TKey k) 
	{
		for (Reset(); !IsEnd(); GoNext()) 
		{
			if (arr[curr].key == k) 
				return true;	
		}
		return false;
	}
	
	void SetCurr(TValue val)
	{
		arr[curr].value = val;
	}

	void Insert(TRecord<TKey, TValue> tr) {
		if (!Find(tr.key)&&!IsFull()) {
			arr[dataCount] = tr;
			dataCount++;
		}
	}
	bool IsFull() { return size == dataCount; }
	void Reset() { curr = 0; }
	void GoNext() { curr++; }
	bool IsEnd() { return curr == dataCount; }
};

template<class TKey, class TValue>
class TScanTable : public TArrayTable<TKey, TValue>
{
public:
	TScanTable(int _size = 100)
	{
		size = _size;
		arr = new TRecord<TKey, TValue>[size];
		dataCount = 0;
		curr = 0;
	}

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

	void Insert(TRecord<TKey, TValue> tr)
	{
		if (!Find(tr.key))
		{
			arr[curr] = tr;
			dataCount++;
		}
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
public:

	TSortTable(int _size = 100) {
		size = _size;
		arr = new TRecord<TKey, TValue>[size];
		dataCount = 0;
		curr = 0;
	}

	int GetSize()
	{
		return size;
	}

	bool Find(TKey k)
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

	void Insert(TRecord<TKey, TValue> tr)
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
		}
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
			while (arr[i].key < mk) { i++; Eff++; }
			while (arr[j].key > mk) { j--; Eff++; }

			if(i <= j)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
		if (j > l) QSort(l, j);
		if (i < r)QSort(i, r);
	}
};

template<class TKey, class TValue>
struct TNode {
public:
	int balance;
	TRecord<TKey, TValue> rec;
	TNode<TKey, TValue> *pLeft, *pRight;

	TNode() {
		TRecord<TKey, TValue> t;
		rec = t;
		pLeft = pRight = nullptr;
		balance = 0;
	}

	TNode(TRecord<TKey, TValue> _record, TNode<TKey, TValue> *_pLeft = nullptr, TNode<TKey, TValue> *_pRight = nullptr) {
		record = _record;
		pLeft = _pLeft;
		pRight = _pRight;
	}
};

template<class TKey, class TValue>
class TTreeTable :public TTable<TKey, TValue> {
protected:
	TNode< TKey, TValue> *pRoot, *pCurr, **pRes;
	std::stack <TNode< TKey, TValue> *> st;
	int pos;
public:
	TTreeTable() { pRoot = pCurr = nullptr; pRes = nullptr; }
	
	bool IsFull() {
		return false;
	}

	bool Find(TKey key) {
		pRes = &pRoot;

		while (*pRes != nullptr) {
			Eff++;

			if ((*pRes)->rec.key== key) {
				return true;
			}
			else if ((*pRes)->rec.key < key) {
				pRes = &((*pRes)->pRight);
			}
			else {
				pRes = &((*pRes)->pLeft);
			}
		}

		return false;
	}
	void Insert(TRecord <TKey, TValue> tr) {
		if (!Find(tr.key)) {
			TNode<TKey, TValue>*tmp = new TNode<TKey, TValue>;

			tmp->rec = tr;
			tmp->pLeft = nullptr;
			tmp->pRight = nullptr;
			*pRes = tmp;
			dataCount++;
		}	
	}

	void Delete(TKey key) {
		if (Find(key)) {
			TNode *tmp = *pRes;
			if (tmp->pLeft == nullptr)
				*pRes = tmp->pRight;
			else if (tmp->pRight == nullptr)
					*pRes = tmp->pLeft;
			else {
				TNode<TKey, TValue> *p = tmp->pLeft;
				TNode<TKey, TValue> **prev = &(tmp->pLeft);

				while (p->pRight != nullptr) {
					prev = &(p->pRight);
					p = p->pRight;
					Eff++;
				}

				tmp->rec = p->rec;
				tmp = p;
				*prev = p->pLeft;
			}

			dataCount--;
			delete tmp;
					
		}
	}
	
	void Reset() {
		pos = 0;
		while (!st.empty())
			st.pop();
		pCurr = pRoot;
		
		while (pCurr->pLeft != nullptr)
		{
			st.push(pCurr);
			pCurr = pCurr->pLeft;
		}
		st.push(pCurr);
	}

	void GoNext() {
		st.pop();
		pos++;

		if (pCurr->pRight) {
			pCurr = pCurr->pRight;

			while (pCurr->pLeft != nullptr) {
				st.push(pCurr);
				pCurr = pCurr->pLeft;
			}
			st.push(pCurr);
		}
		else if (!st.empty()) pCurr = st.top();
	}

	bool IsEnd() {
		return pos == dataCount;
	}

	TRecord <TKey, TValue> GetCurr() {
		return pCurr->rec;
	}

	void SetCurr(TValue val) {
		pCurr->rec.value = val;
	}
};

template<class TKey, class TValue>
class THashTable :public TTable<TKey, TValue>
{
protected:
	int HashFunc(TKey key) {
		int pos = 0;
		for (int i = 0; i < key.length(); i++)
		{
			pos += key[i] << i;
		}
		return pos;
	}
};

template<class TKey, class TValue>
class TArrayHash :public THashTable<TKey, TValue>
{
protected:
	int size, step;
	TRecord<TKey, TValue> *arr;
public:
	TArrayHash(int _size = 100) {
		size = _size;
		arr = new TRecord<TKey, TValue>[size];
		step = 13;
		for (size_t i = 0; i < size; i++)
		{
			arr[i].key = "";
		}
	}
	~TArrayHash() {
		delete[] arr;
	}

	bool Find(TKey key) {
		curr = HashFunc(key) % size;
		int freepos = -1;

		for (int i = 0; i < size; i++) {
			Eff++;

			if (arr[curr].key == "") {
				if (freepos == -1) {
					return false;
				}
				else {
					curr = freepos;
					return false;
				}
			}

			if (arr[curr].key == key) {
				return true;
			}

			if ((freepos == -1) && (arr[curr].key == "&")) {
				freepos = curr;
			}

			curr = (curr + step) % size;
		}
		return false;
	}

	void Delete(TKey key)
	{
		if (Find(key)) {
			arr[curr] = "&";
			dataCount--;
		}
	}

	void Insert(TRecord <TKey, TValue> tr) {
		if (!Find(tr.key)) {
			arr[curr] = tr;
			dataCount++;
		}
	}
	void Reset() {
		curr = 0;
		while (((arr[curr].key == "&") || (arr[curr].key == "")) && (curr < size)) 
			curr++;
	}

	void GoNext() {
		while ((++curr < size)) {
			if (((arr[curr].key != "&") && (arr[curr].key != "")))
				break;
		}
	}

	bool IsEnd() {
		return curr >= size;
	}

	TRecord<TKey, TValue> GetCurr() {
		if (curr < 0 || curr >= size)
			throw "OUT_OF_RANGE";
		return arr[curr];
	}

	void SetCurr(TValue val) {
		arr[curr].value = val;
	}

	bool IsFull()
	{
		return false;
	}
};