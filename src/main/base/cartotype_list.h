/*
cartotype_list.h
Copyright (C) 2004-2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_types.h>
#include <cartotype_errors.h>

namespace CartoTypeCore
{

/** The base class for list items in the List and PointerList template classes. */
class ListLink
    {
    public:
    /**
    A pointer to the previous item in the list, or, if this is the first item,
    the last item in the list.
    */
    ListLink* Prev = nullptr;

    /**
    A pointer to the next item in the list, or, if this is the last item,
    the first item in the list.
    */
    ListLink* Next = nullptr;
    };

/** The base class for the List and PointerList template classes. */
class ListBase
    {
    public:
    ListBase() = default;
    /** Links aLink, which must not be null, into the list before aNext, or at the end of the list if aNext is null. Does not change the start item. */
    void Link(ListLink* aLink,ListLink* aNext);
    /** Inserts aLink, which must not be null, into the list before aNext. */
    void Insert(ListLink* aLink,ListLink* aNext);
    /** Deletes aLink, which must not be null. */
    void Delete(ListLink* aLink);
    /** Moves aLink to the start of the list. */
    void MoveToStart(ListLink* aLink)
        {
        if (aLink && aLink != iStart)
            {
            aLink->Prev->Next = aLink->Next;
            aLink->Next->Prev = aLink->Prev;
            aLink->Next = iStart;
            aLink->Prev = iStart->Prev;
            iStart->Prev->Next = aLink;
            iStart->Prev = aLink;
            iStart = aLink;
            }
        }

    /** Moves aLink to the position before aNext. */
    void MoveTo(ListLink* aLink,ListLink* aNext)
        {
        assert(aLink && aNext);
        if (aLink != aNext && aLink->Next != aNext)
            {
            aLink->Prev->Next = aLink->Next;
            aLink->Next->Prev = aLink->Prev;
            aLink->Next = aNext;
            aLink->Prev = aNext->Prev;
            aNext->Prev->Next = aLink;
            aNext->Prev = aLink;
            }
        if (aNext == iStart)
            iStart = aLink;
        }

    /** Returns the first item in the list, or null if the list is empty. */
    const ListLink* Start() const { return iStart; }
    /** Returns a non-const pointer to the first item in the list, or null if the list is empty. */
    ListLink* Start() { return iStart; }

    protected:
    /** The first link in the list. If the list is empty it is null. */
    ListLink* iStart = nullptr;
    /** The number of elements in the list. */
    int32_t iElements = 0;

    private:
    ListBase(const ListBase&) = delete;
    ListBase(ListBase&&) = delete;
    void operator=(const ListBase&) = delete;
    void operator=(ListBase&&) = delete;
    };

