cmake_minimum_required(VERSION 2.8)
project(libSMTPClient C CXX)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c90 -ansi -pedantic")

option(SMTP_CLIENT_STATIC "Build static library of SMTP Client" ON)
option(SMTP_CLIENT_SHARED "Build shared library of SMTP Client" ON)
option(SMTP_BUILD_EXAMPLE "Build sample program" OFF)

link_directories(${CMAKE_BINARY_DIR}/lib)

set(CMAKE_DEBUG_POSTFIX             "d")
set(CMAKE_RELEASE_POSTFIX           "")
set(CMAKE_RELWITHDEBINFO_POSTFIX    "")
set(CMAKE_MINSIZEREL_POSTFIX "")

set(SMTPCLIENT_INSTALLS)

if(SMTP_CLIENT_STATIC)
    add_library(smtpclient_static STATIC
        src/smtp_client.c
    )
    set_target_properties(smtpclient_static PROPERTIES OUTPUT_NAME smtpclient)
    list(APPEND SMTPCLIENT_INSTALLS smtpclient_static)
    set_target_properties(smtpclient_static PROPERTIES PUBLIC_HEADER src/smtp_client.h)
    set_target_properties(smtpclient_static
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

if(SMTP_CLIENT_SHARED)
    add_library(smtpclient SHARED
        src/smtp_client.c
    )
    target_link_libraries(smtpclient ssl crypto)
    list(APPEND SMTPCLIENT_INSTALLS smtpclient)
    set_target_properties(smtpclient PROPERTIES PUBLIC_HEADER src/smtp_client.h)
    set_target_properties(smtpclient
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

install(TARGETS ${SMTPCLIENT_INSTALLS}
        RUNTIME DESTINATION "bin"
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "lib"
        FRAMEWORK DESTINATION "lib"
        INCLUDES DESTINATION "include")

install(FILES
        src/smtp_client.h
        DESTINATION include
)

if(SMTP_BUILD_EXAMPLE)
    set(SMTP_SAMPLE_HOSTNAME    "smtp.example.ru" CACHE STRING "Hostname for sample program")
    set(SMTP_SAMPLE_PORT        25 CACHE INTEGER "Port for sample program")
    set(SMTP_SAMPLE_NAME_FROM   "Mr. Fox" CACHE STRING "Sender's name")
    set(SMTP_SAMPLE_MAIL_FROM   "mrfox@example.ru" CACHE STRING "Mail From")
    set(SMTP_SAMPLE_NAME_TO     "Ms. Vixen" CACHE STRING "Receipient")
    set(SMTP_SAMPLE_MAIL_TO     "msvixen@example.ru" CACHE STRING "Mail To")
    set(SMTP_SAMPLE_LOGIN       "mrfox@example.ru" CACHE STRING "Login to sample SMTP server")
    set(SMTP_SAMPLE_PASSWORD    "mypassword" CACHE STRING "Password for a sample SMTP server")
    set(SMTP_SAMPLE_USE_SSL     FALSE CACHE BOOLEAN "Use SSL-SMTP")

    add_executable(smtp_test
        sample.c
    )

    target_include_directories(smtp_test
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
    )

    target_compile_definitions(smtp_test PRIVATE
        -DSMTP_SAMPLE_HOSTNAME="${SMTP_SAMPLE_HOSTNAME}"
        -DSMTP_SAMPLE_PORT=${SMTP_SAMPLE_PORT}
        -DSMTP_SAMPLE_LOGIN="${SMTP_SAMPLE_LOGIN}"
        -DSMTP_SAMPLE_NAME_FROM="${SMTP_SAMPLE_NAME_FROM}"
        -DSMTP_SAMPLE_MAIL_FROM="${SMTP_SAMPLE_MAIL_FROM}"
        -DSMTP_SAMPLE_NAME_TO="${SMTP_SAMPLE_NAME_TO}"
        -DSMTP_SAMPLE_MAIL_TO="${SMTP_SAMPLE_MAIL_TO}"
        -DSMTP_SAMPLE_PASSWORD="${SMTP_SAMPLE_PASSWORD}"
        -DSMTP_SAMPLE_USE_SSL=$<BOOL:${SMTP_SAMPLE_USE_SSL}>
    )

    file(COPY
        attachments/foxy.png
        attachments/Jazz_walts.mid
        DESTINATION "${CMAKE_BINARY_DIR}/bin"
    )

    set_target_properties(smtp_test
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    if(SMTP_CLIENT_STATIC AND NOT SMTP_CLIENT_SHARED)
        add_dependencies(smtp_test smtpclient_static)
        target_link_libraries(smtp_test smtpclient ssl crypto)
    else()
        add_dependencies(smtp_test smtpclient)
        target_link_libraries(smtp_test smtpclient)
    endif()
endif()
