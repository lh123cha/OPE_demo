//
// Created by 14913 on 2021/4/12.
//

#ifndef OPE_DEMO_BASE64_H
#define OPE_DEMO_BASE64_H

#include <string>
std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif //OPE_DEMO_BASE64_H