/** A list class designed to hold small objects directly. */
template<class T> class List: private ListBase
    {
    public:
    ~List()
        {
        Clear();
        }
    /** Deletes all objects from the list. */
    void Clear()
        {
        Link* p = (Link*)iStart;
        while (iElements > 0)
            {
            Link* next = (Link*)(p->Next);
            delete p;
            p = next;
            iElements--;
            }
        iStart = nullptr;
        }

    private:
    class Link: public ListLink
        {
        public:
        Link(const T& aT): iT(aT) { }
        T iT;
        };

    public:
    /**
    A non-constant iterator for traversing lists.
    The iterator's 'operator TYPE*' function returns a pointer to the iterator's current element,
    or null if the list is empty or the iterator has reached the end or start of the list.
    */
    class Iter
        {
        public:
        /** Creates an iterator referring to the link aLink. If aLink is null, the iterator is a null iterator and cannot be incremented or decremented. */
        Iter(ListBase& aList,ListLink* aLink): iList(&aList), iCur((Link*)aLink) { }
        /** Returns a pointer to the current item, or null if this is a null iterator. */
        operator T*() { return iCur ? &iCur->iT : 0 ; }
        /** Moves to the next item. The iterator becomes a null iterator if there are no more items. */
        void Next()
            {
            if (iCur)
                {
                iCur = (Link*)(iCur->Next);
                if (iCur == iList->Start())
                    iCur = nullptr;
                }
            }
        /** Moves to the previous item. The iterator becomes a null iterator if there are no more items. */
        void Prev()
            {
            if (iCur)
                {
                if (iCur == iList->Start())
                    iCur = nullptr;
                else
                    iCur = (Link*)(iCur->Prev);
                }
            }
        /** Returns a pointer to the current link. */
        Link* Cur() { return iCur; }
        /** Moves the current item to the start of the list. */
        void MoveCurrentToStart() { iList->MoveToStart(iCur); }
        /** Inserts the item aT before the current item. */
        Result Insert(const T& aT) { return iList->Insert(new Link(aT),iCur); }
        /** Moves the link aLink to the current position. */
        void MoveToCurrentPosition(ListLink* aLink) { iList->MoveTo(aLink,iCur); }
        /** Returns true if this iterator is at the start of the list. */
        bool AtStart() const { return iCur == iList->Start(); }

        private:
        ListBase* iList;
        Link* iCur;
        };

    /**
    A constant iterator for traversing lists.
    The iterator's 'operator const TYPE*' function returns a const pointer to the iterator's current element,
    or null if the list is empty or the iterator has reached the end or start of the list.
    */
    class ConstIter
        {
        public:
        /** Creates a constant iterator referring to the link aLink. If aLink is null, the iterator is a null iterator and cannot be incremented or decremented. */
        ConstIter(const ListBase& aList,const ListLink* aLink): iList(&aList), iCur((Link*)aLink) { }
        /** Returns the current item pointed to, or null if this is a null iterator. */
        operator const T*() const { return iCur ? &iCur->iT : 0 ; }
        /** Moves to the next item. The iterator becomes a null iterator if there are no more items. */
        void Next()
            {
            if (iCur)
                {
                iCur = (Link*)(iCur->iNext);
                if (iCur == iList->Start())
                    iCur = nullptr;
                }
            }
        /** Moves to the previous item. The iterator becomes a null iterator if there are no more items. */
        void Prev()
            {
            if (iCur)
                {
                if (iCur == iList->Start())
                    iCur = nullptr;
                else
                    iCur = (Link*)(iCur->Prev);
                }
            }
        /** Returns true if this iterator is at the start of the list. */
        bool AtStart() const { return iCur == iList->Start(); }

        private:
        const ListBase* iList;
        const Link* iCur;
        };

    /** Returns the number of elements in the list. */
    int32_t Count() const { return iElements; }
    /** Inserts a new element at the start of the list. */
    void Prefix(const T& aT) { ListBase::Insert(new Link(aT),iStart); }
    /** Inserts a new element at the end of the list. */
    void Append(const T& aT) { ListBase::Link(new Link(aT),iStart); }
    /** Deletes the element referred to by the iterator aIter. */
    void Delete(Iter& aIter)
        {
        ListLink* p = aIter.Cur(); assert(p); aIter.Next(); ListBase::Delete(p); delete p;
        }
    /** Returns a non-constant iterator referring to the first element of the list. */
    Iter First() { return List::Iter(*this,iStart); }
    /** Returns a non-constant iterator referring to the last element of the list. */
    Iter Last() { return List::Iter(*this,iStart ? iStart->Prev : 0); }
    /** Returns a constant iterator referring to the first element of the list. */
    ConstIter First() const { return List::ConstIter(*this,iStart); }
    /** Returns a constant iterator referring to the last element of the list. */
    ConstIter Last() const { return ConstIter(*this,iStart ? iStart->Prev : 0); }
    /** Returns a non-constant iterator referring to a specified link. */
    Iter IterAt(ListLink* aLink) { return Iter(*this,aLink); }
    /** Returns a constant iterator referring to a specified link. */
    ConstIter IterAt(const ListLink* aLink) { return ConstIter(*this,aLink); }
    };

