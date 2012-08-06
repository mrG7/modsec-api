JAVA_HOME=/home/varrunr/Downloads/jdk1.7.0_05
TARGET_CLASS=vulnapp.modsecurity.wrappers.ModSecurityWrapper
$JAVA_HOME/bin/javah -jni -classpath bin -o jnilib/ModSecJNIwrapper.h $TARGET_CLASS
