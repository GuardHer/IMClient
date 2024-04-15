## QThread

1. **构造函数：**
   - `QThreadPool(QObject *parent = nullptr)`：构造函数，可以指定父对象。
2. **线程池控制函数：**
   - `void setMaxThreadCount(int maxThreadCount)`：设置线程池的最大线程数。
   - `int maxThreadCount() const`：返回线程池的最大线程数。
   - `void setExpiryTimeout(int expiryTimeout)`：设置线程的过期时间（空闲时间），超过此时间的线程将被终止。
   - `int expiryTimeout() const`：返回线程的过期时间。
3. **任务管理函数：**
   - `void start(QRunnable *runnable, int priority = 0)`：将任务提交到线程池，并指定优先级。
   - `void submit(QRunnable *runnable)`：提交任务到线程池，类似于 `start`，但不支持设置优先级。
   - `void waitForDone()`：等待所有任务完成。
   - `int activeThreadCount() const`：返回当前处于活动状态的线程数量。
4. **全局线程池函数：**
   - `static QThreadPool *globalInstance()`：返回全局线程池的实例。可以使用这个实例在应用程序中共享线程池。
5. **取消任务函数：**
   - `void clear()`：清空线程池中的任务队列，但不终止当前正在执行的任务。
   - `void cancel(QRunnable *runnable)`：取消指定的任务。
6. **信号：**
   - `void started(QRunnable *runnable)`：当任务开始执行时触发的信号。
   - `void finished(QRunnable *runnable)`：当任务完成执行时触发的信号。
7. **其他函数：**
   - `bool tryStart(QRunnable *runnable)`：尝试将任务提交到线程池，如果线程池未满，则开始执行任务并返回 `true`。





## QRunnable 

> `QRunnable` 是 Qt 中用于表示可在线程中执行的任务的基类。它提供了一种在多线程环境中执行工作的机制，与 `QThread` 不同，`QRunnable` 使得任务的执行可以更灵活地与线程进行关联，而无需直接管理线程的生命周期。

1. **构造函数：**
   - `QRunnable()`：默认构造函数。
2. **任务执行函数：**
   - `virtual void run()`：纯虚函数，表示任务的执行逻辑。在你的子类中实现这个函数，定义任务的具体行为。
3. **优先级设置函数：**
   - `void setAutoDelete(bool autoDelete)`：设置任务是否在执行完成后自动删除。如果设置为 `true`，任务对象将在执行完成后自动被删除；如果设置为 `false`，任务对象将不会自动删除，需要手动管理其生命周期。
   - `bool autoDelete() const`：返回任务是否自动删除的状态。
4. **任务执行状态：**
   - `bool isFinished() const`：返回任务是否已完成。
   - `bool isAutoDelete() const`：返回任务是否会自动删除。
5. **任务执行进度：**
   - `void setProgressRange(int minimum, int maximum)`：设置任务的进度范围。
   - `void setProgressValue(int progressValue)`：设置任务的当前进度值。



## QEventLoop

`QEventLoop` 是 Qt 中用于处理事件的类，它通常与事件循环（event loop）相关联。事件循环是一种机制，它允许应用程序等待和响应事件（如用户输入、定时器事件等）而不阻塞主线程。`QEventLoop` 可以用于在程序中创建自己的事件循环。

以下是 `QEventLoop` 的一些主要函数和用法：

1. **构造函数：**
   - `QEventLoop(QObject *parent = Q_NULLPTR)`：构造函数，可以指定父对象。
2. **开始事件循环：**
   - `int exec(QFlags<LoopType> flags = AllEvents)`：开始事件循环，直到 `exit()` 被调用或者 `quit()` 被调用。`flags` 参数可以指定事件循环的类型。
3. **退出事件循环：**
   - `void exit(int returnCode = 0)`：退出事件循环，并指定返回码。返回码可以在调用 `exec()` 的地方获取。
4. **退出事件循环并延迟执行：**
   - `void quit()`：退出事件循环，但是不立即返回，而是在当前事件循环迭代结束后返回。
5. **等待信号或事件：**
   - `void processEvents(QFlags<QEventLoop::ProcessEventsFlag> flags = QEventLoop::AllEvents)`：处理挂起的事件和等待新的事件，直到没有更多事件要处理为止。
6. **定时等待：**
   - `int exec(QTimer *timer, QFlags<LoopType> flags = AllEvents)`：在指定的时间间隔内等待事件，类似于 `exec()`。
7. **单次事件循环迭代：**
   - `bool processEvents(QFlags<QEventLoop::ProcessEventsFlag> flags = QEventLoop::AllEvents)`：执行单次事件循环迭代，处理挂起的事件。
8. **在事件循环中执行代码：**
   - `void exec(QRunnable *runnable)`：在事件循环中执行指定的任务（`QRunnable` 对象），类似于 `QCoreApplication::postEvent()`。
9. **线程相关：**
   - `static QThread *currentThread()`：返回当前事件循环所在的线程。
   - `QThread *thread() const`：返回与当前事件循环关联的线程。

`QEventLoop` 通常与 `QCoreApplication` 或者 `QApplication` 一起使用，以处理整个应用程序的事件循环。它也可以在单独的线程中创建，用于处理该线程的事件循环。