/**
A list class for storing large objects via pointers.
The list takes ownership of the objects.
*/
template<class T> class PointerList: private ListBase
    {
    public:
    PointerList() = default;
    ~PointerList()
        {
        Clear();
        }
    /** Deletes all objects from the list. */
    void Clear()
        {
        Link* p = (Link*)iStart;
        while (iElements > 0)
            {
            Link* next = (Link*)(p->Next);
            delete (T*)(p->iPtr);
            delete p;
            p = next;
            iElements--;
            }
        iStart = nullptr;
        }

    private:
    class Link: public ListLink
        {
        public:
        Link(const T* aPtr): iPtr(aPtr) { }
        const T* iPtr;
        };

    public:
    /**
    A non-constant iterator for traversing lists of pointers.
    There are both const and non-const 'operator TYPE*' functions to return the iterator's current element.
    They return null if the list is empty or the iterator has reached the end or start of the list.
    */
    class Iter
        {
        public:
        /** Creates an iterator referring to the link aLink. If aLink is null, the iterator is a null iterator and cannot be incremented or decremented. */
        Iter(ListBase& aList,ListLink* aLink): iList(&aList), iCur((Link*)aLink) { }
        /** Returns a const pointer to the current item, or null if this is a null iterator. */
        operator const T*() { return iCur ? iCur->iPtr : 0 ; }
        /** Returns a pointer to the current item, or null if this is a null iterator. */
        operator T*() { return iCur ? (T*)(iCur->iPtr) : 0 ; }
        /** Moves to the next item. The iterator becomes a null iterator if there are no more items. */
        void Next()
            {
            if (iCur)
                {
                iCur = (Link*)(iCur->Next);
                if (iCur == iList->Start())
                    iCur = nullptr;
                }
            }
        /** Moves to the previous item. The iterator becomes a null iterator if there are no more items. */
        void Prev()
            {
            if (iCur)
                {
                if (iCur == iList->Start())
                    iCur = nullptr;
                else
                    iCur = (Link*)(iCur->Prev);
                }
            }
        /** Returns a pointer to the current link. */
        Link* Cur() { return iCur; }
        /** Moves the current item to the start of the list. */
        void MoveCurrentToStart() { iList->MoveToStart(iCur); }
        /** Inserts the item aT before the current item. */
        Result Insert(T* aPtr) { return iList->Insert(new Link(aPtr),iCur); }
        /** Moves the link aLink to the current position. */
        void MoveToCurrentPosition(ListLink* aLink) { iList->MoveTo(aLink,iCur); }
        /** Returns true if this iterator is at the start of the list. */
        bool AtStart() const { return iCur == iList->Start(); }

        private:
        ListBase* iList;
        Link* iCur;
        };
    /**
    A constant iterator for traversing lists of pointers.
    The iterator's 'operator const TYPE*' function returns a const pointer to the iterator's current element,
    or null if the list is empty or the iterator has reached the end or start of the list.
    */
    class ConstIter
        {
        public:
        /** Creates a constant iterator referring to the link aLink. If aLink is null, the iterator is a null iterator and cannot be incremented or decremented. */
        ConstIter(const ListBase& aList,const ListLink* aLink): iList(&aList), iCur((Link*)aLink) { }
        /** Returns a const pointer to the current item, or null if this is a null iterator. */
        operator const T*() const { return iCur ? iCur->iPtr : 0 ; }
        /** Moves to the next item. The iterator becomes a null iterator if there are no more items. */
        void Next()
            {
            if (iCur)
                {
                iCur = (Link*)(iCur->iNext);
                if (iCur == iList->Start())
                    iCur = nullptr;
                }
            }
        /** Moves to the previous item. The iterator becomes a null iterator if there are no more items. */
        void Prev()
            {
            if (iCur)
                {
                if (iCur == iList->Start())
                    iCur = nullptr;
                else
                    iCur = (Link*)(iCur->Prev);
                }
            }
        /** Returns true if this iterator is at the start of the list. */
        bool AtStart() const { return iCur == iList->Start(); }
        private:
        const ListBase* iList;
        const Link* iCur;
        };

    /** Returns the number of elements in the list. */
    int32_t Count() const { return iElements; }
    /** Inserts a new element at the start of the list. */
    void Prefix(const T* aPtr) { ListBase::Insert(new Link(aPtr),iStart); }
    /** Inserts a new element at the end of the list. */
    void Append(const T* aPtr) { ListBase::Link(new Link(aPtr),iStart); }
    /** Deletes the element referred to by the iterator aIter. */
    void Delete(Iter& aIter)
        {
        Link* p = aIter.Cur();
        assert(p);
        aIter.Next();
        ListBase::Delete(p);
        delete (T*)(p->iPtr);
        delete p;
        }
    /** Returns a non-constant iterator referring to the first element of the list. */
    Iter First() { return Iter(*this,iStart); }
    /** Returns a non-constant iterator referring to the last element of the list. */
    Iter Last() { return Iter(*this,iStart ? iStart->Prev : 0); }
    /** Returns a constant iterator referring to the first element of the list. */
    ConstIter First() const { return ConstIter(*this,iStart); }
    /** Returns a constant iterator referring to the last element of the list. */
    ConstIter Last() const { return ConstIter(*this,iStart ? iStart->Prev : 0); }
    /** Returns a non-constant iterator referring to a specified link. */
    Iter IterAt(ListLink* aLink) { return Iter(*this,aLink); }
    /** Returns a constant iterator referring to a specified link. */
    ConstIter IterAt(const ListLink* aLink) { return ConstIter(*this,aLink); }
    };

} // namespace CartoTypeCore
