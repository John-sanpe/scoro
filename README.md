this project is called small coroutine

他本质上是一个协程库 目前调度算法使用fifo
首先创建一个pthread线程 然后在这个子线程里面运行协程

# API
1. `struct scoro_worker * scoro_worker_create(const char *name, ...)`
    1. 后期维护可变参数 当前未使用
    2. name为工作协程的名称
    3. 返回值为当前工作协程的唯一数据结构
2. `int scoro_worker_wakeup(struct scoro_worker *worker, scoro_pth_flag *flags)`
    1. 创建一个pthread 并将这个线程进入loop 供之后的任务队列进入
3. `struct scoro_work *scoro_work_create(scoro_fun_t fun, void *data, const char *name, ...)`
    1. 创建空间 返回一个work任务 : struct scoro_work 
4. `void scoro_work_enqueue(struct scoro_worker *worker, struct scoro_work *work)`
    1. 将work 加入到线程worker中