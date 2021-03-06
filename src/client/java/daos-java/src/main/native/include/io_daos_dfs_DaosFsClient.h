/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class io_daos_dfs_DaosFsClient */

#ifndef _Included_io_daos_dfs_DaosFsClient
#define _Included_io_daos_dfs_DaosFsClient
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    move
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_move__JLjava_lang_String_2Ljava_lang_String_2
  (JNIEnv *, jobject, jlong, jstring, jstring);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    move
 * Signature: (JJLjava/lang/String;JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_move__JJLjava_lang_String_2JLjava_lang_String_2
  (JNIEnv *, jobject, jlong, jlong, jstring, jlong, jstring);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    mkdir
 * Signature: (JLjava/lang/String;IZ)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_mkdir
  (JNIEnv *, jobject, jlong, jstring, jint, jboolean);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    createNewFile
 * Signature: (JLjava/lang/String;Ljava/lang/String;IILjava/lang/String;IZ)J
 */
JNIEXPORT jlong JNICALL Java_io_daos_dfs_DaosFsClient_createNewFile
  (JNIEnv *, jobject, jlong, jstring, jstring, jint, jint, jstring, jint, jboolean);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    delete
 * Signature: (JLjava/lang/String;Ljava/lang/String;Z)Z
 */
JNIEXPORT jboolean JNICALL Java_io_daos_dfs_DaosFsClient_delete
  (JNIEnv *, jobject, jlong, jstring, jstring, jboolean);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    daosOpenPool
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)J
 */
JNIEXPORT jlong JNICALL Java_io_daos_dfs_DaosFsClient_daosOpenPool
  (JNIEnv *, jclass, jstring, jstring, jstring, jint);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    daosOpenCont
 * Signature: (JLjava/lang/String;I)J
 */
JNIEXPORT jlong JNICALL Java_io_daos_dfs_DaosFsClient_daosOpenCont
  (JNIEnv *, jclass, jlong, jstring, jint);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    daosCloseContainer
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_daosCloseContainer
  (JNIEnv *, jclass, jlong);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    daosClosePool
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_daosClosePool
  (JNIEnv *, jclass, jlong);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsSetPrefix
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_dfsSetPrefix
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsLookup
 * Signature: (JJLjava/lang/String;IJ)J
 */
JNIEXPORT jlong JNICALL Java_io_daos_dfs_DaosFsClient_dfsLookup__JJLjava_lang_String_2IJ
  (JNIEnv *, jobject, jlong, jlong, jstring, jint, jlong);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsLookup
 * Signature: (JLjava/lang/String;IJ)J
 */
JNIEXPORT jlong JNICALL Java_io_daos_dfs_DaosFsClient_dfsLookup__JLjava_lang_String_2IJ
  (JNIEnv *, jobject, jlong, jstring, jint, jlong);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsGetSize
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_io_daos_dfs_DaosFsClient_dfsGetSize
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsDup
 * Signature: (JJI)J
 */
JNIEXPORT jlong JNICALL Java_io_daos_dfs_DaosFsClient_dfsDup
  (JNIEnv *, jobject, jlong, jlong, jint);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsRelease
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_dfsRelease
  (JNIEnv *, jobject, jlong);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsRead
 * Signature: (JJJJJI)J
 */
JNIEXPORT jlong JNICALL Java_io_daos_dfs_DaosFsClient_dfsRead
  (JNIEnv *, jobject, jlong, jlong, jlong, jlong, jlong, jint);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsWrite
 * Signature: (JJJJJI)J
 */
JNIEXPORT jlong JNICALL Java_io_daos_dfs_DaosFsClient_dfsWrite
  (JNIEnv *, jobject, jlong, jlong, jlong, jlong, jlong, jint);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsReadDir
 * Signature: (JJI)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_io_daos_dfs_DaosFsClient_dfsReadDir
  (JNIEnv *, jobject, jlong, jlong, jint);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsOpenedObjStat
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_dfsOpenedObjStat
  (JNIEnv *, jobject, jlong, jlong, jlong);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsSetExtAttr
 * Signature: (JJLjava/lang/String;Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_dfsSetExtAttr
  (JNIEnv *, jobject, jlong, jlong, jstring, jstring, jint);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsGetExtAttr
 * Signature: (JJLjava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_io_daos_dfs_DaosFsClient_dfsGetExtAttr
  (JNIEnv *, jobject, jlong, jlong, jstring, jint);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsRemoveExtAttr
 * Signature: (JJLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_dfsRemoveExtAttr
  (JNIEnv *, jobject, jlong, jlong, jstring);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsGetChunkSize
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_io_daos_dfs_DaosFsClient_dfsGetChunkSize
  (JNIEnv *, jclass, jlong);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsGetMode
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_io_daos_dfs_DaosFsClient_dfsGetMode
  (JNIEnv *, jclass, jlong);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsIsDirectory
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_io_daos_dfs_DaosFsClient_dfsIsDirectory
  (JNIEnv *, jclass, jint);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsMountFs
 * Signature: (JJZ)J
 */
JNIEXPORT jlong JNICALL Java_io_daos_dfs_DaosFsClient_dfsMountFs
  (JNIEnv *, jclass, jlong, jlong, jboolean);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsMountFsOnRoot
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_io_daos_dfs_DaosFsClient_dfsMountFsOnRoot
  (JNIEnv *, jclass, jlong);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsUnmountFsOnRoot
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_dfsUnmountFsOnRoot
  (JNIEnv *, jclass, jlong);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dfsUnmountFs
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_dfsUnmountFs
  (JNIEnv *, jclass, jlong);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dunsCreatePath
 * Signature: (JLjava/lang/String;JI)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_io_daos_dfs_DaosFsClient_dunsCreatePath
  (JNIEnv *, jclass, jlong, jstring, jlong, jint);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dunsResolvePath
 * Signature: (Ljava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_io_daos_dfs_DaosFsClient_dunsResolvePath
  (JNIEnv *, jclass, jstring);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dunsGetAppInfo
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_io_daos_dfs_DaosFsClient_dunsGetAppInfo
  (JNIEnv *, jclass, jstring, jstring, jint);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dunsSetAppInfo
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_dunsSetAppInfo
  (JNIEnv *, jclass, jstring, jstring, jstring);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dunsDestroyPath
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_dunsDestroyPath
  (JNIEnv *, jclass, jlong, jstring);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    dunsParseAttribute
 * Signature: (Ljava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_io_daos_dfs_DaosFsClient_dunsParseAttribute
  (JNIEnv *, jclass, jstring);

/*
 * Class:     io_daos_dfs_DaosFsClient
 * Method:    daosFinalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_io_daos_dfs_DaosFsClient_daosFinalize
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
