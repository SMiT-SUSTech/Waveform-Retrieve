#include "HBaseUtils.h"
#include "jni.h"

/* 创建java虚拟机
 *	jvm : java虚拟机
 *	env	: JNI运行环境
 */
jint HBaseUtils::create_VM(JavaVM **jvm, JNIEnv** env) {
    JavaVMInitArgs args;
    JavaVMOption options[1];

    args.version = JNI_VERSION_1_8;  //	java版本
    args.nOptions = 1;
    options[0].optionString = "-Djava.class.path=./";  	// 配置java类的目录为当前目录
    args.options = options;
    args.ignoreUnrecognized = JNI_FALSE;

    return JNI_CreateJavaVM(jvm, (void **)env, &args);  // 创建java虚拟机
}

JNIEXPORT void JNICALL Java_HBaseUtils_callbackHBaseUtils
        (JNIEnv * env){
    jclass jclazz = env->FindClass("HBaseUtils");
    jmethodID jmethodId = env->GetMethodID(jclazz,"HBaseUtils","()V");
    env->CallVoidMethod(NULL,jmethodId);
}

JNIEXPORT void JNICALL Java_HBaseUtils_callbackCreateTable
        (JNIEnv * env,jstring jstrT , jstring jstrF){
    jclass jclazz = env->FindClass("HBaseUtils");
    if (jclazz != nullptr) {
        std::cout << "create table find HBaseUtils" << std::endl;
    } else {
        std::cout << "not find HBaseUtils" << std::endl;
    }
    jmethodID jmethodId = env->GetStaticMethodID(jclazz,"createTable","(Ljava/lang/String;Ljava/lang/String;)V");
    if (jmethodId != nullptr) {
        std::cout << "jmethodId not null" << std::endl;
    }
    const char *cStrT =NULL;
    const char *cStrF =NULL;
    jboolean isCopy=JNI_TRUE;
    cStrT = env->GetStringUTFChars(jstrT,&isCopy);
    cStrF = env->GetStringUTFChars(jstrF,&isCopy);
    env->CallStaticVoidMethod(jclazz,jmethodId,cStrT,cStrF);
}

JNIEXPORT void JNICALL Java_HBaseUtils_callbackPut
        (JNIEnv * env, jstring jTableName, jstring jRowKey, jstring jFamily, jstring jColumn, jlong jStartLocation, jstring jData){
    jclass jclazz = env->FindClass("HBaseUtils");
    jmethodID jmethodId = env->GetStaticMethodID(jclazz,"put","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;)V");
    const char *cTableName =NULL;const char *cRowKey =NULL;const char *cFamily =NULL;const char *cColumn=NULL;const char *cData=NULL;
    jboolean isCopy=JNI_TRUE;
    cTableName = env->GetStringUTFChars(jTableName,&isCopy);
    cRowKey = env->GetStringUTFChars(jRowKey,&isCopy);
    cFamily = env->GetStringUTFChars(jFamily,&isCopy);
    cColumn = env->GetStringUTFChars(jColumn,&isCopy);
    cData = env->GetStringUTFChars(jData,&isCopy);
    env->CallStaticVoidMethod(jclazz,jmethodId,cTableName,cRowKey,cFamily,cColumn,jStartLocation,cData);
}

JNIEXPORT void JNICALL Java_HBaseUtils_callbackGet
        (JNIEnv * env, jstring jTableName, jstring jRowKey, jstring jFamily, jstring jColumn, jlong jTs, jstring jData){
    jclass jclazz = env->FindClass("HBaseUtils");
    jmethodID jmethodId = env->GetStaticMethodID(jclazz,"get","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;)V");
    const char *cTableName =NULL;const char *cRowKey =NULL;const char *cFamily =NULL;const char *cColumn=NULL;const char *cData=NULL;
    jboolean isCopy=JNI_TRUE;
    cTableName = env->GetStringUTFChars(jTableName,&isCopy);
    cRowKey = env->GetStringUTFChars(jRowKey,&isCopy);
    cFamily = env->GetStringUTFChars(jFamily,&isCopy);
    cColumn = env->GetStringUTFChars(jColumn,&isCopy);
    cData = env->GetStringUTFChars(jData,&isCopy);
    env->CallStaticVoidMethod(jclazz,jmethodId,cTableName,cRowKey,cFamily,cColumn,jTs,cData);
}

void HBaseUtils::create_table(const char *table_name, const char *column_family) {
    Java_HBaseUtils_callbackCreateTable(env, (jstring) table_name, (jstring) column_family);
}

void HBaseUtils::put(const char* table_name, const char* row_key, const char* family,const char* column,long long start_location,const char* data) {
    Java_HBaseUtils_callbackPut(env,(jstring) table_name,(jstring)row_key,(jstring)family,(jstring)column,(jlong)start_location,(jstring)data);
//    jstr = (*env)->;
}