#ifndef CPP_HBASEUTILS_H
#define CPP_HBASEUTILS_H

#include <jni.h>
#include <cstdlib>
#include <string>
#include <iostream>


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
    jobject jobj;

    /* 创建java虚拟机
    *	jvm : java虚拟机
    *	env	: JNI运行环境
    */
    jint create_VM(JavaVM **jvm, JNIEnv** env);
public:
    HBaseUtils() {
        std::cout << "in cpp hbaseutils init" << std::endl;
        const std::string class_name = "HBaseUtils";
        ret = create_VM(&jvm, &env);
        if (ret < 0) {
            fprintf(stderr, "create_VM error!\n");
            exit(0);
        }

        cls = env->FindClass(class_name.c_str());
        if(cls == nullptr) {
            fprintf(stderr, "FindClass error!\n");
            exit(0);
        }
        jobj = create_HBaseUtils();
//        std::cout << "0000" << std::endl;
//        jobj=create_HBaseUtils();
//        std::cout << "1111" << std::endl;
    }

    jobject create_HBaseUtils();
//    void hello();
    void create_table(const char* table_name);
    void create_table(const char* table_name, const char* column_family);
    void put(const char* table_name, const char* row_key, long long start_location,const char* data);
    void put(const char* table_name, const char* row_key, const char* family,const char* column,long long start_location,const char* data);
    char* get(const char* table_name, const char* row_key, const char* family,const char* column,long long start_location);
    char* get(const char* table_name, const char* row_key, long long start_location);
    void delete_table(const char* table_name);
};

#endif //CPP_HBASEUTILS_H

#ifndef _Included_HBaseUtils
#define _Included_HBaseUtils
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     HBaseUtils
 * Method:    callbackHBaseUtils
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_HBaseUtils_callbackhello
        (JNIEnv * env);
JNIEXPORT void JNICALL Java_HBaseUtils_callbackHBaseUtils
        (JNIEnv *, jobject);

/*
 * Class:     HBaseUtils
 * Method:    callbackCreateTable
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_HBaseUtils_callbackCreateTable
        (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     HBaseUtils
 * Method:    callbackPut
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_HBaseUtils_callbackPut
        (JNIEnv *, jobject, jstring, jstring, jstring, jstring, jlong, jstring);

/*
 * Class:     HBaseUtils
 * Method:    callbackGet
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_HBaseUtils_callbackGet
        (JNIEnv *, jobject, jstring, jstring, jstring, jstring, jlong, jstring);

#ifdef __cplusplus
}
#endif
#endif
