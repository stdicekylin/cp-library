namespace debug {

}  // namespace debug

#ifdef LOCAL
#define CHECK(expr) assert(expr)
#else
#define CHECK(expr)
#endif