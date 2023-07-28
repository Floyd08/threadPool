# threadPool

This is a thread pool I've written, anticipating the need for one in another project(Alchemy). 

This was also a study project. I wrote a thread pool in C for an operating systems course at York, and I enjoyed that enough that I felt it would be a good crash course in C++
It was!

You might notice that the pool will only accept tasks of a single type, assigned at creation of the pool. I've done this for simplicity, expecting that my use case will be to execute many identical tasks in parallel.
It is also neccesary to bind parameters to the packaged_task before submition to the task queue, and I've imposed this requirement to avoid the need for parameter packs.
