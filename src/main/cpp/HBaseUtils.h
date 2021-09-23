#ifndef CPP_HBASEUTILS_H
#define CPP_HBASEUTILS_H

#include <jni.h>
#include <cstdlib>

#ifdef _WIN32
#define PATH_SEPARATOR ';'
#else
#define PATH_SEPARATOR ':'
#endif


class HBaseUtils {
    jint ret = 0;
    JavaVM *jvm;
    JNIEnv *env;

    jclass cls;
    jmethodID mid;
    jstring jstr;

    /* 创建java虚拟机
    *	jvm : java虚拟机
    *	env	: JNI运行环境
    */
    jint create_VM(JavaVM **jvm, JNIEnv** env);
public:
    HBaseUtils(char* class_name) {
        ret = create_VM(&jvm, &env);
        if (ret < 0) {
            fprintf(stderr, "create_VM error!\n");
            exit(0);
        }

        cls = env->FindClass(class_name);
        if(cls == nullptr) {
            fprintf(stderr, "FindClass error!\n");
            exit(0);
        }
    }

    void put(const char* table_name, const char* row_key, long long start_location, char* data);

};

#endif //CPP_HBASEUTILS_H
