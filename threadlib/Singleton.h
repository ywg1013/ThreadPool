//
// Created by 杨文广 on 2017/9/25.
//

#ifndef THREADPOOL_SINGLETON_H
#define THREADPOOL_SINGLETON_H

#define DECLARE_SINGLETON(CLASS)     \
public:                              \
    static CLASS* Instance(){        \
       static CLASS instance;        \
       return &instance;             \
}

#endif //THREADPOOL_SINGLETON_H
