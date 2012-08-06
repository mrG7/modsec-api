#!/bin/bash

STANDALONE_PATH=/home/varrunr/tomcat/github/modsec-api/mod-security/standalone
JAVA_HOME=/home/varrunr/Downloads/jdk1.7.0_05
APR_PATH=/usr/include/apr-1.0
LIBXML_PATH=/usr/include/libxml2
APACHE_PATH=/usr/include/apache2
JNI_HEAD_PATH=/home/varrunr/tomcat/github/modsec-api/modsec-conn-test/jnilib

if [ "$1" == 'so' ]; then 
    
    echo 'Building shared library'; 
    g++ -shared main.cpp -I$STANDALONE_PATH -I$APACHE_PATH -I$APR_PATH -I$JNI_HEAD_PATH -I$STANDALONE_PATH/../apache2 -I$LIBXML_PATH -L/usr/lib/ -L$STANDALONE_PATH -lstandalone -L$STANDALONE_PATH -lapr-1 -I$JAVA_HOME/include -I$JAVA_HOME/include/linux -o libmodsecurity.so;

else echo 'Building executable'; 
    g++ main.cpp -I$STANDALONE_PATH -I$APACHE_PATH -I$APR_PATH -I$JNI_HEAD_PATH -I$STANDALONE_PATH/../apache2 -I$LIBXML_PATH -L/usr/lib/ -L$STANDALONE_PATH -lstandalone -L$STANDALONE_PATH -lapr-1 -I$JAVA_HOME/include -I$JAVA_HOME/include/linux -o modsec
fi
