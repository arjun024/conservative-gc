/* Do GC when @bytesAllocatedSinceLastGC breaches this */
#define GC_THRESHOLD 1048576

template <class SourceHeap>
GCMalloc<SourceHeap>::GCMalloc()
{

}

template <class SourceHeap>
void *GCMalloc<SourceHeap>::malloc(size_t sz)
{

}

template <class SourceHeap>
size_t GCMalloc<SourceHeap>::getSize(void * p)
{

}

template <class SourceHeap>
size_t GCMalloc<SourceHeap>::bytesAllocated()
{

}

template <class SourceHeap>
void GCMalloc<SourceHeap>::walk(const std::function< void(Header *) >& f)
{

}

template <class SourceHeap>
size_t GCMalloc<SourceHeap>::getSizeFromClass(int index)
{

}

template <class SourceHeap>
int GCMalloc<SourceHeap>::getSizeClass(size_t sz)
{

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
