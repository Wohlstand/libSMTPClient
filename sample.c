/*
 * Simplle SMTP Client - a simple C library to send mail by SMTP protocol
 *
 * Copyright (c) 2018-2025 Vitaliy Novichkov <admin@wohlnet.ru>
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef SMTP_SAMPLE_HOSTNAME
#define SMTP_SAMPLE_HOSTNAME "smtp.example.ru"
#endif

#ifndef SMTP_SAMPLE_PORT
#define SMTP_SAMPLE_PORT 25
#endif

#ifndef SMTP_SAMPLE_NAME_FROM
#define SMTP_SAMPLE_NAME_FROM   "Mr. Fox"
#endif

#ifndef SMTP_SAMPLE_MAIL_FROM
#define SMTP_SAMPLE_MAIL_FROM   "mrfox@example.ru"
#endif

#ifndef SMTP_SAMPLE_NAME_TO
#define SMTP_SAMPLE_NAME_TO     "Ms. Vixen"
#endif

#ifndef SMTP_SAMPLE_MAIL_TO
#define SMTP_SAMPLE_MAIL_TO     "msvixen@example.ru"
#endif

#ifndef SMTP_SAMPLE_LOGIN
#define SMTP_SAMPLE_LOGIN       "mrfox@example.ru"
#endif

#ifndef SMTP_SAMPLE_PASSWORD
#define SMTP_SAMPLE_PASSWORD    "mypassword"
#endif

#ifndef SMTP_SAMPLE_USE_SSL
#define SMTP_SAMPLE_USE_SSL     0
#endif

#include <smtp_client.h>

int main()
{
    ssize_t ret;
    SMTP_Client *smtp;

    int security = SMTP_NONSECURE;
    const char *smtpServer     = SMTP_SAMPLE_HOSTNAME;
    const char *smtpLogin      = SMTP_SAMPLE_LOGIN;
    const char *smtpPassword   = SMTP_SAMPLE_PASSWORD;

    const char *letterSubj     = "Hello :3";
    const char *letterBody     = "I love you, <b>My Little Vixie!</b>! <span style=\"font-size: 50px;\">:3</size><br>";

    if(smtp_init(&smtp) < 0)
    {
        fprintf(stderr, "Can't initialize SMTP!\n");
        return 1;
    }

    /* To turn on debug output, set those flags */
    smtp->debugPrint  = 1;
    smtp->debugStream = stderr;

    /* Initialize letter data */
    ret = smtp_createLetter(smtp,
                            SMTP_TextHTML,
                            SMTP_SAMPLE_NAME_FROM, SMTP_SAMPLE_MAIL_FROM,
                            SMTP_SAMPLE_NAME_TO, SMTP_SAMPLE_MAIL_TO,
                            letterSubj, letterBody);

    if(ret >= 0)
        ret = smtp_attachFile(smtp, "./foxy.png");

    if(ret >= 0)
        ret = smtp_attachFile(smtp, "./Jazz_walts.mid");

    if(ret >= 0)
        ret = smtp_endLetter(smtp);

    if(ret < 0)
    {
        fprintf(stderr, "prepare FAILED ... [%s]\n", smtp->errorString);
        smtp_free(&smtp);
        return 1;
    }
    printf("prepare OK ...\n");

    #if SMTP_SAMPLE_USE_SSL
        security = SMTP_SSL;
    #endif

    if(smtp_connect(smtp, smtpServer, SMTP_SAMPLE_PORT, security) < 0)
    {
        fprintf(stderr, "connect FAILED ... [%s]\n", smtp->errorString);
        smtp_free(&smtp);
        return 1;
    }

    printf("connect OK ...\n");

    ret = smtp_login(smtp, smtpLogin, smtpPassword);
    if(ret < 0)
    {
        fprintf(stderr, "auth FAILED ... [%s]\r\n", smtp->errorString);
        smtp_free(&smtp);
        return 1;
    }
    printf("auth OK ...\n");

    ret = smtp_sendLetter(smtp);
    if(ret < 0)
    {
        fprintf(stderr, "send FAIL ... [%s]\n", smtp->errorString);
        smtp_free(&smtp);
        return 1;
    }
    printf("send OK ...\n");

    smtp_free(&smtp);

    return 0;
}
