#ifndef AVALON_PLATFORM_ANDROID_GNUSTL_STRING_FIXES_H
#define AVALON_PLATFORM_ANDROID_GNUSTL_STRING_FIXES_H

#include <avalon/utils/platform.h>

#if AVALON_PLATFORM_IS_ANDROID
#if !_GLIBCXX_HAVE_BROKEN_VSWPRINTF

namespace std {

string to_string(int val);
int stoi(const std::string &str);

} // namespace std

#endif /* !_GLIBCXX_HAVE_BROKEN_VSWPRINTF */
#endif /* AVALON_PLATFORM_IS_ANDROID */
#endif /* AVALON_PLATFORM_ANDROID_GNUSTL_STRING_FIXES_H */
