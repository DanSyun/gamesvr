# Install script for directory: /home/kean/Desktop/mysql-connector-c-6.1.9-src/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local/mysql")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/mysql.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/mysql_com.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_command.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/mysql_time.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_list.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_alloc.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/typelib.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/binary_log_types.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_dbug.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/m_string.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_sys.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_xml.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/mysql_embed.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_thread.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_thread_local.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/decimal.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/errmsg.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_global.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_getopt.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/sslopt-longopts.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_dir.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/sslopt-vars.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/sslopt-case.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/sql_common.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/keycache.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/m_ctype.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_compiler.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/mysql_com_server.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_byteorder.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/byte_order_generic.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/byte_order_generic_x86.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/little_endian.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/big_endian.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/thr_cond.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/thr_mutex.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/thr_rwlock.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/mysql_version.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/my_config.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/mysqld_ername.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/mysqld_error.h"
    "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/sql_state.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mysql" TYPE DIRECTORY FILES "/home/kean/Desktop/mysql-connector-c-6.1.9-src/include/mysql/" REGEX "/[^/]*\\.h$" REGEX "/psi\\_abi[^/]*$" EXCLUDE)
endif()

