/*
 * Simplle SMTP Client - a simple C library to send mail by SMTP protocol
 *
 * Copyright (c) 2018 Vitaliy Novichkov <admin@wohlnet.ru>
 *
 * Code is partially based on https://github.com/weiofcn/smtp
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef SMTPC_H
#define SMTPC_H

#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SMTP_CLIENT_VERSION_MAJOR 1
#define SMTP_CLIENT_VERSION_MINOR 0
#define SMTP_CLIENT_VERSION_PATCH 0

struct SMTP_Private;

typedef struct
{
    /*! Internal private variables*/
    struct SMTP_Private *p;
    /*! Turn on debug print*/
    int debugPrint;
    /*! Output stream (file, stdout or stderr on your choice)*/
    FILE *debugStream;
    /*! Last error string*/
    char *errorString;
} SMTP_Client;

typedef enum
{
    /*! Insecure RAW SMTP*/
    SMTP_NONSECURE,
    /*! TSL SMTP [WIP]*/
    SMTP_TSL,
    /*! SSL-SMTP*/
    SMTP_SSL
} SMTP_Security;

typedef enum
{
    /*! Plain text */
    SMTP_TextPlain,
    /*! HMTL */
    SMTP_TextHTML
} SMTP_BodyFormat;

/**
 * @brief Initialize SMTP instance
 * @param smtp Pointer to pointer of SMTP instance
 * @return 0 on success, -1 on any error
 */
int smtp_init(SMTP_Client **smtp);
/**
 * @brief Clear SMTP instance and close any connections
 * @param smtp Pointer to pointer of SMTP instance
 * @return 0 on success, -1 on any error
 */
int smtp_free(SMTP_Client **smtp);

/**
 * @brief Connect to SMTP server
 * @param smtp Pointer to SMTP instance
 * @param smtpHostName Server name or IP address
 * @param smtpPort Port of SMTP server
 * @param security Connection type: non-encrypted or SSL
 * @return 0 on success, -1 on any error
 */
int smtp_connect(SMTP_Client *smtp, const char *smtpHostName, const unsigned short smtpPort, int security);

/**
 * @brief smtp_close Close SMTP instance
 * @param smtp Pointer to SMTP instance
 * @return 0 on success, -1 on any error
 */
int smtp_close(SMTP_Client *smtp);


/**
 * @brief Create the letter
 * @param smtp Pointer to SMTP instance
 * @param textFormat Format of the body text: plain text or HTML-formatted
 * @param fromName Name of sender
 * @param fromMail E-Mail address of sender
 * @param toName Name of receipient
 * @param toMail E-Mail address of receipient
 * @param mailSubject Subject of the letter
 * @param mailBody Body of the letter
 * @return 0 on success, -1 on any error
 */
ssize_t smtp_createLetter(SMTP_Client *smtp,
                          int textFormat,
                          const char *fromName, const char *fromMail,
                          const char *toName,   const char *toMail,
                          const char *mailSubject, const char *mailBody);


/**
 * @brief Attach a file by path
 * @param smtp Pointer to SMTP instance
 * @param filePath Path to file to attach
 * @return 0 on success, -1 on any error
 */
ssize_t smtp_attachFile(SMTP_Client *smtp, const char *filePath);

/**
 * @brief Finalize letter (Must be, or letter will NOT be accepted by SMTP server!)
 * @param smtp Pointer to SMTP instance
 * @return 0 on success, -1 on any error
 */
ssize_t smtp_endLetter(SMTP_Client *smtp);


/**
 * @brief Authentificate SMTP connection
 * @param smtp Pointer to SMTP instance
 * @param smtpLogin Login
 * @param smtpPasswd Password
 * @return 0 on success, -1 on any error
 */
int smtp_login(SMTP_Client *smtp, const char *smtpLogin, const char *smtpPasswd);

/**
 * @brief Send letter
 * @param smtp Pointer to SMTP instance
 * @return 0 on success, -1 on any error
 */
int smtp_sendLetter(SMTP_Client *smtp);

#ifdef __cplusplus
}
#endif

#endif /*SMTPC_H*/
