#ifndef AWS_SECURITY_H_
#define AWS_SECURITY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "SSLClient/bearssl.h"

#define TAs_NUM 1
#define THING_NAME "ESP32_TEMP"
#define MQTT_PACKET_SIZE 1024

    const char *mqttServer = "a3ei8e16py0910-ats.iot.us-east-1.amazonaws.com";
    const char publishShadowUpdate[] = "$aws/things/" THING_NAME "/shadow/update";
    char publishPayload[MQTT_PACKET_SIZE];

    char *subscribeTopic[5] =
        {
            "$aws/things/" THING_NAME "/shadow/update/accepted",
            "$aws/things/" THING_NAME "/shadow/update/rejected",
            "$aws/things/" THING_NAME "/shadow/update/delta",
            "$aws/things/" THING_NAME "/shadow/get/accepted",
            "$aws/things/" THING_NAME "/shadow/get/rejected"};

    const char my_cert[] =
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDWTCCAkGgAwIBAgIUG2rXcAUBk1U/Q/2s2kfmg53EzOgwDQYJKoZIhvcNAQEL\n"
        "BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"
        "SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIyMDkwMTA5NDcx\n"
        "MFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"
        "ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALxdgbNeuAEJGGd+lYK9\n"
        "uYc1BxXobXBWgNgJe/nSGYOj/35JIB/vPbJMRkRExusl5xnwfCyyCTTpYSmeK72I\n"
        "33RGLzuGPzJxVtHhAUh0VN3aCg7KOebGJD1qMWSZtbgHiJGr4tlZ9K21MSWs1lH1\n"
        "oiC58aE58g/98avKdl2O3n+j/NStAjy42QKzug2mdwEZdNLsS5mLZ31UE12rb2Wf\n"
        "ZUskZgOVuiACp6KjNa9Dah4Fy9dqM5esIdCRL9QxCZnv8yFD/pQkAIyu+7rQdWDN\n"
        "G7gl+4pmBXPfXQuhC4tPY0ccQWfPKglsPf2xmEpQgnfihihijlvdGP90fgz/lITe\n"
        "Q1ECAwEAAaNgMF4wHwYDVR0jBBgwFoAUCH8ptygk40UMDIpT3bpV83xPwNswHQYD\n"
        "VR0OBBYEFPOcyr5N1NX1wurbccC+F1isZvvQMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"
        "AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAX9H/0DKTDRJMglUS2puM8kKa2\n"
        "pZe31MJAhFkxgj//H2wtMXa6Wf3iTelN6p3iW70V5tL/+Kb5okhSertmfvaH3txs\n"
        "YiFkdos7WWk7s0UyrzfagPtnm89sRAFJyBuM5Blu/MPCmNTwucTDybg9JYazJhYq\n"
        "yzVJslShU/I3w8czVxl/i3fpePtmRt5Jxf4h9wZOvxtEy6v0Vr/Cu8QlQzwjIllR\n"
        "x6KguEitPYhsBzQarevvi521lssTHpb5YN7R01vtU11s7Gk0qef9nny2AG6AS0Hq\n"
        "25rKNUEBgfiOri6y87csVQmNYfeLvN5dItPzda3xC2Ti+sYmi1Zvig4C0hLa\n"
        "-----END CERTIFICATE-----\n";

    const char my_key[] =
        "-----BEGIN RSA PRIVATE KEY-----\n"
        "MIIEpQIBAAKCAQEAvF2Bs164AQkYZ36Vgr25hzUHFehtcFaA2Al7+dIZg6P/fkkg\n"
        "H+89skxGRETG6yXnGfB8LLIJNOlhKZ4rvYjfdEYvO4Y/MnFW0eEBSHRU3doKDso5\n"
        "5sYkPWoxZJm1uAeIkavi2Vn0rbUxJazWUfWiILnxoTnyD/3xq8p2XY7ef6P81K0C\n"
        "PLjZArO6DaZ3ARl00uxLmYtnfVQTXatvZZ9lSyRmA5W6IAKnoqM1r0NqHgXL12oz\n"
        "l6wh0JEv1DEJme/zIUP+lCQAjK77utB1YM0buCX7imYFc99dC6ELi09jRxxBZ88q\n"
        "CWw9/bGYSlCCd+KGKGKOW90Y/3R+DP+UhN5DUQIDAQABAoIBADzmqpmYxTsoeqhz\n"
        "h9lmiAaMAYd2x3NrtGqk3l2OLUufiWqGMAnDKxpriMhnyDyc25RKaYt14H4n+o8f\n"
        "IZs9FqocSwcex24CsQRYkDahzhKQfXH8ti3D4FGu9NtGyzb90Izwmv6VFAPZslMx\n"
        "B3YmSc308IelGXtMKofVhOGI4K05hOhuoKS93CqP6iqxTiGy+aZfXfNSmtEmpg1t\n"
        "lezlLv1SbAbdXFW1RvGFROnSWWuTNgEtED83HJS6pDLwb1gD+wkd82hDlvHe5L+m\n"
        "E0cALme+IDyBUDu7oucAsHGbzDdBHYH7CEznuuEA4NYN8FLRcKWVNmSbzrigvE90\n"
        "EU60T/UCgYEA4WTtdC1BgFrticqyJXvdl0IQRAADKZRaqwpwY8CPfFYQgHuyJ4+T\n"
        "LeRC1M5hrKSp0zmJR2rk/VylieuaUgnvmcCPjZXX12x6bBAnfOntJ6e04p3arChX\n"
        "7iBJwoHZhIkMzMPy+PY8yFHgcHPW83BhqSYeb0mEwLUZvAVIEbAqYkcCgYEA1fFk\n"
        "FUlwvRwk/0ly2kYLiMi5qlIDf4E5A/L3u9z8ub90OFkB0qIByiZr/u5Ay91Eg3SV\n"
        "F6pypSlZbHPuj1laCxlWkm7Z3Wdqw7VOYe/hRwfMA0D6GvPjwRdxuhHzcydlpk6U\n"
        "pv5U+d9u4noyfUWnmpVKXjBXdXyb6l5pLQqLIacCgYEAmnQ9PDWfA0RMLvaV6ZXe\n"
        "Hb8Jwm93u1CMaVU3+0ODkCKyIDmjtfRvWS1olsfuqw4TyklNR4F69hw83Cx1je59\n"
        "IJGSTeX3CQqvQco4JSNwfpIwJHMj3IGw7990bDPrSSVZfiBxKiYjNOrWsHjRPfUv\n"
        "F2ilxoFM1XnE7tF3t5dPFBUCgYEAwqI28P69J+5A4lcJu/XZbHlgH8k5EXiYTDVw\n"
        "l2J2MO4yZ0jiVaO5iRTnTyeln1IPkKAxOzR/1C4IAph9wcACIPLORa2/WqNJB/8b\n"
        "7hFzm1FGkvc8EeUroHEZr7PBDVQcL+5xYnMjD6frkvKkmD8UOZfz7jiAeIwCtHdr\n"
        "q+RZqyUCgYEAgSfV0pRbbWL1+CvsyrzYqYoLY3w6Xy7NF7dNbll8OaLcaAyUEfsi\n"
        "D04COWLTJ4rAoTbTJtHyiUSoaV929yPfg1P5MlL6vogJTJrV8MeTTFo3RyIaQoF8\n"
        "8+OjBsMa4hrxj45has7BDgc4WD615El8b9o7brd4um7iEctt3nh+Qn8=\n"
        "-----END RSA PRIVATE KEY-----\n";

    // This file is created using AmazonRootCA1.pem from https://www.amazontrust.com/repository/AmazonRootCA1.pem
    static const unsigned char TA_DN0[] =
        {
            0x30,
            0x39,
            0x31,
            0x0b,
            0x30,
            0x09,
            0x06,
            0x03,
            0x55,
            0x04,
            0x06,
            0x13,
            0x02,
            0x55,
            0x53,
            0x31,
            0x0f,
            0x30,
            0x0d,
            0x06,
            0x03,
            0x55,
            0x04,
            0x0a,
            0x13,
            0x06,
            0x41,
            0x6d,
            0x61,
            0x7a,
            0x6f,
            0x6e,
            0x31,
            0x19,
            0x30,
            0x17,
            0x06,
            0x03,
            0x55,
            0x04,
            0x03,
            0x13,
            0x10,
            0x41,
            0x6d,
            0x61,
            0x7a,
            0x6f,
            0x6e,
            0x20,
            0x52,
            0x6f,
            0x6f,
            0x74,
            0x20,
            0x43,
            0x41,
            0x20,
            0x31,
    };

    static const unsigned char TA_RSA_N0[] =
        {
            0xb2,
            0x78,
            0x80,
            0x71,
            0xca,
            0x78,
            0xd5,
            0xe3,
            0x71,
            0xaf,
            0x47,
            0x80,
            0x50,
            0x74,
            0x7d,
            0x6e,
            0xd8,
            0xd7,
            0x88,
            0x76,
            0xf4,
            0x99,
            0x68,
            0xf7,
            0x58,
            0x21,
            0x60,
            0xf9,
            0x74,
            0x84,
            0x01,
            0x2f,
            0xac,
            0x02,
            0x2d,
            0x86,
            0xd3,
            0xa0,
            0x43,
            0x7a,
            0x4e,
            0xb2,
            0xa4,
            0xd0,
            0x36,
            0xba,
            0x01,
            0xbe,
            0x8d,
            0xdb,
            0x48,
            0xc8,
            0x07,
            0x17,
            0x36,
            0x4c,
            0xf4,
            0xee,
            0x88,
            0x23,
            0xc7,
            0x3e,
            0xeb,
            0x37,
            0xf5,
            0xb5,
            0x19,
            0xf8,
            0x49,
            0x68,
            0xb0,
            0xde,
            0xd7,
            0xb9,
            0x76,
            0x38,
            0x1d,
            0x61,
            0x9e,
            0xa4,
            0xfe,
            0x82,
            0x36,
            0xa5,
            0xe5,
            0x4a,
            0x56,
            0xe4,
            0x45,
            0xe1,
            0xf9,
            0xfd,
            0xb4,
            0x16,
            0xfa,
            0x74,
            0xda,
            0x9c,
            0x9b,
            0x35,
            0x39,
            0x2f,
            0xfa,
            0xb0,
            0x20,
            0x50,
            0x06,
            0x6c,
            0x7a,
            0xd0,
            0x80,
            0xb2,
            0xa6,
            0xf9,
            0xaf,
            0xec,
            0x47,
            0x19,
            0x8f,
            0x50,
            0x38,
            0x07,
            0xdc,
            0xa2,
            0x87,
            0x39,
            0x58,
            0xf8,
            0xba,
            0xd5,
            0xa9,
            0xf9,
            0x48,
            0x67,
            0x30,
            0x96,
            0xee,
            0x94,
            0x78,
            0x5e,
            0x6f,
            0x89,
            0xa3,
            0x51,
            0xc0,
            0x30,
            0x86,
            0x66,
            0xa1,
            0x45,
            0x66,
            0xba,
            0x54,
            0xeb,
            0xa3,
            0xc3,
            0x91,
            0xf9,
            0x48,
            0xdc,
            0xff,
            0xd1,
            0xe8,
            0x30,
            0x2d,
            0x7d,
            0x2d,
            0x74,
            0x70,
            0x35,
            0xd7,
            0x88,
            0x24,
            0xf7,
            0x9e,
            0xc4,
            0x59,
            0x6e,
            0xbb,
            0x73,
            0x87,
            0x17,
            0xf2,
            0x32,
            0x46,
            0x28,
            0xb8,
            0x43,
            0xfa,
            0xb7,
            0x1d,
            0xaa,
            0xca,
            0xb4,
            0xf2,
            0x9f,
            0x24,
            0x0e,
            0x2d,
            0x4b,
            0xf7,
            0x71,
            0x5c,
            0x5e,
            0x69,
            0xff,
            0xea,
            0x95,
            0x02,
            0xcb,
            0x38,
            0x8a,
            0xae,
            0x50,
            0x38,
            0x6f,
            0xdb,
            0xfb,
            0x2d,
            0x62,
            0x1b,
            0xc5,
            0xc7,
            0x1e,
            0x54,
            0xe1,
            0x77,
            0xe0,
            0x67,
            0xc8,
            0x0f,
            0x9c,
            0x87,
            0x23,
            0xd6,
            0x3f,
            0x40,
            0x20,
            0x7f,
            0x20,
            0x80,
            0xc4,
            0x80,
            0x4c,
            0x3e,
            0x3b,
            0x24,
            0x26,
            0x8e,
            0x04,
            0xae,
            0x6c,
            0x9a,
            0xc8,
            0xaa,
            0x0d,
    };

    static const unsigned char TA_RSA_E0[] =
        {
            0x01,
            0x00,
            0x01,
    };

    static const br_x509_trust_anchor TAs[] =
        {
            {{(unsigned char *)TA_DN0, sizeof TA_DN0},
             BR_X509_TA_CA,
             {BR_KEYTYPE_RSA,
              {.rsa =
                   {
                       (unsigned char *)TA_RSA_N0,
                       sizeof TA_RSA_N0,
                       (unsigned char *)TA_RSA_E0,
                       sizeof TA_RSA_E0,
                   }}}},
    };

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef _CERTIFICATES_H_ */
