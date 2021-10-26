#include <cstring>
#include "HBaseUtils.h"
#include "jni.h"


jstring stoJstring(JNIEnv* env, const char* pat) {
    jclass strClass = env->FindClass("Ljava/lang/String;");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>",
                                           "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
    jstring encoding = env->NewStringUTF("utf-8");
    return (jstring)env->NewObject(strClass, ctorID, bytes,
                                      encoding);
}

/*JNI字符串转C字符串*/
char* jstringTostring(JNIEnv* env, jstring jstr) {
    char* rtn = nullptr;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen> 0) {
        rtn = (char*)malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

/* 创建java虚拟机
 *	jvm : java虚拟机
 *	env	: JNI运行环境
 */
jint HBaseUtils::create_VM(JavaVM **jvm, JNIEnv** env) {
    JavaVMInitArgs args;
    JavaVMOption options[2];

    args.version = JNI_VERSION_1_8;  //	java版本
    args.nOptions = 2;
    options[0].optionString = "-Djava.compiler=NONE";
    options[1].optionString = "-Djava.class.path=/home/franklin/smit-project/Waveform-Retrieve/target/Waveform-Retrieve-1.0-SNAPSHOT-jar-with-dependencies.jar";  	// 配置java类的目录为当前目录
    args.options = options;
    args.ignoreUnrecognized = JNI_FALSE;

    return JNI_CreateJavaVM(jvm, (void **)env, &args);  // 创建java虚拟机
}

JNIEXPORT void JNICALL Java_HBaseUtils_callbackhello
        (JNIEnv * env){
    jclass jclazz = env->FindClass("HBaseUtils");
    std::cout<<jclazz<<std::endl;
    jmethodID jmethodId = env->GetStaticMethodID(jclazz,"hello","()V");
    std::cout<<jmethodId<<std::endl;
    env->CallStaticVoidMethod(jclazz,jmethodId);
}

JNIEXPORT jobject JNICALL Java_HBaseUtils_callbackHBaseUtils
        (JNIEnv * env){
    jclass jclazz = env->FindClass("HBaseUtils");
    jmethodID jmethodId = env->GetMethodID(jclazz,"<init>","()V");
    jobject  jobj=env->NewObject(jclazz,jmethodId);
    if(jobj== nullptr){
        std::cout << "4444" << std::endl;
    }
    std::cout << "3333" << std::endl;
    return jobj;
}

JNIEXPORT void JNICALL Java_HBaseUtils_callbackCreateTable
        (JNIEnv * env, jobject jobj, jstring jstrT , jstring jstrF){
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
//    const char *cStrT = nullptr;
//    const char *cStrF = nullptr;
//    jboolean isCopy=JNI_TRUE;
//    cStrT = env->GetStringUTFChars(jstrT,&isCopy);
//    cStrF = env->GetStringUTFChars(jstrF,&isCopy);
//    std::cout << "get string t:" << cStrT << ", f:" << cStrF << std::endl;
//    env->CallStaticVoidMethod(jclazz,jmethodId, jstrT,jstrF);
    env->CallObjectMethod(jobj,jmethodId,jstrT, jstrF);
}

JNIEXPORT void JNICALL Java_HBaseUtils_callbackPut
        (JNIEnv * env, jobject jobj, jstring jTableName, jstring jRowKey, jstring jFamily, jstring jColumn, jlong jStartLocation, jstring jData){
    jclass jclazz = env->FindClass("HBaseUtils");
    jmethodID jmethodId = env->GetStaticMethodID(jclazz,"put","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;)V");
//    const char *cTableName = nullptr;
//    const char *cRowKey = nullptr;
//    const char *cFamily = nullptr;
//    const char *cColumn = nullptr;
//    const char *cData = nullptr;
    jboolean isCopy=JNI_TRUE;
//    cTableName = env->GetStringUTFChars(jTableName,&isCopy);
//    cRowKey = env->GetStringUTFChars(jRowKey,&isCopy);
//    cFamily = env->GetStringUTFChars(jFamily,&isCopy);
//    cColumn = env->GetStringUTFChars(jColumn,&isCopy);
//    cData = env->GetStringUTFChars(jData,&isCopy);
//    env->CallStaticVoidMethod(jclazz,jmethodId,jTableName,jRowKey,jFamily,jColumn,jStartLocation,jData);
    env->CallObjectMethod(jobj,jmethodId,jTableName,jRowKey,jFamily,jColumn,jStartLocation,jData);
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
//    env->CallStaticVoidMethod(jclazz,jmethodId,cTableName,cRowKey,cFamily,cColumn,jTs,cData);
    env->CallStaticVoidMethod(jclazz, jmethodId, jTableName, jRowKey, jFamily, jColumn, jTs, jData);
}

void HBaseUtils::hello() {
    Java_HBaseUtils_callbackhello(env);
}

jobject HBaseUtils::create_HBaseUtils() {
    return Java_HBaseUtils_callbackHBaseUtils(env);
}

void HBaseUtils::create_table(const char *table_name, const char *column_family) {
    jstring jstrT= stoJstring(env,table_name);
    jstring jstrF= stoJstring(env,column_family);
    Java_HBaseUtils_callbackCreateTable(env, jobj,jstrT, jstrF);
    std::cout << "88888" << std::endl;
}

void HBaseUtils::put(const char* table_name, const char* row_key, const char* family,const char* column,long long start_location,const char* data) {
    jstring jtable_name= stoJstring(env,table_name);
    jstring jrow_key= stoJstring(env,row_key);
    jstring jfamily= stoJstring(env,family);
    jstring jcolumn= stoJstring(env,column);
    jstring jdata= stoJstring(env,data);
    Java_HBaseUtils_callbackPut(env, jobj, jtable_name, jrow_key,jfamily,jcolumn,(jlong)start_location,jdata);
}