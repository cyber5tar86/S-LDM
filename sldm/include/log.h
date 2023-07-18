#include <iostream>

#define LogFatal(_s) std::cerr << __FILE__ << ":" << __LINE__ << " [FATAL] " << _s << std::endl;
#define LogError(_s) std::cerr << __FILE__ << ":" << __LINE__ << " [ERROR] " << _s << std::endl;
#define LogWarning(_s)  std::cout << __FILE__ << ":" << __LINE__ << " [WARNING] " << _s << std::endl;
#define LogInfo(_s)  std::cout << __FILE__ << ":" << __LINE__ << " [INFO] " << _s << std::endl;
#define LogDebug(_s) std::cout << __FILE__ << ":" << __LINE__ << " [DEBUG] " << _s << std::endl;
#define LogTrage(_s) std::cout << __FILE__ << ":" << __LINE__ << " [TRACE] " << _s << std::endl;