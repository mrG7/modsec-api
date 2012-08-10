JAVA_HOME=/home/varrunr/Downloads/jdk1.7.0_05
CATALINA_HOME=/home/varrunr/Downloads/apache-tomcat-6.0.35
TARGET_CLASS=vulnapp.modsecurity.wrappers.ModSecurityWrapper
$JAVA_HOME/bin/javah -classpath build/classes/:$CATALINA_HOME/lib/servlet-api.jar -o jnilib/ModSecJNIwrapper.h  -jni vulnapp.modsecurity.wrappers.ModSecurityWrapper
