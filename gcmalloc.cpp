/* Do GC when @bytesAllocatedSinceLastGC breaches this */
#define GC_THRESHOLD 524288

#define LIMIT_16KB 16384
#define LIMIT_512MB 536870912
#define CLASS_16KB 1024
#define CLASS_512MB 1039
#define LOG_LIMIT_16KB 14

/***********************************************************************************************
 This part deals with aligning all memory chunks to the boundary defined by the enum Alignment.
 The class-sizes of the segregated memory allocator does not take header size into consideration.
 Since it is always a multiple of enum Alignment, it is always aligned.
 Now it is only required to add an offset to the header's size to make the header always aligned.
 Making both the header and usable-memory independently aligned makes the allocator very portable
 and easy to maintain.
 Calculation of offset is inspired by Doug Lea's memory allocator.
 ************************************************************************************************/

#define MEM_ALIGN_MASK (Alignment - (size_t)1)

#define is_aligned(X) (((size_t)((X)) & (MEM_ALIGN_MASK)) == 0)

#define calc_align_offset(X) \
  (is_aligned(X) ? 0 : \
    ((Alignment - ((size_t)(X) & MEM_ALIGN_MASK)) & MEM_ALIGN_MASK))

#define HEADER_ORIG_SIZE sizeof(Header)

#define HEADER_ALIGNED_SIZE (HEADER_ORIG_SIZE + calc_align_offset(HEADER_ORIG_SIZE))

/***********************************************************************************************/




template <class SourceHeap>
GCMalloc<SourceHeap>::GCMalloc()
	: bytesAllocatedSinceLastGC (0),
	bytesReclaimedLastGC (0),
	objectsAllocated (0),
	allocated (0),
	allocatedObjects (NULL),
	inGC (false),
	nextGC (GC_THRESHOLD)
 {

	startHeap = endHeap = SourceHeap::getStart();
	for (auto& f : freedObjects) {
	        f = NULL;
	}
 }

template <class SourceHeap>
void *GCMalloc<SourceHeap>::malloc(size_t sz) //set cookie, init clear(), ObjectsAllocated
{
	int class_index;
	size_t rounded_sz, total_sz;
	void *heap_mem;
	Header *mem_chunk;

	class_index = getSizeClass(sz);
	if (class_index < 0)
		return NULL;

	/* round to the next class size */
	rounded_sz = getSizeFromClass(class_index);
	if (!rounded_sz)
		return NULL;

	/* We try to get memory from the free list.
	* If the corresponding free list has no chunks left,
	* we look for memory from the SourceHeap.
	*/
	heapLock.lock();
	mem_chunk = freedObjects[class_index];
	if(mem_chunk) {
		/*remove the first chunk from this free list and give
		* it to the caller */
		freedObjects[class_index] = mem_chunk->nextObject;
		if (mem_chunk->nextObject) {
			freedObjects[class_index]->prevObject = NULL;
		}
		goto out;
	}

	total_sz = HEADER_ALIGNED_SIZE + rounded_sz;
	heap_mem = SourceHeap::malloc(total_sz);
	if (!heap_mem) {
		perror("Out of Memory!!");
		heapLock.unlock();
		return NULL;
	}
	mem_chunk = (Header*) heap_mem;
	mem_chunk->setAllocatedSize(rounded_sz);

out:
	/* Connect it to the doubly linked list tailed by @allocatedObjects */
	if (!allocatedObjects) {
		allocatedObjects = mem_chunk;
		mem_chunk->prevObject = mem_chunk->nextObject = NULL;
	} else {
		allocatedObjects->nextObject = mem_chunk;
		mem_chunk->prevObject = allocatedObjects;
		mem_chunk->nextObject = NULL;
		allocatedObjects = mem_chunk;
	}

	/* A little stats */
	allocated += rounded_sz;
	heapLock.unlock();
	return (void*)((char*)mem_chunk + HEADER_ALIGNED_SIZE);
}

template <class SourceHeap>
size_t GCMalloc<SourceHeap>::getSize(void *p)
{
	Header *header;
	header = (Header*)((char*)p - HEADER_ALIGNED_SIZE);
	return header->getAllocatedSize();
}

template <class SourceHeap>
size_t GCMalloc<SourceHeap>::bytesAllocated()
{
	/* No need of locks, this is read only */
	return allocated;
}

template <class SourceHeap>
void GCMalloc<SourceHeap>::walk(const std::function< void(Header *) >& f)
{
	Header *tmp;
	tmp = allocatedObjects;
	while (tmp) {
		f(tmp);
		/* @allocatedObjects follows the tail, so we go backwards */
		tmp = tmp->prevObject;
	}
}

template <class SourceHeap>
size_t GCMalloc<SourceHeap>::getSizeFromClass(int index)
{
	size_t class_sz;
	if (index <= CLASS_16KB)
		return (size_t)(index * 16);
	class_sz = (size_t) pow(2, index - CLASS_16KB + LOG_LIMIT_16KB);
	/* check for overflow */
	if (!class_sz) {
		perror("Out of memory!");
		return 0;
	}
	return class_sz;
}

template <class SourceHeap>
int constexpr GCMalloc<SourceHeap>::getSizeClass(size_t sz) //TODO: log2
{
	/* The size of sizeClass excludes the header size */

	if (sz <= 0 || sz > LIMIT_512MB)
		return -1;

	/* 
	* problem says: Your size classes should be exact multiples of 16
	* for every size up to 16384, and then powers of two from 16,384 to 512 MB
	* Class0 is kept unused for easy calculations.
	* Class1 to Class1039 is valid.
	*/

	if (sz <= LIMIT_16KB)
		return (int) ceil(sz / 16.0);

	/* Upto LIMIT_16KB, there are CLASS_16KB classes */
	return (int) ceil(log2(sz)) - LOG_LIMIT_16KB + CLASS_16KB;
}


/* private: */

template <class SourceHeap>
void GCMalloc<SourceHeap>::scan(void * start, void * end)
{

}

template <class SourceHeap>
bool GCMalloc<SourceHeap>::triggerGC(size_t szRequested)
{

}

template <class SourceHeap>
void GCMalloc<SourceHeap>::gc()
{

}

template <class SourceHeap>
void GCMalloc<SourceHeap>::mark()
{

}

template <class SourceHeap>
void GCMalloc<SourceHeap>::markReachable(void * ptr)
{

}

template <class SourceHeap>
void GCMalloc<SourceHeap>::sweep()
{

}

template <class SourceHeap>
void GCMalloc<SourceHeap>::privateFree(void *)
{

}

template <class SourceHeap>
bool GCMalloc<SourceHeap>::isPointer(void * p)
{

}
